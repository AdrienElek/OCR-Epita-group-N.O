#include <stdio.h>
#include <stdlib.h>
#include "sanitizer.h"
#include "loader.h"
#include "segmentation.h"

/*converti le PIX* en int* 
Adrien Lorge*/
void convertbmp_int(BMP *bmp, int *array)
{
    PIX pix;
    for (size_t i = 0; i < bmp->imageHeader.height; i++)
     {
        for (size_t j = 0; j < bmp->imageHeader.width; j++)
        {
            pix = GetPixel(bmp, j, i);
            if (pix.r == 0)
            {
                array[j+(bmp->imageHeader.height- i)*bmp->imageHeader.width] = 1;
            }
            else
            {
                array[j+(bmp->imageHeader.height- i)*bmp->imageHeader.width] =0;
            }
        }
     }
}

/*Traduit les chars séparés, remplie le char *text avec les bon charactères*/
/*Adrien Lorge*/
void makeText(struct Tree *tree, int *array, char *text, int *ind, char *separators, int level, int width)
{
    if (tree->child != NULL)
    {
        makeText(tree->child, array, text, ind, separators, level+1, width);
        if (tree->sibling != NULL)
        {
            text[*ind] = separators[level];
            *ind +=1;
            makeText(tree->sibling, array, text, ind,separators, level, width);
        }
    }
    else
    {   /*#################################################################################################################################*/
        text[*ind] = 'h';
        //iachar(resizecar(tree, array, width));/* METTRE LA BONNE FONCTION*/
        /*#################################################################################################################################*/
        *ind+=1;
        if (tree->sibling != NULL)
        {
            makeText(tree->sibling, array, text, ind,separators, level, width);
        }
    }
    
    
    
    
}
/* effectue l'interpretation du texte*/
char* launch(char *file)
{
    /*Get bmp file, convert to BMP struct*/
    BMP *bmp = (BMP*)malloc(sizeof(BMP));
    GetBMP(file, bmp);

    /*greyscale*/
    BMP *gs_bmp = (BMP*)malloc(sizeof(BMP));
    Greyscale(bmp, gs_bmp);

    free(bmp->pixels);
    free(bmp);

    /*Binary*/
    BMP *binary_bmp = (BMP*)malloc(sizeof(BMP));
    Binary(gs_bmp, binary_bmp);

    free(gs_bmp);
    free(gs_bmp->pixels);

    /*Convert BMP binarised to int *array for segmentation treatment*/
    int *array = calloc(binary_bmp->imageHeader.width * binary_bmp->imageHeader.height, sizeof(int));
    convertbmp_int(binary_bmp, array);

    
    int width = binary_bmp->imageHeader.width;
    int height = binary_bmp->imageHeader.height;

    free(binary_bmp->pixels);
    free(binary_bmp);

    /*Build Tree*/
    struct Tree *tree = newTree(0,0,width,height);
    int nombrechar;
    nombrechar = makeTree(tree,width, array);

    /*Build char text*/
    char *text = malloc(nombrechar*sizeof(char));
    if (text == NULL)
    {
        printf("not enough space\n");
    }
    
    char separators[2] = {'\n', ' '};
    int ind =0;
    makeText(tree, array, text, &ind, separators, 0, width);
    return text;
    /*SaveBMP(&binary_bmp, "output.bmp");*/
}

int main()
{
    /*Launch créé le texte*/
    char *text = launch("ocrBTM1.bmp");
    printf("%s", text);
    free(text);
    return 0;
}