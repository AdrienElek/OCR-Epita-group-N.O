#include <stdio.h>
#include "loader.h"
#include "sanitizer.h"



int main(int argc, char *argv[]) {
    BMP bmp;
    GetBMP(*(argv+1), &bmp);
    SaveBMP(&bmp, "raw.bmp");

    BMP st_bmp;
    Straighten(&bmp, &st_bmp);


    /*BMP gs_bmp;
    Greyscale(&bmp, &gs_bmp);
    SaveBMP(&gs_bmp, "gs.bmp");


    BMP dn_bmp;
    Denoising(&gs_bmp, &dn_bmp);
    SaveBMP(&dn_bmp, "dn.bmp");

    BMP bn_bmp;
    Binary(&gs_bmp, &bn_bmp);
    SaveBMP(&bn_bmp, "bn.bmp");*/


}
