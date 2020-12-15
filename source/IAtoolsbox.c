// Authors: Arnaut.Leyre
// 07.12.2020/19:34

#include <math.h>

double sigmoid(double x)
{
    return (1 / (1 + exp(-x)));
}

double sigmoid_derivative(double x)
{
    return x * (1 - x);
}