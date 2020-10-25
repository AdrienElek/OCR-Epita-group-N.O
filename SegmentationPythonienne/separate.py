from OcrClass import Tree

"""Initie la clé du premier arbre"""


def makeTree(B,array, c1 = [0,0], c2 = None):
    if not c2:
        c2 = [len(array)-1,len(array[0])-1]
    B.key = [c1,c2]


"""Test si une ligne est constitué de pixel blanc uniquement"""


def whiteLine(array,ligne,val,valmax):
    while val<valmax and array[ligne][val]==0:
        val+=1
    return val == valmax

def whiteCol(array,ligne,val,valmax):
    while val<valmax and array[val][ligne]==0:
        val+=1
    return val == valmax

"""Sépare toutes les boites de texte sur x, a pour effet de séparer toutes lignes"""

"""Fonctions Xcut et Ycut tres similaires, une seule fonction pour les deux semble possible"""
def Xcut(array, Box):

    xymin = Box.key[0]
    xymax = Box.key[1]
    xmin = xymin[0]
    ymin = xymin[1]
    xmax = xymax[0]
    ymax = xymax[1]
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

def whatsInsideTheBox(B): 
    print(B.key)
    print("\n")
    for t in B.children:
        whatsInsideTheBox(t)

def principal(array,T):
    makeTree(T,array)
    Xcut(array,T)
    for t in T.children:
        Ycut(array,t)
    whatsInsideTheBox(T)
