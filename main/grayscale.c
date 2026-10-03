#include "grayscale.h"

void grayscale(BMPImage *image)
{
    int total = image->width * image->height;

    for (int i = 0; i < total; i++) {

        int position = i * 3;

        unsigned char R = image->pixels[position];
        unsigned char G = image->pixels[position + 1];
        unsigned char B = image->pixels[position + 2];

        unsigned char gray = 0.299 * R +
                             0.587 * G +
                             0.114 * B;

        image->pixels[position] = gray;
        image->pixels[position + 1] = gray;
        image->pixels[position + 2] = gray;
    }
}