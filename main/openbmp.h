#ifndef OPENBMP_H
#define OPENBMP_H
#include<stdio.h>
#pragma pack (push,1)
typedef struct {
    unsigned short type;
    unsigned int filesize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int dataoffset;

} BMPFileHeader;

typedef struct {
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitperpixel;
    unsigned int compression;
    unsigned int xpixelsPerM;
    unsigned int ypixelsPerM;
    unsigned int colorUsed;
    unsigned int importantColors;



} BMPInfoHeader;

#pragma pack (pop)
typedef struct {
    BMPFileHeader fileheader;
    BMPInfoHeader infoheader;
    unsigned char *pixels;//here pixels uses dynamically allocated memory thats why freeBMP is used to clear the whole data of the heap
    int width;
    int height;




} BMPImage;


 BMPImage *openBMP(const char *filename);
 int saveBMP(const char *filename, BMPImage *image);
 void freeBMP(BMPImage *image);
 #endif