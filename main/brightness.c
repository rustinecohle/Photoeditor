#include "brightness.h"

void brightness(BMPImage *image, int level)
{
    int total = image->width * image->height;

    for (int i = 0; i < total; i++) {

        int position = i * 3;

        int R = image->pixels[position] + level;
        int G = image->pixels[position + 1] + level;
        int B = image->pixels[position + 2] + level;

        if (B > 255)
            B = 255;

        if (G > 255)
            G = 255;

        if (R > 255)
            R = 255;

        if (B < 0)
            B = 0;

        if (G < 0)
            G = 0;

        if (R < 0)
            R = 0;

        image->pixels[position] = R;
        image->pixels[position + 1] = G;
        image->pixels[position + 2] = B;
    }
}