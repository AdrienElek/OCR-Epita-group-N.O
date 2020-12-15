#include <stdio.h>
#include <stdlib.h>
#include "segmentation.h"

struct Tree* newTree(int orix, int oriy, int destx, int desty)
{
    struct Tree *tree = (struct Tree*)malloc(sizeof (struct Tree));
    if (tree == NULL)
    {
        printf("\nEspace memoire insuffisant\n");
    }
    
    tree->oriX = orix;
    tree->oriY = oriy;
    tree->destX = destx;
    tree->destY = desty;

    tree->child = NULL;
    tree->sibling = NULL;
    
    return(tree);
    
}


void historygramX(int *histo, int *array, int lenarray, struct Tree *G)
{
    int count =0;
    for (int i = G->oriY; i < G->destY; i++)
    {
        histo[count] = 0;
        for (int j = G->oriX; j < G->destX; j++)
        {
            histo[count] += array[i*lenarray + j];
            
        }
        count+=1;
    }
}

void historygramY(int *histo, int *array, int lenarray, struct Tree *G)
{
    int count =0;
    for (int i = G->oriX; i < G->destX; i++)
    {
        histo[count]=0;

        for (int j = G->oriY; j < G->destY; j++)
        {
            histo[count] += array[i + j*lenarray];
        }
        
        count+=1;
    }  

}

/* Cherche la taille d'espace moyenne, permet la séparation des mots  */
int spacethreshold(int *histo, int len)
{
    int count =0;
    int countsize =0;
    int isLine = 1;
    /* Permet d'ignorer les fin de ligne blanches !!  */
    int intermediaire = 0;
    int i = 0;

    while ( i<len && histo[i] == 0)
    {
        i+=1;
    }
    
    for (; i < len; i++)
    {
        /* Count number of spaces  */
        if (histo[i]==0 && isLine == 1 )
        {
            isLine =0;
            
        }
        else if (histo[i]>0 && isLine == 0)
        {
            countsize+=intermediaire;
            intermediaire =0;
            isLine = 1;
            count+=1;
        }
        /* count total size of space  */
        if (isLine == 0)
        {
            intermediaire+=1;
        }
    }

    if(count == 0)
    {
        return count;
    }
    return countsize/count;
}

/*séparation des caractères*/
struct Tree* __car(struct Tree* G, int *histo, int *ind, int lenx)
{
    int out =0;
    int __oriX =0;
    int __destX =0;

    while (*ind<lenx && out<2)
    {
        if (out == 0 && histo[*ind]>0)
        {
            out =1;
            __oriX = G->oriX+*ind;
        }
        else if (out ==1 && histo[*ind]==0)
        {
            out =2;
            __destX = G->oriX+*ind;
        }
        *ind+=1;
        
    }
    if (out == 1 && *ind == lenx)
    {
        
        __destX = G->destX;
        out =2;
        
    }

    if (out == 2)
    {
        return newTree(__oriX, G->oriY, __destX, G->destY);
    }
    return NULL;
}

int car(struct Tree* G, int *array, int maxleny)
{

    int numberchar =0;
    int lenx = (G->destX - G->oriX);

    int histor[lenx];
    
    historygramY(histor, array, maxleny, G);


    int ind =0;
    G->child = __car(G, histor, &ind, lenx);
    struct Tree *g = G->child;

    while (ind<lenx && g!=NULL)
    {
        numberchar+=1;
        g->sibling = __car(G, histor, &ind, lenx);
        g = g->sibling;
        
    }
    if (g != NULL)
    {
        numberchar+=1;
        
    }
    return numberchar;
}

/*séparation des mots*/
struct Tree* __word(struct Tree* G, int *histo, int *indw, int lenx, int threshold )
{

    int count = 0;
    int out =0;
    int __oriX =0;
    int __destX = 0;
    while (*indw<lenx && out<2)
    {
        if (out == 0 && histo[*indw]>0)
        {
            out =1;
            __oriX = G->oriX+*indw;
        }
        else if (out ==1 && histo[*indw]==0)
        {
            count+=1;
            if (count>threshold)
            {
                out =2;
                __destX = G->oriX+*indw - threshold;
            }  
        }
        else
        {
            count =0;
        }
        *indw+=1;
        
    }
    if (out == 1 && *indw == lenx)
    {
        out =2;
        __destX = G->destX;
    }
    if (out == 2)
    {
        return newTree(__oriX, G->oriY, __destX, G->destY);
    }
    return NULL;
}

