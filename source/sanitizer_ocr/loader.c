#include "stdio.h"
#include <stdlib.h>
#include "bmp_struct.h"


// Copy <src> array to <dst> array both of size <length>
void CopyArray(unsigned char **src, unsigned char **dst, size_t length){
    unsigned char *tsrc = *src;
    unsigned char *tdst = *dst;

    for (size_t i = 0; i < length; i++){
        *(tdst+i) = *(tsrc+i);
    }
}


// x^n
Uint power(int x, int n){
    Uint sum = 1;
    for(int i = 0; i < n; i++){
        sum *= x;
    }
    return sum;
}


/*
 * Convert the <bytes> bytes from [offset, bytes-1] in *tab to int
 * Convert in reverse order i.e : B9 01 -> 01B9 -> 441
 * */
Uint Hex2I(unsigned char **arr, int offset, int bytes){
    Uint sum = 0;
    for(int i = 0; i < bytes; i++){
        if (i == 0)
            sum += (*arr)[offset + i];
        else
            sum += (*arr)[offset+i] * power(16, 2*i);
    }
    return sum;
}


// Fill the <arr> array from a BMP field <field>
// at <offset> position to <offset> + <bytes> - 1 position
void I2Hex(unsigned char **arr, Uint field, int offset, int bytes){
    unsigned char curr;
    for (int i = 0; i < bytes; i++){
        curr = field&255;
        field = field >> 8;

        (*arr)[offset+i] = curr;
    }
}





void CopyBMPWithEmptyPixels(BMP *src, BMP *dst){
    // File Header
    dst->fileHeader.id = src->fileHeader.id;
    dst->fileHeader.file_size = src->fileHeader.file_size;
    dst->fileHeader.arr_offset = src->fileHeader.arr_offset;

    // Image Header
    dst->imageHeader.header_size = src->imageHeader.header_size;
    dst->imageHeader.width = src->imageHeader.width;
    dst->imageHeader.height = src->imageHeader.height;
    //dst->imageHeader.planes = src->imageHeader.planes;
    dst->imageHeader.bits_pp = src->imageHeader.bits_pp;
    //dst->imageHeader.compress = src->imageHeader.compress;
    dst->imageHeader.img_size = src->imageHeader.img_size;
    /*dst->imageHeader.res_h = src->imageHeader.res_h;
    dst->imageHeader.res_v = src->imageHeader.res_v;
    dst->imageHeader.pal_colors_num = src->imageHeader.pal_colors_num;
    dst->imageHeader.important_colors = src->imageHeader.important_colors;*/


    dst->palette = src->palette;

    unsigned char *completeHeader = malloc(src->imageHeader.header_size * sizeof(unsigned char));
    CopyArray(&src->imageHeader.completeArray, &completeHeader, src->imageHeader.header_size);
    dst->imageHeader.completeArray = completeHeader;

    unsigned char *new_pixels = calloc(src->imageHeader.img_size, sizeof(unsigned char));
    dst->pixels = new_pixels;
}



void CopyBMP(BMP *src, BMP *dst){
    // File Header
    dst->fileHeader.id = src->fileHeader.id;
    dst->fileHeader.file_size = src->fileHeader.file_size;
    dst->fileHeader.arr_offset = src->fileHeader.arr_offset;

    // Image Header
    dst->imageHeader.header_size = src->imageHeader.header_size;
    dst->imageHeader.width = src->imageHeader.width;
    dst->imageHeader.height = src->imageHeader.height;
    //dst->imageHeader.planes = src->imageHeader.planes;
    dst->imageHeader.bits_pp = src->imageHeader.bits_pp;
    //dst->imageHeader.compress = src->imageHeader.compress;
    dst->imageHeader.img_size = src->imageHeader.img_size;
    /*dst->imageHeader.res_h = src->imageHeader.res_h;
    dst->imageHeader.res_v = src->imageHeader.res_v;
    dst->imageHeader.pal_colors_num = src->imageHeader.pal_colors_num;
    dst->imageHeader.important_colors = src->imageHeader.important_colors;*/


    dst->palette = src->palette;


    unsigned char *completeHeader = malloc(src->imageHeader.header_size * sizeof(unsigned char));
    CopyArray(&src->imageHeader.completeArray, &completeHeader, src->imageHeader.header_size);
    dst->imageHeader.completeArray = completeHeader;

    unsigned char *new_pixels = malloc(src->imageHeader.img_size * sizeof(unsigned char));
    CopyArray(&(src->pixels), &new_pixels, src->imageHeader.img_size);
    dst->pixels = new_pixels;


}




// May need to pass the array as parameter
// Build the representing array of the bmp file
void BuildFIleArray(BMP *bmp, unsigned char **arr){
    // File header
    I2Hex(arr, bmp->fileHeader.id, 0, 2);
    I2Hex(arr, bmp->fileHeader.file_size, 2, 4);
    I2Hex(arr, 0, 6, 4);           // Reserved field
    I2Hex(arr, bmp->fileHeader.arr_offset, 10, 4);



    //Image Header
    unsigned char *array = *arr;
    for (int i = 0; i < bmp->imageHeader.header_size; i++) {
        *(array+14+i) = *(bmp->imageHeader.completeArray+i);
    }



    // Pixel array
    for (int i = 0; i < bmp->imageHeader.img_size; i++){
        (*arr)[bmp->fileHeader.arr_offset + i] = bmp->pixels[i];
    }
}



