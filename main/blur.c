#include"blur.h"
#include<stdlib.h>
void blur (BMPImage *image){

    int width=image->width;
    int height=image->height;
    unsigned char *blurred=malloc(width*height*3);
    if(blurred==NULL){
        return;
    }
    for(int y=0;y<height; y++){
        for(int x=0;x<width; x++){
            int sumB=0;
            int sumG=0;
            int sumR=0;
            int count=0;
            for(int dy=-1 ; dy<2; dy++){
                for (int dx=-1; dx<2; dx++){
                    int neighbourX= x+dx;
                    int neighbourY= y+dy;
                    if (neighbourX<0 || neighbourX>width)
                        continue;
                    if (neighbourY<0 || neighbourY>width)
                        continue;
                    int position=(neighbourY*width+neighbourX)*3;
                    sumB+=image->pixels[position];
                    sumG+=image->pixels[position+1];
                    sumR+=image->pixels[position+2];
                    count++;
                }
            }
            
            int destination = (y * width + x) * 3;

            blurred[destination] = sumB / count;
            blurred[destination + 1] = sumG / count;
            blurred[destination + 2] = sumR / count;
        }
    }
    free(image->pixels);
    image->pixels=blurred;



}