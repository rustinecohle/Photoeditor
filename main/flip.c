#include "flip.h"

void horizontalFlip(BMPImage *image)
{
    int width = image->width;
    int height = image->height;

    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width / 2; x++) {

            int left = (y * width + x) * 3;
            int right = (y * width + (width - 1 - x)) * 3;

            for (int c = 0; c < 3; c++) {

                unsigned char temp =
                    image->pixels[left + c];

                image->pixels[left + c] =
                    image->pixels[right + c];

                image->pixels[right + c] = temp;
            }
        }
    }
}


void verticalFlip(BMPImage *image)
{
    int width = image->width;
    int height = image->height;

    for (int y = 0; y < height / 2; y++) {

        for (int x = 0; x < width; x++) {

            int top = (y * width + x) * 3;
            int bottom = ((height - 1 - y) * width + x) * 3;

            for (int c = 0; c < 3; c++) {

                unsigned char temp =
                    image->pixels[top + c];

                image->pixels[top + c] =
                    image->pixels[bottom + c];

                image->pixels[bottom + c] = temp;
            }
        }
    }
}
