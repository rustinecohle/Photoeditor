#include "rotate.h"
#include <stdlib.h>
void rotate(BMPImage *image)
{
    int width = image->width;
    int height = image->height;
    int newWidth = height;
    int newHeight = width;
    unsigned char *rotated = malloc(newHeight * newWidth * 3);
    if (rotated == NULL)
        return;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int newX = height - 1 - y;
            int newY = x;
            // this is the actual roration formula to remember
            int source = (y * width + x) * 3;
            int destination = (newY * newWidth + newX) * 3;

            rotated[destination] = image->pixels[source];
            rotated[destination + 1] = image->pixels[source + 1];
            rotated[destination + 2] = image->pixels[source + 2];
        }
    }
    free(image->pixels);
    image->pixels = rotated;
    image->width = newWidth;
    image->height = newHeight;

    image->infoheader.width = newWidth;
    image->infoheader.height = newHeight;
}