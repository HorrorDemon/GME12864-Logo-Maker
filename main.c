#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#define GRAY "\033[90m"
#define ORANGE "\033[38;5;208m"
#define PINK "\033[38;5;213m"
#define TEAL "\033[38;5;30m"
#define LIME "\033[38;5;118m"
#define GOLD "\033[38;5;220m"

#define BMP_PATH "logo.bmp"
#define ARDUINO_EXPORT_PATH "arduino_logo.h"

#define BMP_HEADER 54

#define BMP_WIDTH 128
#define BMP_HEIGHT 64
#define BMP_IMAGE_SIZE ((BMP_WIDTH * BMP_HEIGHT) / 8)

bool inMiddle = false;
bool border = true;
bool watermark = false;

const char* patternNames[] = {"Blank", "Heart", "Square", "Bee"};
const int patternAmount = 4;

const int heart[7][7] = {
    {0,1,1,0,1,1,0},
    {1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1},
    {0,1,1,1,1,1,0},
    {0,0,1,1,1,0,0},
    {0,0,0,1,0,0,0},
    {0,0,0,0,0,0,0}
};

const int square[5][5] = {
    {1,1,1,1,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,1,1,1,1}
};

const int bee[20][24] = {
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

typedef struct {
    char c;
    unsigned char data[3]; // 3 colonnes, 5 bits utiles
} Font3x5;

Font3x5 font3x5[] = {
    {'B', {0b11111, 0b10101, 0b01010}},
    {'e', {0b01110, 0b10110, 0b01100}},
    {'p', {0b11110, 0b00101, 0b00010}},
    {'r', {0b11110, 0b00100, 0b00100}},
    {'v', {0b00110, 0b11000, 0b00110}},
    {'0', {0b01110, 0b10001, 0b01110}},
    {'1', {0b00000, 0b11111, 0b00000}},
    {'.', {0b00000, 0b10000, 0b00000}},
    {' ', {0b00000, 0b00000, 0b00000}},
};

#define FONT3_COUNT (sizeof(font3x5)/sizeof(Font3x5))

typedef struct
{
    int width;
    int height;
    const int* pattern;
} Pattern;

typedef struct
{
    unsigned char signature[2];
    int fileSize;
    int reserved;   
    int dataOffset;
    int headerSize;
    int imageWidth;
    int imageHeight;
    short planes;
    short bpp;
    int compression;
    int dataSize;
    int hResolution;
    int vResolution;
    int paletteNbColors;
    int importantColors;
} BmpHeader;

Pattern patterns[] = {
    {7, 7, &heart[0][0]},
    {5, 5, &square[0][0]},
    {24, 20, &bee[0][0]}
};

int selectPattern();
void writeBmp(char* bmpPath, int patternId);
void readBmp(char * bmpPath);

int main() 
{

    system("clear");

    int patternSelected = selectPattern();

    writeBmp(BMP_PATH, patternSelected);

    readBmp(BMP_PATH);

    printf(YELLOW "[DEBUG]" RESET " Successfully executed the code.\n");
    return 1;

}

int selectPattern()
{

    printf(GREEN "[INFO]" RESET " Displaying all the available patterns to draw.\n");
    printf(MAGENTA "+----------- Available Patterns -----------+\n" RESET);
    printf(MAGENTA "|" RESET " Blank   :   " CYAN "0\n" RESET);
    printf(MAGENTA "|" RESET " Heart   :   " CYAN "1\n" RESET);
    printf(MAGENTA "|" RESET " Square   :   " CYAN "2\n" RESET);
    printf(MAGENTA "|" RESET " Bee   :   " CYAN "3\n" RESET);
    printf(MAGENTA "+------------------------------------------+\n" RESET);
    printf(TEAL "[INPUT]" RESET " Enter the desired pattern to draw.\n   >> ");
    
    int selectedOption;
    scanf("%d", &selectedOption);

    if (selectedOption >= 0 && selectedOption < patternAmount)
    {
        printf(GREEN "[INFO]" RESET " Selected pattern [" BLUE "%s" RESET "].\n", patternNames[selectedOption]);
        return selectedOption;
    }
    else
    {
        printf(RED "[ERROR]" RESET " Entry format invalid. Selected Blank Pattern.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return 0;
    }
    
}

void drawPattern(unsigned char* pixels, Pattern pattern, int startX, int startY)
{

    for (int y = 0; y < pattern.height; y++)
    {
        for (int x = 0; x < pattern.width; x++)
        {
            int value = pattern.pattern[y * pattern.width + x];

            if (value)
            {
                int px = startX + x;
                int py = startY + y;

                if (px < 0 || px >= BMP_WIDTH || py < 0 || py >= BMP_HEIGHT)
                {
                    continue;
                }

                int byteIndex = (BMP_HEIGHT - 1 - py) * (BMP_WIDTH / 8) + (px / 8);
                int bit = 7 - (px % 8);

                pixels[byteIndex] |= (1 << bit);
            }
        }
    }

}

void drawFancyBorder(unsigned char* pixels)
{

    for (int y = 0; y < BMP_HEIGHT; y++)
    {
        for (int x = 0; x < BMP_WIDTH; x++)
        {
            bool isOuter = x == 0 || x == BMP_WIDTH - 1 || y == 0 || y == BMP_HEIGHT - 1;
            bool isInner = x == 2|| x == BMP_WIDTH - 3 || y == 2 || y == BMP_HEIGHT - 3;

            bool isCorner = (x < 6 && y < 6) || (x > BMP_WIDTH - 7 && y < 6) || (x < 6 && y > BMP_HEIGHT - 7) || (x > BMP_WIDTH - 7 && y > BMP_HEIGHT - 7);

            if (isOuter || isInner || isCorner)
            {
                int byteIndex = (BMP_HEIGHT - 1 - y) * (BMP_WIDTH / 8) + (x / 8);
                int bit = 7 - (x % 8);
                pixels[byteIndex] |= (1 << bit);
            }
        }
    }

}

void drawTextSmall(unsigned char* pixels, const char* text, int startX, int startY)
{
    int cursorX = startX;

    while (*text)
    {
        for (int i = 0; i < FONT3_COUNT; i++)
        {
            if (font3x5[i].c == *text)
            {
                for (int col = 0; col < 3; col++)
                {
                    for (int row = 0; row < 5; row++)
                    {
                        if (font3x5[i].data[col] & (1 << row))
                        {
                            int px = cursorX + col;
                            int py = startY + row;

                            if (px < 0 || px >= BMP_WIDTH || py < 0 || py >= BMP_HEIGHT)
                                continue;

                            int byteIndex = (BMP_HEIGHT - 1 - py) * (BMP_WIDTH / 8) + (px / 8);
                            int bit = 7 - (px % 8);
                            pixels[byteIndex] |= (1 << bit);
                        }
                    }
                }
                break;
            }
        }

        cursorX += 4; // 3px + 1 espace
        text++;
    }
}

void writeBmp(char* bmpPath, int patternId)
{

    FILE* bmp = fopen(BMP_PATH, "wb");
    if (!bmp)
    {
        printf(RED "[ERROR]" RESET " Couldn't create file [" BLUE "%s" RESET "].\n", BMP_PATH);
        return;
    }

    BmpHeader bmpHdr;

    unsigned char palette[8] = {0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00};

    unsigned char pixels[BMP_IMAGE_SIZE];
    memset(pixels, 0x00, BMP_IMAGE_SIZE);

    if (border)
    {
        drawFancyBorder(pixels);
    }

    if (patternId > 0) 
    {
        Pattern pattern = patterns[patternId - 1];

        int startX;
        int startY;

        if (inMiddle)
        {
            startX = BMP_WIDTH / 2 - pattern.width / 2;
            startY = BMP_HEIGHT / 2 - pattern.height / 2;
        }
        else
        {
            startX = BMP_WIDTH - pattern.width - 4;
            startY = BMP_HEIGHT - pattern.height - 4;
        }

        drawPattern(pixels, pattern, startX, startY);

    }
    
    if (watermark)
    {
        int textX = 2;
        int textY = BMP_HEIGHT - 7;
        drawTextSmall(pixels, "Beepr v0.1", textX, textY);
    }

    bmpHdr.signature[0] = 'B';
    bmpHdr.signature[1] = 'M';
    bmpHdr.fileSize = BMP_HEADER + BMP_IMAGE_SIZE + sizeof(palette);
    bmpHdr.reserved = 0;
    bmpHdr.dataOffset = BMP_HEADER + sizeof(palette);
    bmpHdr.headerSize = 40;
    bmpHdr.imageWidth = BMP_WIDTH;
    bmpHdr.imageHeight = BMP_HEIGHT;
    bmpHdr.planes = 1;
    bmpHdr.bpp = 1;
    bmpHdr.compression = 0;
    bmpHdr.dataSize = BMP_IMAGE_SIZE;
    bmpHdr.hResolution = 0;
    bmpHdr.vResolution = 0;
    bmpHdr.paletteNbColors = 2;
    bmpHdr.importantColors = 0;

    fwrite(bmpHdr.signature, sizeof(char), 2, bmp);
    fwrite(&bmpHdr.fileSize, sizeof(int), 1, bmp);
    fwrite(&bmpHdr.reserved, sizeof(int), 1, bmp);
    fwrite(&bmpHdr.dataOffset, sizeof(int), 1, bmp);
    fwrite(&bmpHdr.headerSize, sizeof(int), 1, bmp);
    fwrite(&bmpHdr.imageWidth, sizeof(int), 1, bmp);
    fwrite(&bmpHdr.imageHeight, sizeof(int), 1, bmp);
    fwrite(&bmpHdr.planes, sizeof(short), 1, bmp);
    fwrite(&bmpHdr.bpp, sizeof(short), 1, bmp);
    fwrite(&bmpHdr.compression, sizeof(int), 1, bmp);
    fwrite(&bmpHdr.dataSize, sizeof(int), 1, bmp);
    fwrite(&bmpHdr.hResolution, sizeof(int), 1, bmp);
    fwrite(&bmpHdr.vResolution, sizeof(int), 1, bmp);
    fwrite(&bmpHdr.paletteNbColors, sizeof(int), 1, bmp);
    fwrite(&bmpHdr.importantColors, sizeof(int), 1, bmp);
    fwrite(palette, sizeof(unsigned char), 8, bmp);
    fwrite(pixels, sizeof(unsigned char), BMP_IMAGE_SIZE, bmp);
    
    fclose(bmp);

    printf(GREEN "[INFO]" RESET " Successfully created BMP logo [" BLUE "%s" RESET "].\n", bmpPath);

    return;

}

void readBmp(char* bmpPath)
{
    
    FILE* bmp = fopen(bmpPath, "rb");
    if (!bmp)
    {
        printf(RED "[ERROR]" RESET " Couldn't open file [" BLUE "%s" RESET "].\n", bmpPath);
        return;
    }

    BmpHeader bmpHdr;

    fread(&bmpHdr.signature, sizeof(char), 2, bmp);
    fread(&bmpHdr.fileSize, sizeof(int), 1, bmp);
    fread(&bmpHdr.reserved, sizeof(int), 1, bmp);
    fread(&bmpHdr.dataOffset, sizeof(int), 1, bmp);
    fread(&bmpHdr.headerSize, sizeof(int), 1, bmp);
    fread(&bmpHdr.imageWidth, sizeof(int), 1, bmp);
    fread(&bmpHdr.imageHeight, sizeof(int), 1, bmp);
    fread(&bmpHdr.planes, sizeof(short), 1, bmp);
    fread(&bmpHdr.bpp, sizeof(short), 1, bmp);
    fread(&bmpHdr.compression, sizeof(int), 1, bmp);
    fread(&bmpHdr.dataSize, sizeof(int), 1, bmp);
    fread(&bmpHdr.hResolution, sizeof(int), 1, bmp);
    fread(&bmpHdr.vResolution, sizeof(int), 1, bmp);
    fread(&bmpHdr.paletteNbColors, sizeof(int), 1, bmp);
    fread(&bmpHdr.importantColors, sizeof(int), 1, bmp);

    printf(TEAL "[INPUT]" RESET " Display BMP file header ? ( yes | no )\n   >> ");
    char input_dfh[8];
    scanf("%7s", input_dfh);
    if (strcmp(input_dfh, "yes") == 0 || strcmp(input_dfh, "y") == 0)
    {
        printf(GREEN "[INFO]" RESET " Displaying BMP file header for [" BLUE "%s" RESET "].\n", bmpPath);

        printf(MAGENTA "+-------------------- BMP HEADER --------------------+\n" RESET);
        printf(MAGENTA "|" RESET " Signature         :" CYAN " [%c%c]\n" RESET, bmpHdr.signature[0], bmpHdr.signature[1]);
        printf(MAGENTA "|" RESET " File Size         :" CYAN " [%d]\n" RESET, bmpHdr.fileSize);
        printf(MAGENTA "|" RESET " Reserved          :" CYAN " [%d]\n" RESET, bmpHdr.reserved);
        printf(MAGENTA "|" RESET " Data Offset       :" CYAN " [%d]\n" RESET, bmpHdr.dataOffset);
        printf(MAGENTA "|" RESET " Header Size       :" CYAN " [%d]\n" RESET, bmpHdr.headerSize);
        printf(MAGENTA "|" RESET " Image Width       :" CYAN " [%d]\n" RESET, bmpHdr.imageWidth);
        printf(MAGENTA "|" RESET " Image Height      :" CYAN " [%d]\n" RESET, bmpHdr.imageHeight);
        printf(MAGENTA "|" RESET " Planes            :" CYAN " [%hd]\n" RESET, bmpHdr.planes);
        printf(MAGENTA "|" RESET " Bits per Pixel    :" CYAN " [%hd]\n" RESET, bmpHdr.bpp);
        printf(MAGENTA "|" RESET " Compression       :" CYAN " [%d]\n" RESET, bmpHdr.compression);
        printf(MAGENTA "|" RESET " Data Size         :" CYAN " [%d]\n" RESET, bmpHdr.dataSize);
        printf(MAGENTA "|" RESET " H Resolution      :" CYAN " [%d]\n" RESET, bmpHdr.hResolution);
        printf(MAGENTA "|" RESET " V Resolution      :" CYAN " [%d]\n" RESET, bmpHdr.vResolution);
        printf(MAGENTA "|" RESET " Palette Colors    :" CYAN " [%d]\n" RESET, bmpHdr.paletteNbColors);
        printf(MAGENTA "|" RESET " Important Colors  :" CYAN " [%d]\n" RESET, bmpHdr.importantColors);
        printf(MAGENTA "+----------------------------------------------------+\n" RESET);
    }
    else if (strcmp(input_dfh, "no") == 0 || strcmp(input_dfh, "n") == 0)
    {
        printf(GREEN "[INFO]" RESET " Skipping the display of BMP file header.\n");
    }
    else
    {
        printf(RED "[ERROR]" RESET " Entry format invalid. Skipping the display of BMP file header.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    printf(TEAL "[INPUT]" RESET " Create output file for arduino ? (yes | no)\n   >> ");
    char input_cof[8];
    scanf("%7s", input_cof);
    if (strcmp(input_cof, "yes") == 0 || strcmp(input_cof, "y") == 0)
    {
        int widthBytes = (bmpHdr.imageWidth + 7) / 8;
        int rowSize = ((widthBytes + 3) / 4) * 4;
        unsigned char* rowData = malloc(rowSize);
        if (!rowData) 
        {
            printf(RED "[ERROR]" RESET " Memory allocation failed.\n");
            fclose(bmp);
            return;
        }

        FILE* ardExprt = fopen(ARDUINO_EXPORT_PATH, "w");
        if (!ardExprt) {
            printf(RED "[ERROR]" RESET " Couldn't create file [" BLUE "%s" RESET "].\n", ARDUINO_EXPORT_PATH);
            free(rowData);
            fclose(bmp);
            return;
        }

        fprintf(ardExprt, "const unsigned char logo[] PROGMEM = {\n");

        fseek(bmp, bmpHdr.dataOffset, SEEK_SET);

        for (int y = bmpHdr.imageHeight - 1; y >= 0; y--) 
        {
            fseek(bmp, bmpHdr.dataOffset + y * rowSize, SEEK_SET);
            fread(rowData, 1, rowSize, bmp);

            for (int i = 0; i < widthBytes; i++)
            {
                fprintf(ardExprt, "0x%02X", rowData[i]);
                if (!(y == 0 && i == widthBytes -1))
                {
                    fprintf(ardExprt, ",");
                }
            }

            fprintf(ardExprt, "\n");
        }

        fprintf(ardExprt, "};\n");

        printf(GREEN "[INFO]" RESET " Arduino export file generated [" BLUE "%s" RESET "].\n", ARDUINO_EXPORT_PATH);

        free(rowData);
    }
    else if (strcmp(input_cof, "no") == 0 || strcmp(input_cof, "n") == 0)
    {
        printf(GREEN "[INFO]" RESET " Skipping creation of output file.\n");
    }
    else
    {
        printf(RED "[ERROR]" RESET " Entry format invalid. Skipping creation of output file.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    fclose(bmp);

    return;

}

// S'occuper juste de la création des images ici, comme si un tel faisait ces images
// Pour la gestion d'un watermark par exemple, on va le dessiner ici, mais pas le placer ici, ca va etre dans le code arduino en fonction des settings genre upRight, downLeft