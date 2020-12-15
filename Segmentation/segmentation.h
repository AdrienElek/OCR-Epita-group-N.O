
#include <stdio.h>
#include <stdlib.h>

/*struct bintree*/

struct Tree
{
    int oriX, oriY, destX, destY;
    struct Tree *child;
    struct Tree *sibling;
};

struct Tree* newTree(int orix, int oriy, int destx, int desty);

/*outils de séparation*/

void historygramX(int *histo, int *array, int lenarray, struct Tree *G);

void historygramY(int *histo, int *array, int lenarray, struct Tree *G);

int spacethreshold(int *histo, int len);

/*fonctions de segmentation*/

struct Tree* __car(struct Tree* G, int *histo, int *ind, int lenx);

int car(struct Tree* G, int *array, int maxleny);

struct Tree* __word(struct Tree* G, int *histo, int *ind, int lenx, int threshold );

int word(struct Tree *G, int *array, int maxleny);

struct Tree* __lines(struct Tree *G, int *histo, int *ind, int leny);

int lines(struct Tree *G, int *array, int maxleny);

/*fonction de construction de l'arbre*/

int makeTree(struct Tree *G, int lenx, int *array);

/*fonction de resize*/

int getsize(struct Tree *G, int *array, int lenarray);

int* makechar(struct Tree *G, int *array, int lenarray, int l);

/*fonction principale de resize*/

int* resizecar(struct Tree *G, int *array, int lenarray);

