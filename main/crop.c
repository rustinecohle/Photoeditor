#include "crop.h"
#include <stdlib.h>
void crop(BMPImage *image, int startX, int startY, int cropWidth, int cropHeight)
{
    if (startX < 0 || startY < 0)
        return;
    if (cropWidth <= 0 || cropHeight <= 0)
        return;
    if (startX + cropWidth > image->width)
        return;
    if (startY + cropHeight > image->height)
        return;
    unsigned char *cropped;
    cropped = malloc(cropWidth * cropHeight * 3);
    if (cropped == NULL)
        return;
    for (int y=0; y<cropHeight; y++){
        for(int x=0; x<cropWidth; x++){
            int sourceX = startX + x;
            int sourceY = startY + y;

            int source =
                (sourceY * image->width + sourceX) * 3;

            int destination =
                (y * cropWidth + x) * 3;

            cropped[destination] =
                image->pixels[source];

            cropped[destination + 1] =
                image->pixels[source + 1];

            cropped[destination + 2] =
                image->pixels[source + 2];
        }
    }

 free(image->pixels);

    image->pixels = cropped;

    image->width = cropWidth;
    image->height = cropHeight;

    image->infoheader.width = cropWidth;
    image->infoheader.height = cropHeight;
}
