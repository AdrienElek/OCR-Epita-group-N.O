#include "loader.h"
#include "sanitizer.h"



int main(int argc, char *argv[]) {
    BMP bmp;
    GetBMP(*(argv+1), &bmp);
    SaveBMP(&bmp, "raw.bmp");

    BMP gs_bmp;
    Greyscale(&bmp, &gs_bmp);
    SaveBMP(&gs_bmp, "gs.bmp");


    BMP dn_bmp;
    Denoising(&gs_bmp, &dn_bmp);
    SaveBMP(&dn_bmp, "dn.bmp");

    BMP bn_bmp;
    Binary(&dn_bmp, &bn_bmp);
    SaveBMP(&bn_bmp, "bn.bmp");

    BMP st_bmp;
    Straighten(&bn_bmp, &st_bmp);
    SaveBMP(&st_bmp, "st.bmp");

}
