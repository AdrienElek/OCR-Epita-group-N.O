import PIL.Image
import Otsu


def OpenImage(path):
    img = PIL.Image.open(path)
    if not VerifyGreyscale(img):
        imgRGB = img.convert("RGB")
        print(f"size : {img.size[0]}x{img.size[1]}")

        gs_img = Grayscale(imgRGB)
        converted_img = gs_img.convert("1")
        return converted_img

    return img


def SaveImage(image, path):
    image.save(path)


def PathToBinary(path, path2save=None):
    gs_img = OpenImage(path)
    dn_img = Denoising(gs_img, 3)
    dn_img.save("_dncurrent.png")

    threshold = Otsu.OtsuTH(dn_img)
    print(threshold)
    bn_img = Otsu.BinaryFromTH(dn_img, threshold)
    if path2save is not None:
        bn_img.save(path2save)
    return bn_img



# ################# GRAYSCALE #################
def Grayscale(img):
    gs_img = PIL.Image.new("L", img.size)
    gs_pixels = gs_img.load()

    for i in range(img.size[0]):
        for j in range(img.size[1]):
            gs_pixels[i, j] = _GreyscaleFilter(img.getpixel((i, j)))

    return gs_img


def _GreyscaleFilter(pixel):
    brightness = int(0.33 * pixel[0] + 0.59 * pixel[1] + 0.11 * pixel[2])
    return brightness, brightness, brightness


def VerifyGreyscale(img):
    if type(img.getpixel((0, 0))) == int:
        return True
    for i in range(img.size[0]):
        for j in range(img.size[1]):
            pxl = img.getpixel((i, j))
            if not (pxl[0] == pxl[1] == pxl[2]):
                return False
    return True


# ################# DENOISING #################
def Denoising(img, filter_size):
    """Applique le masque de convolution a tout les pixels"""
    dn_img = PIL.Image.new("L", img.size)

    dn_pixels = dn_img.load()

    for w in range(img.size[0]):
        for h in range(img.size[1]):
            dn_pixels[w, h] = _DenoisingFilter(img, w, h, filter_size)

    return dn_img


def _DenoisingFilter(img, i, j, filter_size):
    """Applique le masque de convolution pour un le pixel (i,j) dans pixels"""
    lenL = 0
    L = []
    pixels = img.load()

    # Construction l'histogramme des pixels
    for w in range(filter_size):
        for h in range(filter_size):
            x = int(i - filter_size // 2 + w)  # Position (x,y) du coin en haut à gauche
            y = int(j - filter_size // 2 + h)  # du masque du filtre (filter_size)
            if 0 <= x < img.size[0] and 0 <= y < img.size[1]:
                _InsertSort(pixels[x, y], L, lenL)
                lenL += 1

    # Determination de la valeur medianne et mise sous tuple
    color = L[lenL // 2]
    return color


def _InsertSort(pixel, L, lenL):
    """tri par insertion dans un liste de valeur egale a
    la composante R d'un pixel. Dans le cas d'une image grayscale,
    tris les pixels"""

    i = 0
    while i < lenL and pixel < L[i]:
        i += 1
    L.insert(i, pixel)


# def _VerifySortedList(L):


# ################# GaussianFilter #################
def GaussianFilter(img):
    """Apply filter to every pixels"""
    ga_img = PIL.Image.new("L", img.size)
    pixels = ga_img.load()
    filtre = [
        [1, 2, 1],
        [2, 4, 2],
        [1, 2, 1]
    ]

    # Apply filter
    for i in range(img.size[0]):
        for j in range(img.size[1]):
            pixels[i, j] = ApplyFilter(img, i, j, filtre)

    return ga_img


def ApplyFilter(img, i, j, filter):
    pixels = img.load()
    sum = 0
    diviseur = 0
    for w in range(len(filter)):
        for h in range(len(filter[0])):
            x = int(i - len(filter) // 2 + w)
            y = int(j - len(filter) // 2 + h)
            if 0 <= x < img.size[0] and 0 <= y < img.size[1]:
                sum += filter[w][h] * pixels[x, y]
                diviseur += 1

    return int(sum / 16) if diviseur != 0 else 0