PIX GetPixel(BMP* bmp, int x, int y){
    Uint index = bmp->imageHeader.bits_pp/8 * (x + y * bmp->imageHeader.width);
    PIX pix = {
            .r = bmp->pixels[index],
            .g = bmp->pixels[index+1],
            .b = bmp->pixels[index+2],
    };
    return pix;
}


void SetPixel(BMP* bmp, int x, int y, PIX pix){
    Uint index = bmp->imageHeader.bits_pp/8 * (x + y * bmp->imageHeader.width);
    bmp->pixels[index] = pix.r;
    bmp->pixels[index+1] = pix.g;
    bmp->pixels[index+2] = pix.b;

}




/*void print_info(BMP bmp){
    printf("ID OS : %d\n", bmp.fileHeader.id);
    printf("file s: %d\n", bmp.fileHeader.file_size);
    printf("offset: %d\n", bmp.fileHeader.arr_offset);
    printf("\n");
    printf("hi len: %d\n", bmp.imageHeader.header_size);
    printf("width : %d\n", bmp.imageHeader.width);
    printf("height: %d\n", bmp.imageHeader.height);
    //printf("plans : %d\n", bmp.imageHeader.planes);
    printf("bitspp: %d\n", bmp.imageHeader.bits_pp);
    //printf("comp m: %d\n", bmp.imageHeader.compress);
    printf("arrlen: %d\n", bmp.imageHeader.img_size);
    //printf("h res : %d\n", bmp.imageHeader.res_h);
    //printf("v res : %d\n", bmp.imageHeader.res_v);
    //printf("n col : %d\n", bmp.imageHeader.pal_colors_num);
    //printf("hl col: %d\n", bmp.imageHeader.important_colors);
}*/




void SaveBMP(BMP *bmp, char* const filename){
    // Open file
    FILE *fptr = fopen(filename, "wb");

    // Building an array representing file
    unsigned char *arr = malloc(sizeof(unsigned char) * bmp->fileHeader.file_size);
    BuildFIleArray(bmp, &arr);

    // Writing in the <filename> file
    fwrite(arr, sizeof(unsigned char), bmp->fileHeader.file_size, fptr);
    fclose(fptr);
    free(arr);
}




// Load a BMP struct into dst
void GetBMP(char* path, BMP* dst)
{
    // chargement du fichier
    FILE *fp = fopen(path,"rb");

    // détermine la taille du fichier BMP
    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);
    //printf("Fsize: %ld\n",fileSize);

    fseek(fp, 0, SEEK_SET);

    // allocation d'un tableau de la taille du fichier
    unsigned char *BMPArray = malloc(fileSize * sizeof(unsigned char));

    // chargement du fichier bmp en mémoire
    int count = 0;
    int c;
    while((c = fgetc(fp)) != EOF)
        BMPArray[count++] = c;


    // File Header
    dst->fileHeader.id = Hex2I(&BMPArray, 0, 2);
    dst->fileHeader.file_size = Hex2I(&BMPArray, 2, 4);
    dst->fileHeader.arr_offset = Hex2I(&BMPArray, 10, 4);



    // Image Header
    dst->imageHeader.header_size = Hex2I(&BMPArray, 14, 4);
    dst->imageHeader.width = Hex2I(&BMPArray, 18, 4);
    dst->imageHeader.height = Hex2I(&BMPArray, 22, 4);
    dst->imageHeader.bits_pp = Hex2I(&BMPArray, 28, 2);
    dst->imageHeader.img_size = Hex2I(&BMPArray, 34, 4);
    if (dst->imageHeader.bits_pp == 32)
        printf("GetBMP: File is 32 bits per pixel.\nThis can cause problems while using the Straightening function.\n");

    // Complete image header
    unsigned char *completeArray = malloc(dst->imageHeader.header_size * sizeof(unsigned char));
    for (int i = 0; i < dst->imageHeader.header_size; i++) {
        *(completeArray+i) = BMPArray[14+i];
    }
    dst->imageHeader.completeArray = completeArray;



    // Table of color management
    int offset = 54;
    Uint reste = dst->fileHeader.file_size - dst->imageHeader.header_size - 14 - dst->imageHeader.img_size;
    if(reste > 0) {
        dst->palette = Hex2I(&BMPArray, 54, reste);
        offset += reste;
    }

    // Retrieve pixels
    unsigned char* pixels = malloc((dst->imageHeader.img_size) * sizeof (unsigned char ));
    for(int i = 0; i < dst->imageHeader.img_size; i++)
        *(pixels+i) = *(BMPArray+offset+i);
    dst->pixels = pixels;

    fclose(fp);
    free(BMPArray);
}