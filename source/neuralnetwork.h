// Authors: Arnaut.Leyre / Nicolas.Le-lan
// 08.12.2020/18:49

#ifndef UNTITLED3_NEURALNETWORK_H
#define UNTITLED3_NEURALNETWORK_H
// NeuralNetwork structure.
typedef struct Neuron
{
    double net;          // Sum of weights times L-1 values + bias.
    double val;          // Sigmoid of net.
    double bias;         // Bias of the Neuron.
    int size;           // Size of L-1.
    double* weights;     // Weights toward L-1.
} Neuron;

typedef struct Layer
{
    int size;           // Number of Neuron in the Layer.
    Neuron* NeuronList; // List of Neuron in the Layer.
} Layer;

typedef struct NeuralNetwork
{
    int size;           // Number of Layers not including InputLayer.
    int inputsize;      // Size of Input.
    int* InputLayer;    // Layer that the IA Input.
    Layer* LayerList;   // List of all Layers of the NeuralNetwork.
} NeuralNetwork;

NeuralNetwork initNN();
void Free(NeuralNetwork nn);
void RunLayers(struct Layer L,struct Layer Lminus1);
void RunNeuralNetwork(struct NeuralNetwork nn, int Input[]);
void SaveIA(NeuralNetwork nn);
char RunIA(int* input);

#endif //UNTITLED3_NEURALNETWORK_H
