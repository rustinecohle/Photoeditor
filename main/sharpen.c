#include "sharpen.h"
#include <stdlib.h>

void sharpen(BMPImage *image){

    int width = image->width;
    int height = image->height;

    unsigned char *sharpened = malloc(width * height * 3);

    if (sharpened == NULL)
        return;

    int kernel[3][3] = {
        {  0, -1,  0 },
        { -1,  5, -1 },
        {  0, -1,  0 }
    };
    for (int y=0; y < height; y++){
        for(int x=0; x < width; x++){
            int sumB=0;
            int sumG=0;
            int sumR=0;
            for(int ky=-1; ky<2;ky++){
                for(int kx=-1; kx<2; ky++){
                    int neighborX = x + kx;
                    int neighborY = y + ky;

                    if (neighborX < 0)
                        neighborX = 0;

                    if (neighborX >= width)
                        neighborX = width - 1;

                    if (neighborY < 0)
                        neighborY = 0;

                    if (neighborY >= height)
                        neighborY = height - 1;
                    int position=(neighborY*width+neighborX)*3;
                    int weight=kernel[ky + 1][kx + 1];

                    sumB += image->pixels[position] * weight;
                    sumG += image->pixels[position + 1] * weight;
                    sumR += image->pixels[position + 2] * weight;
                }
            
        
        if (sumB < 0) sumB = 0;
            if (sumB > 255) sumB = 255;

            if (sumG < 0) sumG = 0;
            if (sumG > 255) sumG = 255;

            if (sumR < 0) sumR = 0;
            if (sumR > 255) sumR = 255;

            int destination = (y * width + x) * 3;

            sharpened[destination] = sumB;
            sharpened[destination + 1] = sumG;
            sharpened[destination + 2] = sumR;
        }
    }

    free(image->pixels);

    image->pixels = sharpened;
    }


}