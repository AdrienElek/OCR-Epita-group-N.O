// Authors: Arnaut.Leyre / Nicolas.Le-lan
// 07.12.2020/19:15

#include "IAtoolsbox.h"
#include "neuralnetwork.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// data
int sizeoflist = 11;
int sizeofInput = 256;
int sizesList[11] = {320, 320, 320, 320, 320, 320, 320, 320, 320, 320, 256};

// NeuralNetwork Methods.
struct NeuralNetwork initNN()
{
    time_t t;
    srand((unsigned) time(&t));
    NeuralNetwork nn =
            {
                    .size = sizeoflist,
                    .inputsize = sizeofInput,
                    .InputLayer = calloc(sizeofInput, sizeof(int)),
                    .LayerList = malloc(sizeoflist * sizeof(Layer)),
            };
    int psize = sizeofInput;
    double a = 4.0;
    for (int i = 0; i < sizeoflist; ++i)
    {
        int size = sizesList[i];
        Layer l =
                {
                        .size = size,
                        .NeuronList = malloc(size * sizeof(Neuron)),
                };
        for (int j = 0; j < size; ++j)
        {
            Neuron n =
                    {
                            .net = 0,
                            .val = 0,
                            .size = psize,
                            .bias = (((double)rand()/(double)(RAND_MAX)) * a) - (((double)rand()/(double)(RAND_MAX)) * a) ,
                            .weights = malloc(psize * sizeof(double)),
                    };
            for (int k = 0; k < psize; ++k)
            {
                n.weights[k] = (((double)rand()/(double)(RAND_MAX)) * a) - (((double)rand()/(double)(RAND_MAX)) * a) ;
            }
            l.NeuronList[j] = n;
        }
        psize = size;
        nn.LayerList[i] = l;
    }
    return nn;
}

void Free(NeuralNetwork nn)
{
    free(nn.InputLayer);
    for (int l = 0; l < sizeoflist; ++l)
    {
        for (int n = 0; n < sizesList[l]; ++n)
        {
            free(nn.LayerList[l].NeuronList[n].weights);
        }
        free(nn.LayerList[l].NeuronList);
    }
    free(nn.LayerList);
}

// Methods:Propagation
void RunLayers(Layer L,Layer Lminus1)
{
    for(int i=0; i<L.size;i++)
    {
        L.NeuronList[i].net = L.NeuronList[i].bias;
        for(int j=0; j<Lminus1.size; j++)
        {
            L.NeuronList[i].net += Lminus1.NeuronList[j].val*L.NeuronList[i].weights[j];

        }
        L.NeuronList[i].val = sigmoid(L.NeuronList[i].net);
    }
}


void RunNeuralNetwork(NeuralNetwork nn, int* Input)
{
    nn.InputLayer = Input;
    //init first layer
    for (int i=0; i<nn.LayerList[0].size; i++)
    {
        nn.LayerList[0].NeuronList[i].net = nn.LayerList[0].NeuronList[i].bias;
        for (int j=0 ; j<nn.inputsize; j++)
        {
            nn.LayerList[0].NeuronList[i].net += (double)Input[j]*nn.LayerList[0].NeuronList[i].weights[j];
        }
        nn.LayerList[0].NeuronList[i].val = sigmoid(nn.LayerList[0].NeuronList[i].net);
    }
    for (int l=1; l<nn.size; l++)
    {
        RunLayers(nn.LayerList[l],nn.LayerList[l-1]);
    }
}

void SaveIA(NeuralNetwork nn)
{
    FILE *IA;
    IA = fopen( "./IA.ia", "w");
    fprintf(IA, "%i\n",sizeoflist);
    fprintf(IA, "%i\n",sizeofInput);
    for (int i = 0; i < sizeoflist; ++i)
    {
        fprintf(IA, "%i\n",sizesList[i]);
    }
    for (int l = 0; l < sizeoflist; ++l)
    {
        fprintf(IA, "===== Layer %i =====\n",l);
        for (int n = 0; n < sizesList[l]; ++n)
        {
            fprintf(IA, "=== neuron [%i,%i] ===\n",l,n);
            fprintf(IA, "%f\n",nn.LayerList[l].NeuronList[n].bias);
            for (int w = 0; w < nn.LayerList[l].NeuronList[n].size; ++w)
            {
                fprintf(IA, "%f\n",nn.LayerList[l].NeuronList[n].weights[w]);
            }
        }
    }
    fclose(IA);
}

char RunIA(int* Input) {
    FILE *IA;
    IA = fopen("./IA.ia", "r");
    if (!IA) {
        printf("Failed to open text file\n");
        exit(1);
    }
    int Sizeoflist;
    int SizeofInput;
    fscanf(IA, "%i", &Sizeoflist);
    fscanf(IA, "%i", &SizeofInput);
    int *SizesList = calloc(Sizeoflist, sizeof(int));
    for (int k = 0; k < Sizeoflist; ++k) {
        fscanf(IA, "%i", &SizesList[k]);
    }
    NeuralNetwork nn =
            {
                    .size = Sizeoflist,
                    .inputsize = SizeofInput,
                    .InputLayer = calloc(SizeofInput, sizeof(int)),
                    .LayerList = malloc(Sizeoflist * sizeof(Layer)),
            };
    for (int i = 0; i < Sizeoflist; ++i) {
        char *string;
        fscanf(IA, "%c", &string);
        int size = SizesList[i];
        Layer l =
                {
                        .size = size,
                        .NeuronList = malloc(size * sizeof(Neuron)),
                };
        int psize = SizeofInput;
        for (int j = 0; j < size; ++j) {
            char *string;
            fscanf(IA, "%c", &string);
            double bias;
            fscanf(IA, "%lf", &bias);
            Neuron n =
                    {
                            .net = 0,
                            .val = 0,
                            .size = psize,
                            .bias = bias,
                            .weights = malloc(psize * sizeof(double)),
                    };
            for (int k = 0; k < psize; ++k) {
                double weight;
                fscanf(IA, "%lf", &weight);
                n.weights[k] = weight;
            }
            l.NeuronList[j] = n;
            psize = size;
        }
        nn.LayerList[i] = l;
    }
    fclose(IA);
    free(SizesList);
    RunNeuralNetwork(nn, Input);
    int max = 0;
    Layer l = nn.LayerList[nn.size - 1];
    for (int i = 0; i < l.size; ++i) {
        if (l.NeuronList[i].val > l.NeuronList[max].val) max = i;
    }
    char res = (char) ((int) l.NeuronList[max].val);
    Free(nn);
    return res;
}

