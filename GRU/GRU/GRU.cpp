#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class GRU {
public:
    int input_size;
    int hidden_size;

    vector<vector<double>> Wz;
    vector<vector<double>> Wr;
    vector<vector<double>> Wh;
    vector<double> bz;
    vector<double> br;
    vector<double> bh;

    GRU(int input_size, int hidden_size) {
        this->input_size = input_size;
        this->hidden_size = hidden_size;

        // Initialize weights and biases
        for (int i = 0; i < hidden_size; i++) {
            for (int j = 0; j < input_size; j++) {
                Wz[i][j] = (double)rand() / RAND_MAX;
                Wr[i][j] = (double)rand() / RAND_MAX;
                Wh[i][j] = (double)rand() / RAND_MAX;
            }
            bz[i] = (double)rand() / RAND_MAX;
            br[i] = (double)rand() / RAND_MAX;
            bh[i] = (double)rand() / RAND_MAX;
        }
    }

    vector<double> forward(vector<double> x) {
        vector<double> z_t(hidden_size);
        vector<double> r_t(hidden_size);
        vector<double> h_t(hidden_size);

        // Compute gates
        for (int i = 0; i < hidden_size; i++) {
            z_t[i] = sigmoid(bz[i] + dot(Wz[i], x) + dot(Wz[i], h_t));
            r_t[i] = sigmoid(br[i] + dot(Wr[i], x) + dot(Wr[i], h_t));
            double inner = 0.0;
            for (int j = 0; j < input_size; j++) {
                inner += Wh[i][j] * r_t[i] * x[j];
            }
            h_t[i] = z_t[i] * h_t[i] + (1 - z_t[i]) * tanh(bh[i] + inner);
        }

        return h_t;
    }

    void backward(vector<double> x, vector<double> target, double learning_rate) {
        vector<double> z_t(hidden_size);
        vector<double> r_t(hidden_size);
        vector<double> h_t(hidden_size);

        // Compute gates
        for (int i = 0; i < hidden_size; i++) {
            z_t[i] = sigmoid(bz[i] + dot(Wz[i], x) + dot(Wz[i], h_t));
            r_t[i] = sigmoid(br[i] + dot(Wr[i], x) + dot(Wr[i], h_t));
            double inner = 0.0;
            for (int j = 0; j < input_size; j++) {
                inner += Wh[i][j] * r_t[i] * x[j];
            }
            h_t[i] = z_t[i] * h_t[i] + (1 - z_t[i]) * tanh(bh[i] + inner);
        }

        // Compute gradients
        vector<vector<double>> dz(hidden_size, vector<double>(input_size));
        vector<vector<double>> dr(hidden_size, vector<double>(input_size));
        vector<vector<double>> dWh(hidden_size, vector<double>(input_size));
        vector<vector<double>> dWz(hidden_size, vector<double>(input_size));
        vector<vector<double>> dWr(hidden_size, vector<double>(input_size));
        vector<vector<double>> dBr(hidden_size, vector<double>(input_size));
        vector<vector<double>> dBz(hidden_size, vector<double>(input_size));
        vector<vector<double>> dBr(hidden_size, vector<double>(input_size));
        vector<double> dh_next(hidden_size);

        for (int i = 0; i < hidden_size; i++) {
            dz[i][i] = 1.0;
            dr[i][i] = 1.0;
            for (int j = 0; j < input_size; j++) {
                dz[i][j] = dot(dz[i], Wz[i]);
                dr[i][j] = dot(dr[i], Wr[i]);
                dWh[i][j] = dot(r_t[i] * dz[i], x);
                dWz[i][j] = dot(z_t[i] * dz[i], x);
                dWr[i][j] = dot(r_t[i] * dr[i], x);
                dBr[i][j] = dot(r_t[i] * dr[i], x);
            }
            double dh = 0.0;
            for (int j = 0; j < hidden_size; j++) {
                dh += dz[j] * Wh[i][j];
            }
            dh_next[i] = dh * (1 - z_t[i]) * (1 - h_t[i] * h_t[i]);
        }

        // Update weights and biases
        for (int i = 0; i < hidden_size; i++) {
            for (int j = 0; j < input_size; j++) {
                Wz[i][j] -= learning_rate * dWz[i][j];
                Wr[i][j] -= learning_rate * dWr[i][j];
                Wh[i][j] -= learning_rate * dWh[i][j];
            }
            bz[i] -= learning_rate * dz[i][i];
            br[i] -= learning_rate * dr[i][i];
            bh[i] -= learning_rate * dh_next[i];
        }
    }

private:
    double dot(vector<double> a, vector<double> b) {
        double result = 0.0;
        for (int i = 0; i < a.size(); i++) {
            result += a[i] * b[i];
        }
        return result;
    }

    double sigmoid(double x) {
        return 1.0 / (1.0 + exp(-x));
    }
};

int main() {
    int input_size = 3;
    int hidden_size = 16;
    int sequence_length = 41;

    GRU gru(input_size, hidden_size);

    vector<vector<double>> input_data;
    vector<double> target_data;

    double value;
    for (int i = 0; i < sequence_length; i++) {
        vector<double> row;
        for (int j = 0; j < input_size + 1; j++) {
            cin >> value;
            row.push_back(value);
        }
        input_data.push_back(row);
        cin >> value;
        target_data.push_back(value);
    }

    double learning_rate = 0.01;

    for (int epoch = 0; epoch < 1000; epoch++) {
        double loss = 0.0;
        for (int t = 0; t < sequence_length; t++) {
            vector<double> x = input_data[t];
            vector<double> h = gru.forward(x);
            double error = 0.0;
            for (int i = 0; i
