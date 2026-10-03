#include "invert.h"

void invert(BMPImage *image)
{
    int total = image->width * image->height;

    for (int i = 0; i < total; i++) {

        int position = i * 3;

        image->pixels[position] =
            255 - image->pixels[position];

        image->pixels[position + 1] =
            255 - image->pixels[position + 1];

        image->pixels[position + 2] =
            255 - image->pixels[position + 2];
    }
}