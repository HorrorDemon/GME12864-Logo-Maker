#include <Adafruit_SSD1306.h>
#include <arduino_logo.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

#define BUTTON_NEXT 2
#define BUTTON_PREV 1
#define BUTTON_UP 6
#define BUTTON_DOWN 20
#define BUTTON_LED 4
#define LED_PIN 5

const char* ssid = "MSR2";
const char* password = "lacaravanepasse2020";

const char* discordWebhook = "https://discord.com/api/webhooks/1477379048396296375/z6R7RM5ndJAS8hRg869UK07guI2VjwTk_WvtSeMiSCLfzcW-g23M8oL4CFLuVFriJPWM";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int currentLogo = 0;
int targetLogo = 0;
bool lastNext = HIGH;
bool lastPrev = HIGH;
bool lastLedBtn = HIGH;
bool lastUp = HIGH;
bool lastDown = HIGH;

bool splashShown = false;
bool animating = false;
int slideOffset = 0;
int slideDirection = 0;

int menuSelected = 0;
const int MENU_ITEMS_COUNT = 3;

const char* menuItems[] = {
  "Come eat.",
  "Come when you're done.",
  "Come as soon as possible, urgent !"
};

bool inMainMenu = false;

const int SLIDE_DURATION_MS = 320;
const int FRAME_DELAY_MS = 8;

unsigned long animStartTime = 0;

bool sendDiscordMessage(String text) {
  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  http.begin(discordWebhook);
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"content\": \"" + text + "\"}";

  int code = http.POST(payload);
  http.end();

  return (code == 200 || code == 204);
}

