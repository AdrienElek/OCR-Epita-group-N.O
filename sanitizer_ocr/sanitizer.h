//
// Created by erwan on 01/12/2020.
//

#ifndef SANITIZER_C_SANITIZER_H
#define SANITIZER_C_SANITIZER_H
#include "bmp_struct.h"
void Greyscale(BMP* bmp, BMP* gs_bmp);
void Denoising(BMP* bmp, BMP* dn_bmp);
void Binary(BMP* bmp, BMP* bn_bmp);
void Straighten(BMP* bmp, BMP* st_bmp);
void Rotate(BMP* bmp, BMP* rotate_bmp, int angle);




#endif //SANITIZER_C_SANITIZER_H

