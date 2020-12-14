//
// Created by erwan on 05/12/2020.
//

#ifndef SANITIZE_C_BMP_STRUCT_H
#define SANITIZE_C_BMP_STRUCT_H
typedef unsigned int Uint;


typedef struct PIX{
    unsigned char r, g, b;
}PIX;


typedef struct FileHeader{
    Uint id;                // Offset: 0,  Bytes: 2
    Uint file_size;         // Offset: 2,  Bytes: 4
    Uint arr_offset;        // Offset: 10,  Bytes: 4
}FileHeader;



typedef struct ImageHeader{
    Uint header_size;       // Offset: 14,  Bytes: 4
    Uint width;             // Offset: 18,  Bytes: 4
    Uint height;            // Offset: 22,  Bytes: 4
    //Uint planes;            // Offset: 26,  Bytes: 2
    Uint bits_pp;           // Offset: 28,  Bytes: 2
    //Uint compress;          // Offset: 30,  Bytes: 4
    Uint img_size;          // Offset: 34,  Bytes: 4
    /*Uint res_h;             // Offset: 38,  Bytes: 4
    Uint res_v;             // Offset: 42,  Bytes: 4
    Uint pal_colors_num;    // Offset: 46,  Bytes: 4
    Uint important_colors;*/  // Offset: 50,  Bytes: 4
    unsigned char *completeArray;
}ImageHeader;

typedef struct BMP{
    ImageHeader imageHeader;
    FileHeader fileHeader;
    Uint palette;
    unsigned char* pixels;
}BMP;
#endif //SANITIZE_C_BMP_STRUCT_H
