import PIL.Image


def OtsuTH(gs_img):
    nbpxl = gs_img.size[0] * gs_img.size[1]
    histo = _Histogramme(gs_img)
    proba = [pix / nbpxl for pix in histo]

    var_ic_th = (0, 0)
    for seuil in range(1, 255):
        varIC = _VarInter(proba, seuil)

        # Sauvegarde de la variance inter max
        if var_ic_th[0] < varIC:
            var_ic_th = (varIC, seuil)

    return var_ic_th[1]


def BinaryFromTH(gs_img, threshold):
    binary_img = PIL.Image.new("1", gs_img.size)
    pixels = binary_img.load()
    for i in range(gs_img.size[0]):
        for j in range(gs_img.size[1]):
            if gs_img.getpixel((i, j)) <= threshold :
                pixels[i, j] = 0
            else:
                pixels[i, j] = 1
    return binary_img


# ####################### SUB FUNCTION Otsu 1################################
def _Histogramme(img):
    histo = [0 for i in range(256)]
    for i in range(img.size[0]):
        for j in range(img.size[1]):
            histo[img.getpixel((i, j))] += 1
    return histo


def _Sum(L):
    """retrun the sum of L values"""
    sum = 0
    for i in range(len(L)):
        sum += L[i]
    return sum


def _Moy(histo, T=0):
    """T is the min value of i"""
    sum = 0
    divide = 0
    for i in range(len(histo)):
        sum += histo[i] * (i + T)
        divide += histo[i]
    if divide == 0:
        return 0
    else:
        return sum / divide


def _VarInter(prob, T):
    sum = _Sum(prob)
    sum_low = _Sum(prob[:T+1])
    sum_high = _Sum(prob[T+1:])

    moy = _Moy(prob)
    moy_low = _Moy(prob[:T+1])
    moy_high = _Moy(prob[T+1:], T+1)
    # print("Low : {}   High : {}".format(moy_low, moy_high))

    varSum = (sum_low * (moy_low) ** 2 ) + (sum_high * (moy_high) ** 2)
    varIC = varSum / sum - (moy ** 2)

    return varIC


"""var_ic = (moy * low_class_prob - moy_low) ** 2 / high_x_low
var_ic = (moy_low * low_class_prob - moy_high * high_class_prob) ** 2 / high_x_low"""



