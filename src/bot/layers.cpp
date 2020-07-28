#include "bot/layers.h"

namespace faai {
prep::Matrix Layer::function(const prep::Matrix& layer_input) const {
    std::size_t rows = layer_input.getRows();
    std::size_t cols = layer_input.getCols();

    prep::Matrix new_matrix(rows, cols);
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            new_matrix(i, j) = func.func(layer_input(i, j));
        }
    }

    return new_matrix;
}

prep::Matrix Layer::derivative(const prep::Matrix& layer_input) const {
    std::size_t rows = layer_input.getRows();
    std::size_t cols = layer_input.getCols();

    prep::Matrix new_matrix(rows, cols);
    for (std::size_t i = 0; i < rows; i++) {
        for (std::size_t j = 0; j < cols; j++) {
            new_matrix(i, j) = func.derivative_func(layer_input(i, j));
        }
    }

    return new_matrix;
}

Layer::Layer(std::size_t nodes_count_, activation::float_func funcs) :
        nodes_count(nodes_count_),
        func(funcs) {}

Layer::Layer(std::istream& stream) {
    std::string func_name;
    stream >> func_name;
    func = activation::function_dict[func_name];
    weight = prep::Matrix(stream);
    nodes_count = weight.getRows();
}

prep::Matrix Layer::getWeight() const {
    return weight;
}

std::size_t Layer::getNodesCount() const {
    return nodes_count;
}

prep::Matrix Layer::getError(const prep::Matrix& error) {
    return weight.transp() * error;
}

void Layer::createWeights(std::size_t back_layernodes) {
    weight = prep::Matrix(nodes_count, back_layernodes, 0.0, static_cast<float >(std::pow(nodes_count, -0.5)));
}

Dense::Dense(std::size_t nodes_count_, activation::float_func funcs) :
        Layer(nodes_count_, funcs) {}

Dense::Dense(std::istream& stream) : Layer(stream) {}

void Dense::writeToStream(std::ostream& stream) const {
    stream << "Dense" << std::endl;
    stream << func.name << std::endl;
    stream << weight;
}

void Dense::train(const prep::Matrix& error, float learning_rate) {
    weight = weight +
            learning_rate *
            (error %
            derivative(input)) *
            back_layerout.transp();
}

const prep::Matrix& Dense::query(const prep::Matrix& back_layeroutput) {
    back_layerout = back_layeroutput;
    input = weight * back_layeroutput;
    output = function(input);

    return output;
}


Biased::Biased(std::size_t nodes_count_, activation::float_func funcs) :
        Layer(nodes_count_, funcs),
        bias_weight(prep::Matrix(nodes_count_, 1, 0.0, static_cast<float >(std::pow(nodes_count_, -0.5)))) {}

Biased::Biased(std::istream& stream) :
        Layer(stream),
        bias_weight(prep::Matrix(stream)) {}

void Biased::writeToStream(std::ostream& stream) const {
    stream << "Biased" << std::endl;
    stream << func.name << std::endl;
    stream << weight;
    stream << bias_weight;
}

void Biased::train(const prep::Matrix& error, float learning_rate) {
    prep::Matrix input_derivative = derivative(input);

    weight = weight +
            learning_rate *
            (error %
            input_derivative) *
            back_layerout.transp();

    bias_weight = bias_weight +
                 learning_rate *
                 (error %
                 input_derivative);
}

const prep::Matrix& Biased::query(const prep::Matrix& back_layeroutput) {
    back_layerout = back_layeroutput;
    input = weight * back_layeroutput + bias_weight;
    output = function(input);

    return output;
}


Recurrent::Recurrent(std::size_t nodes_count_, activation::float_func funcs) :
        Layer(nodes_count_, funcs),
        rnn_weight(prep::Matrix(nodes_count_, 1, 0.0, static_cast<float >(std::pow(nodes_count, -0.5)))) {
    output = prep::Matrix(nodes_count_, 1, 0.0);
}

Recurrent::Recurrent(std::istream& stream) :
        Layer(stream),
        rnn_weight(prep::Matrix(stream)) {
    output = prep::Matrix(stream);
}

void Recurrent::writeToStream(std::ostream& stream) const {
    stream << "Recurrent" << std::endl;
    stream << func.name << std::endl;
    stream << weight;
    stream << output;
    stream << rnn_weight;
}

void Recurrent::train(const prep::Matrix& error, float learning_rate) {
    weight = weight +
            learning_rate *
            (error %
            derivative(input)) *
            back_layerout.transp();

    rnn_weight = rnn_weight +
                learning_rate *
                (error %
                derivative(rnn_weight % previous_output)) %
                previous_output;
}

const prep::Matrix& Recurrent::query(const prep::Matrix& back_layeroutput) {
    previous_output = output;
    back_layerout = back_layeroutput;
    input = weight * back_layeroutput + previous_output % rnn_weight;
    output = function(input);

    return output;
}
}  // namespace faai

