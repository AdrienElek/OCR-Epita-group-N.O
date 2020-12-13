//
// Created by erwan on 03/12/2020.
//


#ifndef SANITIZE_C_LOADER_H
#define SANITIZE_C_LOADER_H
#include "bmp_struct.h"

void print_info(BMP);
void CopyArray(unsigned char **src, unsigned char **dst, size_t length);
void CopyBMP(BMP* src, BMP* dst);
void CopyBMPWithEmptyPixels(BMP* src, BMP* dst);

PIX GetPixel(BMP* bmp, int x, int y);
void SetPixel(BMP* bmp, int x, int y, PIX pix);

void GetBMP(char*, BMP* dst);
void SaveBMP(BMP*, char*);


#endif //SANITIZE_C_LOADER_H
