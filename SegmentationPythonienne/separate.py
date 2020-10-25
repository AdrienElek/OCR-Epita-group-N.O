from OcrClass import Tree
#adrien.lorge
#methode inspirée de la méthode XYcut, utilisation d'arbres généraux dans l'optique de traiter les images "pas normal" contenant plusieurs blocs
#de texte, les fonctions ne sont pas encore développé pour traiter ce genre de cas et font le travail de base de séparation sans réajustement 
#de la taille du caractère en 16x16
#source : http://tibo.lelore.free.fr/Traitement_Image/Etat_Art/Segmentation/XY.php
#source : https://www.haralick.org/conferences/71280952.pdf
#source : https://towardsdatascience.com/segmentation-in-ocr-10de176cf373
#source : https://github.com/ (for examples in C)

#fonction d'initialisation d'un arbre général avec deux points de coordonnées
def makeTree(array, c1 = [0,0], c2 = None):
    if not c2:
        c2 = [len(array)-1,len(array[0])-1]
    return Tree.Tree([c1,c2])



#WhiteLine/Col testent les pixels d'une ligne/col, renvoi True si la ligne/col est uniquement composé de 0
def whiteLine(array,ligne,val,valmax):
    while val<valmax and array[ligne][val]==0:
        val+=1
    return val == valmax

def whiteCol(array,ligne,val,valmax):
    while val<valmax and array[val][ligne]==0:
        val+=1
    return val == valmax

#Sépare toutes les boites de texte sur x, a pour effet de séparer toutes lignes d'un texte "normal"
#Fonctions Xcut et Ycut tres similaires, une seule fonction pour les deux semble possible
def Xcut(array, Box):

    XYmin = Box.key[0]
    XYmax = Box.key[1]
    xmin = XYmin[0]
    ymin = XYmin[1]
    xmax = XYmax[0]
    ymax = XYmax[1]
    white = True 
    for x in range(xmin,xmax+1):
        if white != whiteLine(array,x,ymin,ymax):
            if white:
                b=Tree.Tree([[x,ymin]])
            else :
                b.key.append([x-1,ymax])
                Box.children.append(b)
            white = whiteLine(array,x,ymin,ymax)
            

    if not white :
        b.key.append([x-1,ymax])
        Box.children.append(b)

    if Box.nbchildren==1:
        Box = Box.children[0]
#Sépare toutes les boites de texte sur y, a pour effet de séparer tout les charactères d'une ligne "normale"
def Ycut(array,Box):
    xymin = Box.key[0]
    xymax = Box.key[1]
    xmin = xymin[0]
    ymin = xymin[1]
    xmax = xymax[0]
    ymax = xymax[1]
    white = True 

    for y in range(ymin,ymax+1):
        if white != whiteCol(array,y,xmin,xmax):
            if white:
                b=Tree.Tree([[xmin,y]])
            else :
                b.key.append([xmax,y-1])
                Box.children.append(b)
            white = whiteCol(array,y,xmin,xmax)

    if not white :
        b.key.append([xmax,y-1])
        Box.children.append(b)

    if Box.nbchildren==1:
        Box = Box.children[0]

#def whatsInsideTheBox(B): 
#    print(B.key)
#    print("\n")
#    for t in B.children:
#        whatsInsideTheBox(t)
#primaire print fonction pour les arbres, affiche les clés en parcour prefixe

def fonctionprincipale(array,T):
    T = makeTree(array)
    Xcut(array,T)
    for t in T.children:
        Ycut(array,t)
    #whatsInsideTheBox(T)
