#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#define ETA 1//学习率，步长
#define PRECISION 0.001//精度要求

typedef struct Neuron//神经元结构体
{
    double input;//输入
    double output;//输出
    double* weights;//权重
    double Error;//误差
} NEURON;

typedef struct Layer//层结构体
{
    int numberOfNeurons;//神经元个数
    NEURON *neurons;//第一个神经元头指针，数组
} LAYER;

typedef struct NNet //网络结构体
{
    int numberOfLayers;//神经层数
    LAYER *layers;//第一层神经层头指针，数组
} NNET;

double sigmoid(double v)//S形，初始rule函数
{
    return 1 / (1 + exp(-v));
}

double randomWeight()//随机权重值
{
    return ((int)rand() % 100000) / (float)100000 - 1;
}

void createNetWorks(NNET *nnet, int NumberOfLayers, int* NumberOfNeurons)
//构造网络，参数为网络头指针、神经层数、神经元个数
{
    nnet->numberOfLayers = NumberOfLayers;//将神经层数赋值给网络结构体的层数元素
    nnet->layers = (LAYER*)malloc(NumberOfLayers * sizeof(LAYER));//构造得到一个神经层头指针
    for (int i = 0; i < NumberOfLayers; i++)//神经层和神经元都是连续数组结构
    {
        nnet->layers[i].numberOfNeurons = NumberOfNeurons[i];//将神经元个数一一赋值代入具体元素
        nnet->layers[i].neurons = (NEURON*)malloc(NumberOfNeurons[i] * sizeof(NEURON));//构造得到每一层的神经元头指针
    }
}

void init(NNET *nnet, double * inputs) //神经网络初始化，参数为神经网络指针，输入参数
{
    for (int i = 0; i < nnet->layers[0].numberOfNeurons; i++) //遍历网络第一层的所有神经元
    {
        nnet->layers[0].neurons[i].output = inputs[i];//将输入序列依次赋值给网络第一层各个神经元的输出元素
    }
    for (int i = 1; i < nnet->numberOfLayers; i++) //遍历网络的所有层
    {
        for (int j = 0; j < nnet->layers[i].numberOfNeurons; j++)
            //遍历每层的所有神经元，由于i从1开始，因此这里从第二层开始遍历
        {
            nnet->layers[i].neurons[j].weights = (double*)malloc(nnet->layers[i - 1].numberOfNeurons * sizeof(double));
            //为每个神经元的权值参数申请一个双精度的存储空间，个数为上一层神经元个数
            //因为权重值是与上一层的输出进行结合计算的
            //且本层每一个神经元都默认与上一层的每个输出进行计算，因此是神经元里的权重数组
            //因此个数应为上一层的神经元个数，且第一层只有输入作为输出，不用权重处理
            double input = 0;//初定义输入为0
            for (int kk = 0; kk < nnet->layers[i - 1].numberOfNeurons; kk++)
                //当kk小于上一层的神经元个数时进行循环（因为从第二层开始遍历）
            {
                double  weight = randomWeight();//定义一个权重数进行随机赋值---------------------------------------------------------------
                nnet->layers[i].neurons[j].weights[kk] = weight;//将这个随机数赋值给权重数组的依次位
                input += nnet->layers[i - 1].neurons[kk].output*weight;//将权重值和上一层对应的神经元的输出进行乘积计算并累加     
            }
            nnet->layers[i].neurons[j].input = input;//将输入累加值赋值为此神经元的输入值
            nnet->layers[i].neurons[j].output = sigmoid(input);//用rule函数对输入值进行计算得到本神经元的输出值
        }
    }
}

void feedforward(NNET *nnet) //前向网络
{
    for (int i = 1; i < nnet->numberOfLayers; i++) //遍历每层，从第二层开始
    {
        for (int j = 0; j < nnet->layers[i].numberOfNeurons; j++) //遍历每个神经元，从第一个开始
        {
            double input = 0;//输入初始为0
            for (int kk = 0; kk < nnet->layers[i - 1].numberOfNeurons; kk++) //根据上一层的神经元个数配置权重数组
            {
                double  weight = nnet->layers[i].neurons[j].weights[kk];
                input += nnet->layers[i - 1].neurons[kk].output*weight;
            }
            nnet->layers[i].neurons[j].input = input;//累加为函数输入
            nnet->layers[i].neurons[j].output = sigmoid(input);//用rule函数求输出
        }
    }
}

void feedforwardWithiInput(NNET *nnet, double* input) //带有初始输入的前向网络
{
    for (int i = 0; i < nnet->layers[0].numberOfNeurons; i++) //遍历第一层的神经元
    {
        nnet->layers[0].neurons[i].output = input[i];//将输入作为第一层神经元输出
    }
    for (int i = 1; i < nnet->numberOfLayers; i++) //从第二层开始遍历神经层，类似地，进行参数配置
    {
        for (int j = 0; j < nnet->layers[i].numberOfNeurons; j++)
        {
            double input = 0;
            for (int kk = 0; kk < nnet->layers[i - 1].numberOfNeurons; kk++)
            {
                double  weight = nnet->layers[i].neurons[j].weights[kk];
                input += nnet->layers[i - 1].neurons[kk].output*weight;
            }
            nnet->layers[i].neurons[j].input = input;
            nnet->layers[i].neurons[j].output = sigmoid(input);
        }
    }
}