int word(struct Tree *G, int *array, int maxleny)
{
    int numberchar =0;
    int lenx = (G->destX - G->oriX);
    
    int histo[lenx];
    historygramY(histo, array, maxleny, G);

    int indw =0;
    int threshold = spacethreshold(histo, lenx);
    G->child = __word(G, histo, &indw, lenx, threshold);
    struct Tree *g = G->child;
    
    while (indw<lenx && g!=NULL)
    {   
        numberchar+= car(g, array, maxleny)+1;
        g->sibling = __word(G, histo, &indw, lenx, threshold);
        g = g->sibling;
    }
    
    if (g!=NULL)
    {
        numberchar += car(g, array, maxleny)+1;

    }
    
    return numberchar;
    
}


/**/
/*séparation des lignes*/
struct Tree* __lines(struct Tree *G, int *histo, int *ind, int leny)
{
    int out = 0;
    int __oriY =0;

    while (*ind<leny && out < 2)
    {
        if (histo[*ind]>0 && out==0)
        {
            out =1;
            __oriY = G->oriY + *ind;
        }
        else if (histo[*ind] == 0  && out == 1)
        {
            out = 2;
        }
        *ind+=1;
    }


    if (*ind==leny && out==1)
    {
        out =2;
    }

    if (out == 2)
    {   
        *ind -=1;
        return newTree(G->oriX, __oriY, G->destX, G->oriY+*ind);
    }
    else
    {
        return NULL;
    }
}

int lines(struct Tree *G, int *array, int maxlenX)
{
    int numberchar = 0;
    /*Creation de l'histogramme*/
    int leny =(G->destY - G->oriY);

    int histo[leny];
    historygramX(histo, array, maxlenX, G);

    int ind =0;
    G->child = __lines(G, histo, &ind, leny);
    struct Tree *g = G->child;
    
    while (ind<leny && g!=NULL)
    {
        numberchar += word(g, array, maxlenX)+1;
        g->sibling = __lines(G, histo, &ind, leny);
        g = g->sibling;
        
    }
    if (g!=NULL)
    {

        numberchar+= word(g, array, maxlenX)+1;
    } 
    
    
    
    return numberchar;
    
}


/*segmentation ligne->mots->caractères, retour de l'arbre OwO */
int makeTree(struct Tree *G, int lenx,  int *array)
{
    int nbchar = lines(G, array, lenx);
    return nbchar;
}

/* renvoi la taille du coté le plus grand de G, sert de référence pour les rapports de longueurs du makechar()*/
/* recoupe aussi le caractère parfaitement s'il ne l'etait pas avant*/
int getsize(struct Tree *G, int *array, int lenarray)
{   
    int leny = (G->destY - G->oriY);
    int histo[leny];
    historygramX(histo, array, lenarray, G);
    int begin = 0;
    int __oriy = 0;
    for (int i = 0; i < leny; i++)
    {
        if (begin == 0 && histo[i]>0)
        {
            __oriy = G->oriY +i;
            begin =1;
        }
        else if (histo[i]==0 && begin == 1)
        {
            G->destY = G->oriY+i;
        }
    }
    
    G->oriY = __oriy;
    if((G->destY - G->oriY)>=(G->destX - G->oriX))
    {
        return (G->destY - G->oriY);
    }
    else
    {
        return (G->destX - G->oriX);
    }
}

int* makechar(struct Tree *G, int *array, int lenarray, int l)
{   
    int *sizedchar = calloc(16*16, sizeof(int));
    int arrayi;
    int arrayj;
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            arrayi = (i*l)/16 + G->oriY;
            arrayj = (j*l)/16 + G->oriX;
            if (arrayi>=G->destY || arrayj>=G->destX)
            {
                sizedchar[j+i*16] = 0;
            }
            else
            {
                sizedchar[j+i*16] = array[arrayj+arrayi*lenarray];
            }
        }
    }
    return sizedchar;
}

int* resizecar(struct Tree *G, int *array, int lenarray)
{
    int l = getsize(G, array, lenarray);
    return makechar(G, array, lenarray, l);
}

/*commande pour tests*/
/*gcc -Wall -Wextra -Werror -std=c99 -O1 -o main*/
