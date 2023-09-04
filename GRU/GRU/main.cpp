#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <fstream>

// 定义激活函数 sigmoid
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// 生成指定范围内的随机数
double random(double min, double max) {
    static std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

class GRULayer {
public:
    int input_size;
    int hidden_size;

    std::vector<double> h_prev;

    std::vector<std::vector<double>> Wz;
    std::vector<std::vector<double>> Wr;
    std::vector<std::vector<double>> Wh;
    std::vector<double> bz;
    std::vector<double> br;
    std::vector<double> bh;

    GRULayer(int input_size, int hidden_size) : input_size(input_size), hidden_size(hidden_size) {
        h_prev.resize(hidden_size, 0.0);
        Wz.resize(hidden_size, std::vector<double>(input_size));
        Wr.resize(hidden_size, std::vector<double>(input_size));
        Wh.resize(hidden_size, std::vector<double>(input_size));
        bz.resize(hidden_size);
        br.resize(hidden_size);
        bh.resize(hidden_size);

        // 随机初始化权重和偏置
        for (int i = 0; i < hidden_size; i++) {
            for (int j = 0; j < input_size; j++) {
                Wz[i][j] = random(-5.0, 5.0);
                Wr[i][j] = random(-5.0, 5.0);
                Wh[i][j] = random(-5.0, 5.0);
            }
            bz[i] = random(-5.0, 5.0);
            br[i] = random(-5.0, 5.0);
            bh[i] = random(-5.0, 5.0);
        }
    }

    // 前向传播
    std::vector<double> forward(std::vector<double> x) {
        std::vector<double> z_t(hidden_size);
        std::vector<double> r_t(hidden_size);
        std::vector<double> h_t(hidden_size);

        for (int i = 0; i < hidden_size; i++) {
            z_t[i] = sigmoid(bz[i] + dot(Wz[i], x) + dot(Wz[i], h_prev));
            r_t[i] = sigmoid(br[i] + dot(Wr[i], x) + dot(Wr[i], h_prev));
            double inner = 0.0;
            for (int j = 0; j < input_size; j++) {
                inner += Wh[i][j] * r_t[i] * x[j];
            }
            h_t[i] = z_t[i] * h_prev[i] + (1 - z_t[i]) * tanh(bh[i] + inner);
        }

        h_prev = h_t;
        return h_t;
    }

    // 反向传播
    void backward(std::vector<double> x, double target, double learning_rate) {
        std::vector<double> z_t(hidden_size);
        std::vector<double> r_t(hidden_size);
        std::vector<double> h_t(hidden_size);

        for (int i = 0; i < hidden_size; i++) {
            z_t[i] = sigmoid(bz[i] + dot(Wz[i], x) + dot(Wz[i], h_prev));
            r_t[i] = sigmoid(br[i] + dot(Wr[i], x) + dot(Wr[i], h_prev));
            double inner = 0.0;
            for (int j = 0; j < input_size; j++) {
                inner += Wh[i][j] * r_t[i] * x[j];
            }
            h_t[i] = z_t[i] * h_prev[i] + (1 - z_t[i]) * tanh(bh[i] + inner);
        }

        std::vector<double> delta_h(hidden_size, 0.0);
        for (int i = 0; i < hidden_size; i++) {
            delta_h[i] = 2.0 * (h_t[i] - target); // 均方误差损失的梯度
        }

        // 更新权重和偏置
        for (int i = 0; i < hidden_size; i++) {
            for (int j = 0; j < input_size; j++) {
                Wz[i][j] -= learning_rate * delta_h[i] * z_t[i] * (1 - z_t[i]) * x[j];
                Wr[i][j] -= learning_rate * delta_h[i] * r_t[i] * (1 - r_t[i]) * x[j];
                Wh[i][j] -= learning_rate * delta_h[i] * (1 - z_t[i]) * tanh(bh[i] + dot(Wh[i], x)) * r_t[i] * (1 - r_t[i]) * x[j];
            }
            bz[i] -= learning_rate * delta_h[i] * z_t[i] * (1 - z_t[i]);
            br[i] -= learning_rate * delta_h[i] * r_t[i] * (1 - r_t[i]);
            bh[i] -= learning_rate * delta_h[i] * (1 - z_t[i]) * tanh(bh[i] + dot(Wh[i], x)) * r_t[i] * (1 - r_t[i]);
        }
    }

    // 计算向量点积
    double dot(const std::vector<double>& a, const std::vector<double>& b) {
        double result = 0.0;
        for (int i = 0; i < a.size(); i++) {
            result += a[i] * b[i];
        }
        return result;
    }
};

int main() {
    int input_size = 3;
    int hidden_size = 16;
    int sequence_length = 41;

    GRULayer gru(input_size, hidden_size);

    std::ifstream data_file("C:/Users/Pengyk/Desktop/QT_study/GRU/GRU/data.txt");

    std::vector<std::vector<double>> input_data;
    std::vector<double> target_data;

    double value;
    while (data_file >> value) {
        std::vector<double> row;
        row.push_back(value);
        for (int i = 2; i < input_size + 1; i++) {
            data_file >> value;
            row.push_back(value);
        }
        input_data.push_back(row);
        data_file >> value;
        target_data.push_back(value);
    }

    double learning_rate = 0.01;

    for (int epoch = 0; epoch < 1000; epoch++) {
        double loss = 0.0;
        for (int t = 0; t < sequence_length; t++) {
            std::vector<double> output = gru.forward(input_data[t]);
            double error = 0.0;
            double target = target_data[t]; // 单个目标值
            for (int i = 0; i < hidden_size; i++) {
                error += (output[i] - target) * (output[i] - target);
            }
            loss += error;

            // 反向传播和参数更新
            gru.backward(input_data[t], target, learning_rate);
        }
        loss /= sequence_length;
        std::cout << "Epoch " << epoch << ", Loss: " << loss << std::endl;
    }

    std::vector<double> input_data_to_predict = {10.0, 14.0, 0.3};
    std::vector<double> predicted_output = gru.forward(input_data_to_predict);

    std::cout << "Predicted Output: " << predicted_output[0] << std::endl;

    return 0;
}
