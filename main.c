#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#define BMP_PATH "logo.bmp"

#define BMP_HEADER 54

#define BMP_WIDTH 128
#define BMP_HEIGHT 64
#define BMP_IMAGE_SIZE ((BMP_WIDTH * BMP_HEIGHT) / 8)

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

void readBmp(char * bmpPath);

int main() 
{

    system("clear");

    FILE* bmp = fopen(BMP_PATH, "wb");
    if (!bmp)
    {
        printf(RED "[ERROR]" RESET " Couldn't create file [" BLUE "%s" RESET "].\n", BMP_PATH);
        return -1;
    }

    BmpHeader bmpHdr;

    unsigned char palette[8] = {0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00};

    unsigned char pixels[BMP_IMAGE_SIZE];
    memset(pixels, 0x00, BMP_IMAGE_SIZE);

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

    readBmp(BMP_PATH);

    printf(YELLOW "[DEBUG]" RESET " Successfully executed the code.\n");
    return 1;

}

void readBmp(char * bmpPath)
{
    
    FILE* bmp = fopen(bmpPath, "rb");
    if (!bmp)
    {
        printf(RED "[ERROR]" RESET " Couldn't open file [" BLUE "%s" RESET "].\n", bmpPath);
        return;
    }

    printf(GREEN "[INFO]" RESET " Displaying BMP file header for [" BLUE "%s" RESET "].\n", bmpPath);

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

    return;

}