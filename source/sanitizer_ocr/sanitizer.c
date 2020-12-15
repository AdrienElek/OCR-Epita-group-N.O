#include "stdlib.h"
#include "sanitizer.h"
#include "loader.h"
#include "stdio.h"
#include "math.h"

// x^n
double powerd(double x, int n){
    double sum = 1;
    for(int i = 0; i < n; i++){
        sum *= x;
    }
    return sum;
}


// Verify if a bmp is already greyscaled
// Return 0 if its not
// Return 1 if its already Grayscales
int VerifyGrayscale(BMP* bmp){
    PIX cur;
    for (int i = 0; i < bmp->imageHeader.width; ++i) {
        for (int j = 0; j < bmp->imageHeader.height; ++j) {
            cur = GetPixel(bmp, i, j);
            if (!(cur.r == cur.g && cur.g == cur.b)) {
                return 0;
            }
        }
    }
    return 1;
};


// Convert a rgb bmp to a greyscale bmp
void Greyscale(BMP* bmp, BMP* gs_bmp){
    CopyBMP(bmp, gs_bmp);

    if (VerifyGrayscale(bmp) == 1) {
        //printf("Already Grayscaled !\n");
        return;
    }

    for (int i = 0; i < bmp->imageHeader.width; i++){
        for (int j = 0; j < bmp->imageHeader.height; j++) {
            PIX cur = GetPixel(bmp, i, j);
            unsigned char grey = (unsigned char)(0.30*cur.r + 0.59*cur.g + 0.11*cur.b);
            cur.r = grey;
            cur.g = grey;
            cur.b = grey;
            SetPixel(gs_bmp, i, j, cur);
        }
    }
}





// DENOISING PART

void InsertSort(unsigned char color_value, unsigned char **median_list, int lenL){
    int i = 0;
    unsigned char* median = *median_list;
    while (i<lenL && color_value<*(median+i))
        i += 1;

    // Insertion procedure
    for (int j = i; j < lenL; j++) {
        *(median+j+1) = *(median+j);
    }
    *(median+i) = color_value;
}




// Get the denoised grey value for a pixel in the source bmp
Uint DenoisingFilter(BMP* bmp, int i, int j, int filter_size){
    unsigned char *median_list = malloc(sizeof(unsigned char) * filter_size * filter_size);
    int lenL = 0;

    // Construct pixels histogram
    for (int w = 0; w < filter_size; w++) {
        for (int h = 0; h < filter_size; h++) {
            int x = i - filter_size/2 + w;
            int y = j - filter_size/2 + h;
            if (0<=x && x < bmp->imageHeader.width && 0<=y && y<bmp->imageHeader.height){
                InsertSort(GetPixel(bmp, x, y).r, &median_list, lenL);
                lenL +=1;
            }
        }
    }

    Uint ret = *(median_list + lenL/2);
    free(median_list);
    return ret;
}



// Filter noise with median filter
// Mask is 3x3 by default
void Denoising(BMP* gs_bmp, BMP* dn_bmp){
    CopyBMP(gs_bmp, dn_bmp);
    PIX pix;

    for (int i = 0; i < gs_bmp->imageHeader.width; i++){
        for (int j = 0; j < gs_bmp->imageHeader.height; j++) {
            Uint grey_value = DenoisingFilter(gs_bmp, i, j, 3);
            pix.r = grey_value;
            pix.g = grey_value;
            pix.b = grey_value;
            SetPixel(dn_bmp, i, j, pix);
        }
    }
}




// BINARY (OTSU) PART


// Return sum of value in <L> array between [left, right]
// where L is an array of floating numbers (prob)
double Sum(double** L, unsigned char left, unsigned char right){
    double *list = *L;
    double sum = 0;

    for (int i = left; i <= right; i++)
        sum += *(list+i);

    return sum;
}


// Build pixels histogram in <histogram> array
// <histogram> array must have a 256 length
// and every cells is 0
// python example would be: `histo = [0 for i in range(256)]`
void Histogram(BMP* gs_bmp, Uint** histogram){
    Uint *histo = *histogram;
    unsigned char pxl_value;

    for (int i = 0; i < gs_bmp->imageHeader.width; i++) {
        for (int j = 0; j < gs_bmp->imageHeader.height; j++){
            pxl_value = GetPixel(gs_bmp, i, j).r;
            *(histo+pxl_value) += 1;
        }
    }
}