void backprop(NNET *nnet, double* targets) //后向反馈调节，参数为网络头指针和目标值数组
{
    //double **Errors= (double**)malloc(nnet->numberOfLayers * sizeof(double*));
    int num = nnet->layers[nnet->numberOfLayers - 1].numberOfNeurons;//定义最后一层的神经元个数
    //Errors[nnet->numberOfLayers - 1]=(double*)malloc((num+1)*sizeof(double));

    for (int i = 0; i < num; i++)//遍历最后一层的神经元
    {
        double out = nnet->layers[nnet->numberOfLayers - 1].neurons[i].output;//取出out进行赋值
        nnet->layers[nnet->numberOfLayers - 1].neurons[i].Error = out*(1 - out)*(targets[i] - out);//误差计算
    }

    for (int i = nnet->numberOfLayers - 1; i >= 0;)

        //从最后一层遍历神经层，直到第一层则停止操作，因为没有更前层用来调节
    {
        if (i != 0)//只要不是第一层
        {
            //  Errors[i - 1] = (double*)malloc(nnet->layers[i - 1].numberOfNeurons * sizeof(double));
            for (int jj = 0; jj < nnet->layers[i - 1].numberOfNeurons; jj++)
                //前一层的神经元数量,逐个用上一层的神经元对本层所有神经元进行权值调整
            {
                double temp = 0;//用temp将权值*误差进行累加
                for (int kk = 0; kk < nnet->layers[i].numberOfNeurons; kk++)//本层神经元数量
                {
                    temp += nnet->layers[i].neurons[kk].weights[jj] * nnet->layers[i].neurons[kk].Error;
                    nnet->layers[i].neurons[kk].weights[jj] = nnet->layers[i].neurons[kk].weights[jj] +
                            ETA * nnet->layers[i].neurons[kk].Error *nnet->layers[i - 1].neurons[jj].output;
                    //将学习率*当次本神经元的误差 *本神经元的误差所对应的上层神经元的输出 所得值加在权值进行调节;
                }
                double out = nnet->layers[i - 1].neurons[jj].output;
                //用上层一个神经元对一层权值进行了调整之后，取出上一层用来调节的神经元的output
                nnet->layers[i - 1].neurons[jj].Error = out * (1 - out)*temp;//非末层的误差计算公式不一样
            }
        }
        i--;
    }
}

int main()
{
    NNET* net = (NNET*)malloc(sizeof(NNET));//建立网络头指针
    int num = 3;
    int a[4] = { 5, 10, 1 };
    createNetWorks(net, num, a);//建立网络，3层网络，神经元个数分别为5、10、1

    double input0[5] = { 1, 11.1, 1,0,5 };
    double input1[5] = { 1, 0, 1,0,1.5 };
    double input2[5] = { 1, 1, 0,0,3.5 };
    double input3[5] = { 0, 1, 1 ,0,7.5};//4种输入，3个元素因为第一层有3个神经元

    double target0[3] = { 0.8,0.3 };
    double target1[3] = { 0.9,0.5 };
    double target2[3] = { 0.6,0.2 };
    double target3[3] = { 0.2,0.9 };//4种输出，一个元素因为最后一层只有1个神经元

    //思考：如何用参数初始化一个网络，
    init(net, input0);//用第一个输入进行初始化，不能省，
    printf("\n");
    int alpha = 0;
    int flag = 0;
    while (1)
    {

        //训练过程
        feedforwardWithiInput(net, input0);//前向一次，后向一次，四个输入输出轮一回
        backprop(net, target0);

        feedforwardWithiInput(net, input1);
        backprop(net, target1);

        feedforwardWithiInput(net, input2);
        backprop(net, target2);

        feedforwardWithiInput(net, input3);
        backprop(net, target3);

        alpha++;//迭代次数计数

        //测试过程
        feedforwardWithiInput(net, input0);
        if (fabs(net->layers[2].neurons[0].output - target0[0]) >= PRECISION)//精度要求0.00001
        {
            flag = 1;
            //continue;
        }
        feedforwardWithiInput(net, input1);
        if (fabs(net->layers[2].neurons[0].output - target1[0]) >= PRECISION)
        {
            flag = 1;
            //continue;
        }
        feedforwardWithiInput(net, input2);
        if (fabs(net->layers[2].neurons[0].output - target2[0]) >= PRECISION)
        {
            flag = 1;
            //continue;
        }

        feedforwardWithiInput(net, input3);
        if (fabs(net->layers[2].neurons[0].output - target3[0]) >= PRECISION)
        {
            flag = 1;
            //continue;
        }
        
        if ((alpha <= 9965536)&&(flag == 1)) 
        	continue;
        else
                break;
    }

    //输出结果
    printf("\n");
    printf("Numbers of iteration : %d", alpha);
    printf("\n");
    //再次前向计算并输出
    double input[5] = { 1, 11.5, 1,0,5.5};
    
    
    feedforwardWithiInput(net, input);
    printf(" %f  \n", net->layers[2].neurons[0].output);
    
	
	feedforwardWithiInput(net, input0);
    printf(" %f  \n", net->layers[2].neurons[0].output);
    feedforwardWithiInput(net, input1);
    printf(" %f  \n", net->layers[2].neurons[0].output);
    feedforwardWithiInput(net, input2);
    printf(" %f  \n", net->layers[2].neurons[0].output);
    feedforwardWithiInput(net, input3);
    printf(" %f  \n", net->layers[2].neurons[0].output);
    getchar();
    return 0;
}
