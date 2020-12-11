#define SEGMENTATION_H
#ifndef SEGMENTATION_H


/*struct bintree*/

struct Tree;

struct Tree* newTree(int orix, int oriy, int destx, int desty);

/*outils de séparation*/

void historygramX(int *histo, int *array, int lenarray, struct Tree *G);

void historygramY(int *histo, int *array, int lenarray, struct Tree *G);

int spacethreshold(int *histo, int len);

/*fonctions de segmentation*/

struct Tree* __car(struct Tree* G, int *histo, int *ind, int lenx);

void car(struct Tree* G, int *array, int maxleny);

struct Tree* __word(struct Tree* G, int *histo, int *ind, int lenx, int threshold );

void word(struct Tree *G, int *array, int maxleny);

struct Tree* __lines(struct Tree *G, int *histo, int *ind, int leny);

void lines(struct Tree *G, int *array, int maxleny);

/*fonction de construction de l'arbre*/

struct Tree* makeTree(int lenx, int leny, int *array);

/*fonction de resize*/

int getsize(struct Tree *G, int *array, int lenarray);

int* makechar(struct Tree *G, int *array, int lenarray, int l);

/*fonction principale de resize*/

int* resizecar(struct Tree *G, int *array, int lenarray);

#endif