// Returns the mean of the histogram <histo>
// between [left, right]
double Mean(double** prob, unsigned char left, unsigned char right){
    double *histo = *prob;
    double sum = 0;
    double divide = 0;

    for (int i = left; i <= right; i++){
        sum += *(histo+i) * (i);
        divide += *(histo+i);
    }

    if (divide == 0)
        return 0;
    //printf("sum: %f, divide: %f\n", sum, divide);
    return sum/divide;
}

// Finds out inter variance of the 2 clusters
// Separated by the threshold <TH> value
double VarInter(double** prob, unsigned char TH){
    // Prob sum
    double sum = Sum(prob, 0, 255);
    double sum_low = Sum(prob, 0, TH);
    double sum_high = Sum(prob, TH+1, 255);
    //printf("sum: %f  sum_low: %f  sum_high: %f\n", sum, sum_low, sum_high);

    // Prob means
    double mean = Mean(prob, 0, 255);
    double mean_low = Mean(prob, 0, TH);
    double mean_high = Mean(prob, TH+1, 255);

    // Variance
    double varSum = (sum_low * powerd(mean_low, 2))
            + (sum_high * powerd(mean_high, 2));
    double varIC = varSum / sum - powerd(mean, 2);
    //printf("varIC: %f\n\n\n", varIC);

    return varIC;
}






unsigned char GetOtsuTH(BMP* gs_bmp){
    Uint nbpxl = gs_bmp->imageHeader.width * gs_bmp->imageHeader.height;

    Uint *histo = calloc(256, sizeof(Uint));
    Histogram(gs_bmp, &histo);


    double *proba = malloc(256 * sizeof(double));
    for (int i = 0; i < 256; i++)
        *(proba+i) = (double)*(histo+i) / (double)nbpxl;



    double varIC = 0;
    unsigned char varIC_index = 0;

    double cur_VarIC;
    for (unsigned char TH = 1; TH < 255; TH++) {
        cur_VarIC = VarInter(&proba, TH);

        // Save var if higher (Get var max)
        if (varIC < cur_VarIC){
            varIC = cur_VarIC;
            varIC_index = TH;
        }
    }

    //printf("OtsuTH: %d\n", varIC_index);
    free(proba);
    return varIC_index;
}


void BinaryFromTH(BMP* gs_bmp, BMP* bn_bmp, unsigned char threshold){
    CopyBMP(gs_bmp, bn_bmp);
    PIX white = {.r=255, .g=255, .b=255};
    PIX black = {.r=0, .g=0, .b=0};

    PIX pix;
    for (int i = 0; i < gs_bmp->imageHeader.width; ++i) {
        for (int j = 0; j < gs_bmp->imageHeader.height; ++j) {
            pix = GetPixel(gs_bmp, i, j);
            if (pix.r <= threshold)
                SetPixel(bn_bmp, i, j, black);
            else
                SetPixel(bn_bmp, i, j, white);
        }
    }
}


void Binary(BMP* gs_bmp, BMP* bn_bmp){
    unsigned char threshold = GetOtsuTH(gs_bmp);
    BinaryFromTH(gs_bmp, bn_bmp, threshold);
}









// Hough transform part (straightening)


// BANANA ROTAT E
void Rotate(BMP* bmp, BMP* rotate_bmp, int angle){
    int midX = (int)(bmp->imageHeader.width/2);
    int midY = (int)(bmp->imageHeader.height/2);
    double cosA = cos(angle*M_PI/180);
    double sinA = sin(angle*M_PI/180);



    PIX pix;
    for (int i = 0; i < bmp->imageHeader.width; i++) {
        for (int j = 0; j < bmp->imageHeader.height; j++) {

            int x = (int)((i-midX)*cosA + (j-midY)*sinA) + midX;
            int y = (int)(-(i-midX)*sinA + (j-midY)*cosA) + midY;

            if (x>=0 && x<bmp->imageHeader.width && y>=0 && y<bmp->imageHeader.height){
                pix = GetPixel(bmp, x, y);
                SetPixel(rotate_bmp, i, j, pix);
            }
        }
    }



}




const float EdgedDetectMatrix[3][3] = {
        {0, 1, 0},
        {1, -4, 1},
        {0, 1, 0}
};


/*const float SharpenMatrix[3][3] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0},
};*/