void drawLogoAt(int logoIndex, int xoffset) {
  display.clearDisplay();

  if (logoIndex == 3 && inMainMenu) {
    // ─── Mode MENU sur page principale ──────────────────────────────

    // Fond logo4 (optionnel – tu peux le garder ou le virer)
    const uint8_t* bmp = logo4;
    if (bmp) {
      display.drawBitmap(xoffset, 0, bmp, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    }

    // Titre en haut (gras simulé = double tracé ou texte + underline)
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(SCREEN_WIDTH/2 - 28, 4);   // centré approximatif
    display.print(F("Page 1 - Appel"));

    // Ligne sous le titre
    display.drawLine(8, 13, SCREEN_WIDTH-8, 13, WHITE);

    // Affichage du menu vertical
    int startY = 18;
    int lineHeight = 10;

    for (int i = 0; i < MENU_ITEMS_COUNT; i++) {
      int y = startY + i * lineHeight;

      // Item sélectionné → cadre ou inversion
      if (i == menuSelected) {
        // Cadre autour de l'item
        display.drawRect(6, y-2, SCREEN_WIDTH-12, lineHeight+2, WHITE);
        // Ou inversion (noir sur blanc) → plus visible sur certains logos
        // display.fillRect(6, y-2, SCREEN_WIDTH-12, lineHeight+2, WHITE);
        // display.setTextColor(BLACK);
      }

      display.setCursor(12, y);
      display.print(menuItems[i]);

      // Reset couleur si inversion
      display.setTextColor(WHITE);
    }

    // Footer habituel
    display.setTextSize(1);
    display.setCursor(4, 52);
    display.print(F("Beepr v0.1"));

  } else {
    // ─── Affichage normal des autres logos ──────────────────────────
    const uint8_t* bmp = nullptr;
    switch (logoIndex) {
      case 0: bmp = logo1; break;
      case 1: bmp = logo2; break;
      case 2: bmp = logo3; break;
      case 3: bmp = logo4; break;
      case 4: bmp = logo5; break;
    }
    if (bmp) {
      display.drawBitmap(xoffset, 0, bmp, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    }

    drawFooter();   // ton footer habituel avec Beepr v0.1 + Press si besoin
  }

  display.display();
}

// Petit tableau pour l'animation de chargement (style roue / spinner)
const uint8_t PROGMEM loading_frames[4][8] = {
  {0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00}, // -
  {0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x00}, // \
  {0x00, 0x00, 0x00, 0x42, 0x42, 0x00, 0x00, 0x00}, // |
  {0x00, 0x00, 0x00, 0x81, 0x81, 0x00, 0x00, 0x00}  // /
};

void startSlideAnimation(int direction) {
  // direction : +1 = next (slide vers gauche), -1 = prev (slide vers droite)
  slideDirection  = direction;
  targetLogo      = (currentLogo + direction + 5) % 5;
  slideOffset     = 0;
  animating       = true;
  animStartTime   = millis();
}

void updateSlideAnimation() {
  if (!animating) return;

  unsigned long now = millis();
  unsigned long elapsed = now - animStartTime;

  if (elapsed >= SLIDE_DURATION_MS) {
    // Fin de l'animation
    currentLogo = targetLogo;
    slideOffset = 0;
    animating   = false;
    drawLogoAt(currentLogo, 0);
    return;
  }

  // Progression linéaire (0 → 128 ou 0 → -128)
  int progress = (elapsed * SCREEN_WIDTH) / SLIDE_DURATION_MS;

  if (slideDirection > 0) {
    // Next → contenu actuel part à gauche → nouveau arrive de droite
    slideOffset = -progress;
  } else {
    // Prev → contenu actuel part à droite → nouveau arrive de gauche
    slideOffset = +progress;
  }

  // Dessin des deux écrans avec décalage
  display.clearDisplay();

  // Ancien logo (qui s'en va)
  drawLogoAt(currentLogo, slideOffset);

  // Nouveau logo (qui arrive)
  int newX = (slideDirection > 0) ? (SCREEN_WIDTH + slideOffset) : (slideOffset - SCREEN_WIDTH);
  drawLogoAt(targetLogo, newX);

  display.display();
}

void drawFooter() {
  // On dessine le texte en bas à gauche
  display.setTextSize(1);           // taille standard (5×8 pixels par caractère)
  display.setTextColor(WHITE);
  
  // Position : 
  //   x = 3 (bordure) + 1 (marge) = 4
  //   y = 64 - 3 (bordure) - 1 (marge) - 8 (hauteur texte) = 52
  display.setCursor(4, 52);
  
  display.print(F("Beepr v0.1"));
}

void showSplashScreen() {
  unsigned long startTime = millis();
  uint8_t frame = 0;

  const unsigned long MIN_SPLASH_TIME = 5000;  // 5 secondes minimum
  // ou 3000 si tu préfères 3 secondes

  bool wifiConnected = false;

  // ───────────────────────────────────────────────────────────────
  // Boucle principale du splash : on reste dedans tant que
  //   - on n'a pas atteint le temps minimum
  //   - OU que le WiFi n'est pas connecté
  // ───────────────────────────────────────────────────────────────
  while (true) {
    unsigned long elapsed = millis() - startTime;

    // Condition de sortie : temps minimum écoulé ET WiFi connecté
    if (elapsed >= MIN_SPLASH_TIME && WiFi.status() == WL_CONNECTED) {
      break;
    }

    display.clearDisplay();

    // Logo abeille centrale
    display.drawBitmap(0, 0, logo4, 128, 64, WHITE);

    // Spinner en bas
    int y = 56;
    display.drawBitmap(60, y, loading_frames[frame], 8, 8, WHITE);

    // Option : texte d'état (très utile pour le debug et l'utilisateur)
    display.setTextSize(1);
    display.setTextColor(WHITE);

    if (WiFi.status() == WL_CONNECTED) {
      continue;
    } else {
      display.setCursor(4, 48);
      display.print(F("Connecting to Wi-Fi..."));
    }

    display.display();

    frame = (frame + 1) % 4;

    // Délai d'animation (~5-6 fps)
    delay(180);
  }

  // ─── Splash terminé ───────────────────────────────────────────────
  splashShown = true;

  // On passe directement à logo5 (ou au logo que tu veux par défaut)
  currentLogo = 4;
  drawLogoAt(currentLogo, 0);
}


void setup() {
  pinMode(BUTTON_NEXT, INPUT_PULLUP);
  pinMode(BUTTON_PREV, INPUT_PULLUP);
  pinMode(BUTTON_LED, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Écran le plus tôt possible
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    for(;;);   // blocage si écran HS
  }

  display.clearDisplay();
  display.display();

  // On lance le WiFi tout de suite (mais sans attendre ici)
  WiFi.begin(ssid, password);

  // Serial (optionnel, après l'écran)
  Serial.begin(115200);

  // Le splash screen gère TOUT : animation + attente WiFi + temps minimum
  showSplashScreen();
}


void loop() {
  if (!splashShown) return;

  bool rNext = digitalRead(BUTTON_NEXT);
  bool rPrev = digitalRead(BUTTON_PREV);
  bool rUp   = digitalRead(BUTTON_UP);
  bool rDown = digitalRead(BUTTON_DOWN);
  bool rLed  = digitalRead(BUTTON_LED);

  // ─── On entre / sort du mode menu ───────────────────────────────
  if (currentLogo == 3) {
    inMainMenu = true;   // on est sur la page principale
  } else {
    inMainMenu = false;
  }

  // ─── Navigation menu (seulement en mode menu) ───────────────────
  if (inMainMenu && !animating) {

    // Monter
    if (lastUp == HIGH && rUp == LOW) {
      menuSelected--;
      if (menuSelected < 0) menuSelected = MENU_ITEMS_COUNT - 1;
      drawLogoAt(currentLogo, 0);
      delay(60);
    }

    // Descendre
    if (lastDown == HIGH && rDown == LOW) {
      menuSelected++;
      if (menuSelected >= MENU_ITEMS_COUNT) menuSelected = 0;
      drawLogoAt(currentLogo, 0);
      delay(60);
    }

    // Valider avec bouton LED
    if (lastLedBtn == HIGH && rLed == LOW) {
      // Envoi Discord avec le message sélectionné
      String message = String("Urgent appel : ") + menuItems[menuSelected];
      
      bool success = sendDiscordMessage(message);  // ← ta fonction adaptée

      // Feedback visuel
      display.clearDisplay();
      display.drawBitmap(0, 0, logo4, 128, 64, WHITE);
      display.setTextSize(2);
      display.setCursor(10, 20);
      display.print(F("SENT"));
      display.setTextSize(1);
      display.setCursor(8, 45);
      if (success) {
        display.print(F("Message envoye"));
      } else {
        display.print(F("Echec envoi"));
      }
      display.display();
      delay(1400);

      // Retour menu
      drawLogoAt(currentLogo, 0);
    }
  }
  else {
    // ─── Comportement normal des boutons NEXT/PREV ──────────────────
    if (lastNext == HIGH && rNext == LOW && !animating) {
      startSlideAnimation(+1);
      delay(50);
    }
    if (lastPrev == HIGH && rPrev == LOW && !animating) {
      startSlideAnimation(-1);
      delay(50);
    }
  }

  // Animation slide si en cours
  if (animating) {
    updateSlideAnimation();
    delay(FRAME_DELAY_MS);
  }

  // LED physique
  digitalWrite(LED_PIN, rLed == LOW ? HIGH : LOW);

  // Sauvegarde états
  lastNext   = rNext;
  lastPrev   = rPrev;
  lastUp     = rUp;
  lastDown   = rDown;
  lastLedBtn = rLed;
}