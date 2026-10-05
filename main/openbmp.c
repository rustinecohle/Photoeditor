#include <stdio.h>
#include <stdlib.h>

#include "openbmp.h"


BMPImage *openBMP(const char *filename)
{
    FILE *file;

    file = fopen(filename, "rb");

    if (file == NULL) {
        printf("Cannot open file.\n");
        return NULL;
    }

    BMPImage *image = malloc(sizeof(BMPImage));

    if (image == NULL) {
        fclose(file);
        return NULL;
    }

    fread(
        &image->fileheader,
        sizeof(BMPFileHeader),
        1,
        file
    );

    fread(
        &image->infoheader,
        sizeof(BMPInfoHeader),
        1,
        file
    );


    if (image->fileheader.type != 0x4D42) {

        printf("This is not a BMP file.\n");

        fclose(file);
        free(image);

        return NULL;
    }


    if (image->infoheader.bitsperpixel != 24) {

        printf("Only 24-bit BMP images are supported.\n");

        fclose(file);
        free(image);

        return NULL;
    }


    if (image->infoheader.compression != 0) {

        printf("Compressed BMP files are not supported.\n");

        fclose(file);
        free(image);

        return NULL;
    }


    image->width =
        image->infoheader.width;

    image->height =
        image->infoheader.height;


    int width = image->width;
    int height = image->height;

    int padding =
        (4 - (width * 3) % 4) % 4;


    image->pixels =
        malloc(width * height * 3);


    if (image->pixels == NULL) {

        fclose(file);
        free(image);

        return NULL;
    }


    fseek(
        file,
        image->fileheader.dataoffset,
        SEEK_SET
    );


    unsigned char pixel[3];




    for (int y = 0; y < height; y++) {

        int memoryY =
            height - 1 - y;

        for (int x = 0; x < width; x++) {

            fread(pixel, 3, 1, file);

            int position =
                (memoryY * width + x) * 3;


            /*
               BMP = BGR

               Memory = RGB
            */

            image->pixels[position] =
                pixel[2];

            image->pixels[position + 1] =
                pixel[1];

            image->pixels[position + 2] =
                pixel[0];
        }

        fseek(file, padding, SEEK_CUR);
    }


    fclose(file);

    printf("BMP opened successfully.\n");
    printf("Width: %d\n", width);
    printf("Height: %d\n", height);

    return image;
}



int saveBMP(const char *filename, BMPImage *image)
{
    if (image == NULL)
        return 0;


    FILE *file =
        fopen(filename, "wb");


    if (file == NULL) {

        printf("Cannot create BMP file.\n");

        return 0;
    }


    int width = image->width;
    int height = image->height;


    int padding =
        (4 - (width * 3) % 4) % 4;


    int rowSize =
        width * 3 + padding;


    int imageSize =
        rowSize * height;


    int fileSize =
        image->fileheader.dataoffset +
        imageSize;




    image->fileheader.filesize =
        fileSize;

    image->infoheader.width =
        width;

    image->infoheader.height =
        height;


    fwrite(
        &image->fileheader,
        sizeof(BMPFileHeader),
        1,
        file
    );


    fwrite(
        &image->infoheader,
        sizeof(BMPInfoHeader),
        1,
        file
    );


    unsigned char zero[3] =
        {0, 0, 0};




    for (int y = height - 1; y >= 0; y--) {

        for (int x = 0; x < width; x++) {

            int position =
                (y * width + x) * 3;


            unsigned char pixel[3];


            pixel[0] =
                image->pixels[position + 2];

            pixel[1] =
                image->pixels[position + 1];

            pixel[2] =
                image->pixels[position];


            fwrite(
                pixel,
                3,
                1,
                file
            );
        }


        fwrite(
            zero,
            padding,
            1,
            file
        );
    }


    fclose(file);

    printf("BMP saved successfully.\n");

    return 1;
}



void freeBMP(BMPImage *image)
{
    if (image != NULL) {

        free(image->pixels);

        free(image);
    }
}