int Borne(int x){
    if(x < 0)
        return 0;
    if (x > 255)
        return 255;
    return x;
}

void EdgeDetect(BMP* bmp, BMP* ed_bmp, const float* Matrix){
    PIX pix;
    float sum_r, sum_g, sum_b;
    int filter_size = 3;


    for (int i = 0; i < bmp->imageHeader.width; i++) {
        for (int j = 0; j < bmp->imageHeader.height; j++) {

            // reset mask
            sum_r = 0;
            sum_g = 0;
            sum_b = 0;

            // Apply filter
            for (int w = 0; w < filter_size; w++) {
                for (int h = 0; h < filter_size; h++) {
                    int x = i - filter_size/2 + w;
                    int y = j - filter_size/2 + h;
                    if (0<=x && x < bmp->imageHeader.width && 0<=y && y<bmp->imageHeader.height){
                        pix = GetPixel(bmp, x, y);
                        sum_r += (float)pix.r * *(Matrix + w + h * filter_size);
                        sum_g += (float)pix.g * *(Matrix + w + h * filter_size);
                        sum_b += (float)pix.b * *(Matrix + w + h * filter_size);
                    }
                }
            }

            // Save pixel
            pix.r = Borne((int)sum_r);
            pix.g = Borne((int)sum_g);
            pix.b = Borne((int)sum_b);
            SetPixel(ed_bmp, i, j, pix);
        }
    }
}






void HoughTrans(BMP* bmp, Uint **accumulator, int Rmax){

    Uint *accu = *accumulator;
    int x, y;
    int r;
    PIX pix;
    for(int i = 0; i < bmp->imageHeader.width; i++){
        for (int j = 0; j < bmp->imageHeader.height; j++) {

            pix = GetPixel(bmp, i, j);
            if (pix.r > 200 && pix.g > 200 && pix.b > 200){
                for (int theta = 1; theta <= 180; theta++) {
                    x = i - (int)(bmp->imageHeader.width / 2);
                    y = j - (int)(bmp->imageHeader.height / 2);
                    r = (int) (x * cos(theta*M_PI/180) + y * sin(theta*M_PI/180));
                    *(accu + (r + Rmax) + (theta - 1) * 2 * Rmax) += 1;
                }
            }
        }
    }
}




int GetAngleFromAccu(BMP* bmp, Uint **accumulator, int Rmax){
    Uint *accu = *accumulator;


    int thetaMax = 0;
    int max = 0;
    int threshold = 3/5;
    if (bmp->imageHeader.width < bmp->imageHeader.height)
        threshold *= bmp->imageHeader.height;
    else
        threshold *= bmp->imageHeader.width;


    // Loop through all accumulator value to get max angle
    for (int i = 0; i < Rmax*2; i++) {
        for (int j = 0; j < 180; j++) {
            if(*(accu + i + j * Rmax * 2) > threshold){

                //printf("accu[%d,%d]= %d\n", i, j+1, *(accu + i + j * Rmax * 2));
                if (max < *(accu + i + j * Rmax * 2)){
                    max = *(accu + i + j * Rmax * 2);
                    thetaMax = j+1;
                }
            }


        }
    }



    //printf("angle: %d, at value: %d\n", thetaMax, max);

    // A positive angle rotate to the left
    thetaMax = thetaMax % 90;
    if (thetaMax >= 45)
        thetaMax = 90 - thetaMax;
    else
        thetaMax = -thetaMax;

    return thetaMax;
}




void Straighten(BMP* bmp, BMP* st_bmp){
    BMP edge_bmp;
    CopyBMP(bmp, &edge_bmp);
    EdgeDetect(bmp, &edge_bmp, &(EdgedDetectMatrix[0][0]));
    SaveBMP(&edge_bmp, "edge_detect.bmp");


    int Rmax = (int)sqrt(pow(bmp->imageHeader.width, 2) + pow(bmp->imageHeader.height, 2)) / 2;
    Uint *accu = calloc(Rmax * 2 * 180, sizeof(Uint));
    HoughTrans(bmp, &accu, Rmax);



    int angle = GetAngleFromAccu(bmp, &accu, Rmax);
    //printf("real angle: %d\n", angle);

    CopyBMPWithEmptyPixels(bmp, st_bmp);
    Rotate(bmp, st_bmp, -angle);

}


