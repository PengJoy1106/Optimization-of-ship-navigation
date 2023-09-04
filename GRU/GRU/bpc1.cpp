#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#define ETA 1//ѧϰ�ʣ�����
#define PRECISION 0.001//����Ҫ��

typedef struct Neuron//��Ԫ�ṹ��
{
    double input;//����
    double output;//���
    double* weights;//Ȩ��
    double Error;//���
} NEURON;

typedef struct Layer//��ṹ��
{
    int numberOfNeurons;//��Ԫ����
    NEURON *neurons;//��һ����Ԫͷָ�룬����
} LAYER;

typedef struct NNet //����ṹ��
{
    int numberOfLayers;//�񾭲���
    LAYER *layers;//��һ���񾭲�ͷָ�룬����
} NNET;

double sigmoid(double v)//S�Σ���ʼrule����
{
    return 1 / (1 + exp(-v));
}

double randomWeight()//���Ȩ��ֵ
{
    return ((int)rand() % 100000) / (float)100000 - 1;
}

void createNetWorks(NNET *nnet, int NumberOfLayers, int* NumberOfNeurons)
//�������磬����Ϊ����ͷָ�롢�񾭲�������Ԫ����
{
    nnet->numberOfLayers = NumberOfLayers;//���񾭲�����ֵ������ṹ��Ĳ���Ԫ��
    nnet->layers = (LAYER*)malloc(NumberOfLayers * sizeof(LAYER));//����õ�һ���񾭲�ͷָ��
    for (int i = 0; i < NumberOfLayers; i++)//�񾭲����Ԫ������������ṹ
    {
        nnet->layers[i].numberOfNeurons = NumberOfNeurons[i];//����Ԫ����һһ��ֵ�������Ԫ��
        nnet->layers[i].neurons = (NEURON*)malloc(NumberOfNeurons[i] * sizeof(NEURON));//����õ�ÿһ�����Ԫͷָ��
    }
}

void init(NNET *nnet, double * inputs) //�������ʼ��������Ϊ������ָ�룬�������
{
    for (int i = 0; i < nnet->layers[0].numberOfNeurons; i++) //���������һ���������Ԫ
    {
        nnet->layers[0].neurons[i].output = inputs[i];//�������������θ�ֵ�������һ�������Ԫ�����Ԫ��
    }
    for (int i = 1; i < nnet->numberOfLayers; i++) //������������в�
    {
        for (int j = 0; j < nnet->layers[i].numberOfNeurons; j++)
            //����ÿ���������Ԫ������i��1��ʼ���������ӵڶ��㿪ʼ����
        {
            nnet->layers[i].neurons[j].weights = (double*)malloc(nnet->layers[i - 1].numberOfNeurons * sizeof(double));
            //Ϊÿ����Ԫ��Ȩֵ��������һ��˫���ȵĴ洢�ռ䣬����Ϊ��һ����Ԫ����
            //��ΪȨ��ֵ������һ���������н�ϼ����
            //�ұ���ÿһ����Ԫ��Ĭ������һ���ÿ��������м��㣬�������Ԫ���Ȩ������
            //��˸���ӦΪ��һ�����Ԫ�������ҵ�һ��ֻ��������Ϊ���������Ȩ�ش���
            double input = 0;//����������Ϊ0
            for (int kk = 0; kk < nnet->layers[i - 1].numberOfNeurons; kk++)
                //��kkС����һ�����Ԫ����ʱ����ѭ������Ϊ�ӵڶ��㿪ʼ������
            {
                double  weight = randomWeight();//����һ��Ȩ�������������ֵ---------------------------------------------------------------
                nnet->layers[i].neurons[j].weights[kk] = weight;//������������ֵ��Ȩ�����������λ
                input += nnet->layers[i - 1].neurons[kk].output*weight;//��Ȩ��ֵ����һ���Ӧ����Ԫ��������г˻����㲢�ۼ�     
            }
            nnet->layers[i].neurons[j].input = input;//�������ۼ�ֵ��ֵΪ����Ԫ������ֵ
            nnet->layers[i].neurons[j].output = sigmoid(input);//��rule����������ֵ���м���õ�����Ԫ�����ֵ
        }
    }
}

void feedforward(NNET *nnet) //ǰ������
{
    for (int i = 1; i < nnet->numberOfLayers; i++) //����ÿ�㣬�ӵڶ��㿪ʼ
    {
        for (int j = 0; j < nnet->layers[i].numberOfNeurons; j++) //����ÿ����Ԫ���ӵ�һ����ʼ
        {
            double input = 0;//�����ʼΪ0
            for (int kk = 0; kk < nnet->layers[i - 1].numberOfNeurons; kk++) //������һ�����Ԫ��������Ȩ������
            {
                double  weight = nnet->layers[i].neurons[j].weights[kk];
                input += nnet->layers[i - 1].neurons[kk].output*weight;
            }
            nnet->layers[i].neurons[j].input = input;//�ۼ�Ϊ��������
            nnet->layers[i].neurons[j].output = sigmoid(input);//��rule���������
        }
    }
}

