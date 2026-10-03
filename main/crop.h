#ifndef CROP_H
#define CROP_H

#include "openbmp.h"

void crop(BMPImage *image,
          int startX,
          int startY,
          int cropWidth,
          int cropHeight);

#endif