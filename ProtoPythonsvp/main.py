import ImageExtractor as Ie
import Otsu

def ProcessFilter(path):
    img = Ie.OpenImage(path)

    pathdn = Ie.Denoising(img, 3)
    pathdn.save("_dn"+path)

    pathga = Ie.GaussianFilter(img)
    pathga.save("_ga"+path)


def GrayscaleAndSave(path):
    img = Ie.OpenImage(path)
    img.save("_gs"+path)


def ProcessHisto(path):
    img = Ie.OpenImage(path)
    print("size : " + str(img.size[0] * img.size[1]))
    #return Otsu._BinaryHistogramme(img)
    return Otsu._Histogramme(img)


lena = "lena.jpeg"
otsuex = "otsuex.jpg"
maison = "maison.jpg"
bateau = "bateau.png"
txt = "txt.jpg"


"""ProcessFilter(bateau)
ProcessFilter(maison)
ProcessFilter(txt)"""

"""histo = ProcessHisto(bateau)
print(histo)"""

Ie.PathToBinary(otsuex, "_bn_otsuex.jpg")


"""def test():
    tab1 = [32, 34, 66, 79, 60, 29, 3]
    tab2 = [50, 58, 112, 138, 106, 55, 16]
    tabC = [82, 92, 178, 217, 166, 84, 19]

    moy1 = Otsu._Moy(tab1)
    moy2 = Otsu._Moy(tab2)
    moy = Otsu._Moy(tabC)

    sum1 = 0
    for i in range(len(tab1)):
        sum1 += tab1[i]

    sum2 = 0
    for i in range(len(tab2)):
        sum2 += tab2[i]

    var = ((sum1 * (moy1 ** 2)) + (sum2 * (moy2 ** 2))) / (sum1 + sum2) - moy ** 2
    print(var)

test()"""






