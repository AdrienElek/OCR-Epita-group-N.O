#include <stdio.h>
#include <stdlib.h>

//AUTHOR:Arnaut.leyre **** 08/10/20
//Get a size of pixel in the eder.
//Created with : http://www.machaon.fr/isn/representation/Les%20fichiers%20images%20BMP.htm
int Get_NB_pixel(int start,const unsigned char BMPArray[])
{
    int res = 0;
    for (int i = 0; i <= 4;i++)
    {
        int transi = (int) BMPArray[start+i];
        for (int j = 0; j < i*2 ; ++j)
        {
            transi = transi * 16;
        }
        res += transi;
    }
    return res;
}

//AUTHOR:Arnaut.leyre **** 08/10/20
//transform an Hexadecimal color in the BMPArray into a shade of gray.
//Created with : https://fr.wikipedia.org/wiki/Niveau_de_gris
int Fifty_Shade_of_Gray(int start,const unsigned char BMPArray[])
{
    return   (int) (BMPArray[start] * 0.0722 + BMPArray[start+1] * 0.7152+ BMPArray[start+2] * 0.2126);
}

//AUTHOR:Arnaut.leyre **** 30/09/20
//transforme a *.Bmp* file into a 2 dimension Array of 0 and 1.
//Created with : https://openclassrooms.com/forum/sujet/load-bmp-image-en-c
//               http://www.machaon.fr/isn/representation/Les%20fichiers%20images%20BMP.htm
//               https://openclassrooms.com/forum/sujet/malloc-tableau-double-dimension-97993
void Bmp_To_BArray()/**filepath "C:/Users/Pers0/Documents/Travail/OCR/image.bmp" **/
{
    FILE *fp = fopen("C:/Users/Pers0/Documents/Travail/OCR/image5.bmp","rb");

    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    unsigned char *BMPArray = malloc(fileSize * sizeof(unsigned char));
    int count = 0;
    int c;

    while((c = fgetc(fp)) != EOF)
    {
        BMPArray[count++] = c;
    }

    int NbCol = Get_NB_pixel(18,BMPArray);
    int NbLin = Get_NB_pixel(22,BMPArray);
    int gap =(4-(NbCol*3)%4)%4;

    /**for(int i = 54; i < fileSize; i++)
    {
        printf("%.2x ", BMPArray[i]);
    }
    printf("\n\n\n");**/ //DebugLog

    int **GrayMap = malloc(NbLin * sizeof(BMPArray));
    for (c = 0; c < NbLin; c++){
        GrayMap[c] = malloc(NbCol * sizeof(BMPArray));}

    int InLigne = 0;
    int TheLigne = NbLin-1;
    for(int i = 54; i < fileSize; i+=3)
    {
        if (InLigne%(NbCol)!=0||InLigne==0)
        {
            int res = Fifty_Shade_of_Gray(i,BMPArray);
            //printf("%d ", res); //DebugLog
            GrayMap[TheLigne][InLigne] = res;
            InLigne+=1;
        }
        else
        {
            i+=gap-3;
            InLigne = 0;
            TheLigne -= 1;
            //printf("\n"); //DebugLog
        }
    }

    int **BichromMap = malloc(NbLin * sizeof(BMPArray));
    for (c = 0; c < NbLin; c++){
        BichromMap[c] = malloc(NbCol * sizeof(BMPArray));}

    for (int i = 0; i < NbLin; ++i)
    {
        for (int j = 0; j < NbCol; ++j)
        {
            if (GrayMap[i][j] > 200)
                BichromMap[i][j]=0;
            else
                BichromMap[i][j]=1;
        }
    }

    printf("\n");

    free(BMPArray);
    fclose(fp);
}