void feedforwardWithiInput(NNET *nnet, double* input) //���г�ʼ�����ǰ������
{
    for (int i = 0; i < nnet->layers[0].numberOfNeurons; i++) //������һ�����Ԫ
    {
        nnet->layers[0].neurons[i].output = input[i];//��������Ϊ��һ����Ԫ���
    }
    for (int i = 1; i < nnet->numberOfLayers; i++) //�ӵڶ��㿪ʼ�����񾭲㣬���Ƶأ����в�������
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

void backprop(NNET *nnet, double* targets) //���������ڣ�����Ϊ����ͷָ���Ŀ��ֵ����
{
    //double **Errors= (double**)malloc(nnet->numberOfLayers * sizeof(double*));
    int num = nnet->layers[nnet->numberOfLayers - 1].numberOfNeurons;//�������һ�����Ԫ����
    //Errors[nnet->numberOfLayers - 1]=(double*)malloc((num+1)*sizeof(double));

    for (int i = 0; i < num; i++)//�������һ�����Ԫ
    {
        double out = nnet->layers[nnet->numberOfLayers - 1].neurons[i].output;//ȡ��out���и�ֵ
        nnet->layers[nnet->numberOfLayers - 1].neurons[i].Error = out*(1 - out)*(targets[i] - out);//������
    }

    for (int i = nnet->numberOfLayers - 1; i >= 0;)

        //�����һ������񾭲㣬ֱ����һ����ֹͣ��������Ϊû�и�ǰ����������
    {
        if (i != 0)//ֻҪ���ǵ�һ��
        {
            //  Errors[i - 1] = (double*)malloc(nnet->layers[i - 1].numberOfNeurons * sizeof(double));
            for (int jj = 0; jj < nnet->layers[i - 1].numberOfNeurons; jj++)
                //ǰһ�����Ԫ����,�������һ�����Ԫ�Ա���������Ԫ����Ȩֵ����
            {
                double temp = 0;//��temp��Ȩֵ*�������ۼ�
                for (int kk = 0; kk < nnet->layers[i].numberOfNeurons; kk++)//������Ԫ����
                {
                    temp += nnet->layers[i].neurons[kk].weights[jj] * nnet->layers[i].neurons[kk].Error;
                    nnet->layers[i].neurons[kk].weights[jj] = nnet->layers[i].neurons[kk].weights[jj] +
                            ETA * nnet->layers[i].neurons[kk].Error *nnet->layers[i - 1].neurons[jj].output;
                    //��ѧϰ��*���α���Ԫ����� *����Ԫ���������Ӧ���ϲ���Ԫ����� ����ֵ����Ȩֵ���е���;
                }
                double out = nnet->layers[i - 1].neurons[jj].output;
                //���ϲ�һ����Ԫ��һ��Ȩֵ�����˵���֮��ȡ����һ���������ڵ���Ԫ��output
                nnet->layers[i - 1].neurons[jj].Error = out * (1 - out)*temp;//��ĩ��������㹫ʽ��һ��
            }
        }
        i--;
    }
}

int main()
{
    NNET* net = (NNET*)malloc(sizeof(NNET));//��������ͷָ��
    int num = 3;
    int a[4] = { 5, 10, 1 };
    createNetWorks(net, num, a);//�������磬3�����磬��Ԫ�����ֱ�Ϊ5��10��1

    double input0[5] = { 1, 11.1, 1,0,5 };
    double input1[5] = { 1, 0, 1,0,1.5 };
    double input2[5] = { 1, 1, 0,0,3.5 };
    double input3[5] = { 0, 1, 1 ,0,7.5};//4�����룬3��Ԫ����Ϊ��һ����3����Ԫ

    double target0[3] = { 0.8,0.3 };
    double target1[3] = { 0.9,0.5 };
    double target2[3] = { 0.6,0.2 };
    double target3[3] = { 0.2,0.9 };//4�������һ��Ԫ����Ϊ���һ��ֻ��1����Ԫ

    //˼��������ò�����ʼ��һ�����磬
    init(net, input0);//�õ�һ��������г�ʼ��������ʡ��
    printf("\n");
    int alpha = 0;
    int flag = 0;
    while (1)
    {

        //ѵ������
        feedforwardWithiInput(net, input0);//ǰ��һ�Σ�����һ�Σ��ĸ����������һ��
        backprop(net, target0);

        feedforwardWithiInput(net, input1);
        backprop(net, target1);

        feedforwardWithiInput(net, input2);
        backprop(net, target2);

        feedforwardWithiInput(net, input3);
        backprop(net, target3);

        alpha++;//������������

        //���Թ���
        feedforwardWithiInput(net, input0);
        if (fabs(net->layers[2].neurons[0].output - target0[0]) >= PRECISION)//����Ҫ��0.00001
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

    //������
    printf("\n");
    printf("Numbers of iteration : %d", alpha);
    printf("\n");
    //�ٴ�ǰ����㲢���
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
