"""
@autor: Arnaut.Leyre & nicolas.le-lan 15/10/20
"""

# Import
import math
import random


# Class
class Neurone:
    influencedsum: float = 0
    value: float = 0
    bias: float = random.uniform(0,2)-1
    weights: [float] = []
    def __init__(self, lenPrevious):
        self.weights = [random.uniform(0,2)-1 for i in range(lenPrevious)]


class Layer:
    NeuroneList: [Neurone]
    def __init__(self, size, lenPrevious):
        self.NeuroneList = []
        for i in range(size):
          self.NeuroneList.append(Neurone(lenPrevious))


class NeuralNetwork:
    InputLayer: [int] = []
    LayerList: [Layer] = []
    def __init__(self, nbneuronelist: [int], inputlistsize: int):
        self.InputLayer = Layer(inputlistsize,0)
        lenPrevious: int = inputlistsize
        for size in nbneuronelist:
            self.LayerList.append(Layer(size, lenPrevious))
            lenPrevious = size


# Methods
# Methods:tools
def sigmoïde(x: float):
  return 1 / (1 + math.exp(-x))

def sigmoïde_derivative(x: float):
    return x * (1 - x)


# Methods:Propagation
def get_neural_value(n: Neurone, nlist: [Neurone]):
    res: float = n.bias
    for i in range(len(nlist)):
        res += nlist[i].value * n.weights[i]
    n.value = sigmoïde(res)
    n.influencedsum = res
    return n


def get_layer_neural_value(l: Layer, pre_n_list: [Neurone]):
    for i in range(len(l.NeuroneList)):
        l.NeuroneList[i] = get_neural_value(l.NeuroneList[i], pre_n_list)
    return l


def run_neural_network(nn: NeuralNetwork,input: [int]):
    for i in range(len(input)):
      nn.InputLayer.NeuroneList[i].value = input[i]
    nn.LayerList[0] = get_layer_neural_value(nn.LayerList[0], nn.InputLayer.NeuroneList)
    length: int = len(nn.LayerList)
    for i in range(1, length):
        nn.LayerList[i] = get_layer_neural_value(nn.LayerList[i], nn.LayerList[i - 1].NeuroneList)
    return nn.LayerList[length - 1].NeuroneList[0].value


#Methods:trainning



def get_linked_wieght_cost(n: Neurone,pn: Neurone):
  return pn.value*sigmoïde_derivative(n.influencedsum)*(n.value)

def get_linked_bias_cost(n: Neurone):
  return sigmoïde_derivative(n.influencedsum)*(n.value)

def get_sum_brnv(nIndex: int,bl: Layer):
  res = 0
  for j in range(len(bl.NeuroneList)):
    bn = bl.NeuroneList[j]
    res += bn.weights[nIndex]*sigmoïde_derivative(bn.influencedsum)*bn.value
  return sigmoïde(res)

def change_is_now(nn: NeuralNetwork,y: int):
  res: [[[[],float]]] = []
  for l in range(len(nn.LayerList)):
    res.append([])
    for j in range(len(nn.LayerList[l].NeuroneList)):
      res[l].append([[],float(0)])
  length = len(nn.LayerList)-1
  for g in range(len(nn.LayerList[length].NeuroneList)):
    nn.LayerList[length].NeuroneList[g].value =2*(nn.LayerList[length].NeuroneList[g].value-y)
  for l in range(len(nn.LayerList)-1,-1,-1):
    for j in range(len(nn.LayerList[l].NeuroneList)):
      for k in range(len(nn.LayerList[l].NeuroneList[j].weights)):
        if l == 0:
          res[l][j][0].append(get_linked_wieght_cost(nn.LayerList[l].NeuroneList[j],nn.InputLayer.NeuroneList[k]))
        else : 
          res[l][j][0].append(get_linked_wieght_cost(nn.LayerList[l].NeuroneList[j],nn.LayerList[l-1].NeuroneList[k]))
          nn.LayerList[l-1].NeuroneList[k].value = get_sum_brnv(k,nn.LayerList[l])
      res[l][j][1] += get_linked_bias_cost(nn.LayerList[l].NeuroneList[j])
  return res

#trainning

inputlist = [0, 1]
nblneuronnelist = [4, 4, 1]
nn = NeuralNetwork(nblneuronnelist,len(inputlist))

trainInput = [[0,0],[0,1],[1,0],[1,1]]
trainOutput = [0,1,1,0]

def training(nbtrain):
  l=len(trainInput)
  table: [[[[],float]]] = []
  for _ in range(nbtrain):
    i = random.randint(0, l-1)
    result  = (run_neural_network(nn,trainInput[i]))
    inter_res = change_is_now(nn,trainOutput[i])
    print(trainInput[i], trainOutput[i],result) 
    if table == []:
      table = inter_res
    else:
      for l in range(len(nn.LayerList),0):
        for j in range(len(nn.LayerList[l].NeuroneList),0):
          table[l][j][1] += inter_res[l][j][1]
          for k in range(len(nn.LayerList[l].NeuroneList[j].weights)):
            table[l][j][0][k] += inter_res [l][j][0][k]
  for l in range(len(nn.LayerList)):
    for j in range(len(nn.LayerList[l].NeuroneList)):
      nn.LayerList[l].NeuroneList[j].bias*=float(table[l][j][1])
      for k in range(len(nn.LayerList[l].NeuroneList[j].weights)):
        nn.LayerList[l].NeuroneList[j].weights[k] *= table[l][j][0][k]
  print("============")


#test

for i in range(20):
  print("============")
  print("epoch =",i)
  training(10)
