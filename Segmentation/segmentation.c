#include <stdio.h>
#include <stdlib.h>

struct Tree
{
    int oriX, oriY, destX, destY;
    struct Tree *child;
    struct Tree *sibling;
};

struct Tree* newTree(int orix, int oriy, int destx, int desty)
{
    struct Tree *tree = (struct Tree*)malloc(sizeof (struct Tree));
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
    for (int i = G->oriY; i < G->destY; i++)
    {
        histo[i]=0;
        for (int j = G->oriX; j < G->destX; j++)
        {
            histo[i] += array[i*lenarray + j];
        }
    }
}

void historygramY(int *histo, int *array, int lenarray, struct Tree *G)
{

    for (int i = G->oriX; i < G->destX; i++)
    {
        histo[i]=0;

        for (int j = G->oriY; j < G->destY; j++)
        {
            histo[i] += array[i + j*lenarray];
        }
    }  
}

/* Cherche la taille d'espace moyenne, permet la séparation des mots  */
int spacethreshold(int *histo, int len)
{
    int count =0;
    int countsize =0;
    int isNotLine = 1;
    /* Permet d'ignorer les émargements et les fin de ligne blanches !!  */
    int intermediaire = 0;
    
    for (int i = 0; i < len; i++)
    {
        /* Count number of spaces  */
        if (histo[i]==0 && isNotLine != 1 )
        {
            isNotLine =1;
            count+=1;
        }
        else if (histo[i]>0 && isNotLine == 1)
        {
            count+=intermediaire;
            intermediaire =0;
            isNotLine = 0;
        }
        /* count total size of space  */
        if (isNotLine == 1)
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
    while (*ind<lenx)
    {
        if (out == 0 && histo[*ind]>0)
        {
            out =1;
            __oriX = G->oriX+*ind;
        }
        else if (out ==1 && histo[*ind]==0)
        {
            out =2;
        }
        *ind+=1;
        
    }
    if (out == 1 && *ind == lenx)
    {
        out =2;
    }
    if (out == 2)
    {
        return newTree(__oriX, G->oriY, G->oriX+*ind, G->destY);
    }
    return NULL;
}

void car(struct Tree* G, int *array, int maxleny)
{
    int lenx = (G->destX - G->oriX);

    int *histo = malloc(lenx * sizeof(int));
    historygramY(histo, array, maxleny, G);
    int ind =0;

    G->child = __car(G, histo, &ind, lenx);
    struct Tree *g = G->child;

    while (ind<lenx)
    {
        g->sibling = __car(G, histo, &ind, lenx);
        g = g->sibling;
    }
    free(histo);
}

/*séparation des mots*/
struct Tree* __word(struct Tree* G, int *histo, int *ind, int lenx, int threshold )
{
    int count = 0;
    int out =0;
    int __oriX =0;
    while (*ind<lenx)
    {
        if (out == 0 && histo[*ind]>0)
        {
            out =1;
            __oriX = G->oriX+*ind;
        }
        else if (out ==1 && histo[*ind]==0)
        {
            count+=1;
            if (count>threshold)
            {
                out =2;
            }  
        }
        else
        {
            count =0;
        }
        *ind+=1;
        
    }
    if (out == 1 && *ind == lenx)
    {
        out =2;
    }
    if (out == 2)
    {
        return newTree(__oriX, G->oriY, G->oriX+*ind, G->destY);
    }
    return NULL;
    
    
    

}

void word(struct Tree *G, int *array, int maxleny)
{
    int lenx = (G->destX - G->oriX);

    int *histo = malloc(lenx * sizeof(int));
    historygramY(histo, array, maxleny, G);
    int ind =0;

    int threshold = spacethreshold(histo, lenx);

    G->child = __word(G, histo, &ind, lenx, threshold);
    struct Tree *g = G->child;

    while (ind<lenx)
    {
        g->sibling = __word(G, histo, &ind, lenx, threshold);
        car(g, array, maxleny);
        g = g->sibling;
    }
    if (g!=NULL)
    {
        car(g, array, maxleny);
    }
    free(histo);
    
}


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
        return newTree(G->oriX, __oriY, G->destX, G->oriY+*ind);
    }
    else
    {
        return NULL;
    }
    
    
    
    


}

void lines(struct Tree *G, int *array, int maxleny)
{
    /*Creation de l'histogramme*/
    int leny =(G->destY - G->oriY);

    int *histo = malloc(leny * sizeof(int));
    historygramX(histo, array, maxleny, G);
    int ind =0;

    G->child = __lines(G, histo, &ind, leny);
    struct Tree *g = G->child;
    
    while (ind<leny)
    {
        g->sibling = __lines(G, histo, &ind, leny);
        word(g, array, maxleny);
        g = g->sibling;
    }
    if (g!=NULL)
    {
        word(g, array, maxleny);
    }
    free(histo);
    
}


/*segmentation ligne->mots->caractères, retour de l'arbre OwO */
struct Tree* makeTree(int lenx, int leny, int *array)
{
    struct Tree*G = newTree(0,0,lenx, leny);
    lines(G, array, lenx);
    return G;
}

/* renvoi la taille du coté le plus grand de G, sert de référence pour les rapports de longueurs du makechar()*/
/* recoupe aussi le caractère parfaitement s'il ne l'etait pas avant*/
int getsize(struct Tree *G, int *array, int lenarray)
{   
    int leny = (G->destY - G->oriY);
    int *histo = malloc(leny * sizeof(int));
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
    free(histo);
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
        for (int j = 0; i < 16; j++)
        {
            arrayi = (i*l)/16 + G->oriY;
            arrayj = (j*l)/16 +G->oriX;
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
