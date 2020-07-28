#include "bot/networks.h"

namespace faai {
Neural::Neural(std::size_t input_nodes_, float learning_rate_) :
        learning_rate(learning_rate_),
        output_nodes(input_nodes_) {}

Neural::Neural(std::string filename) {
    std::ifstream fin;
    fin.open(filename);
    int layers_count;
    fin >> layers_count;
    fin >> learning_rate;
    std::string layer_name;
    for (int i = 0; i < layers_count; i++) {
        fin >> layer_name;
        if (layer_name == "Dense") {
            Dense layer(fin);
            layers.push_back(std::make_shared<Dense>(std::move(layer)));
            output_nodes = layer.getNodesCount();
        } else if (layer_name == "Biased") {
            Biased layer(fin);
            layers.push_back(std::make_shared<Biased>(std::move(layer)));
            output_nodes = layer.getNodesCount();
        } else if (layer_name == "Recurrent") {
            Recurrent layer(fin);
            layers.push_back(std::make_shared<Recurrent>(std::move(layer)));
            output_nodes = layer.getNodesCount();
        }
    }
}

void Neural::save(std::string filename) const {
    std::ofstream fout;
    fout.open(filename);
    fout << layers.size() << std::endl;
    fout << learning_rate << std::endl;
    for (std::size_t i = 0; i < layers.size(); i++) {
        layers[i]->writeToStream(fout);
    }
    fout.close();
}

void Neural::setLearningRate(float learningrate) {
    learning_rate = learningrate;
}

void Neural::train(const prep::Matrix& input, const prep::Matrix& targets) {
    prep::Matrix layer_output = input;

    for (std::size_t i = 0; i < layers.size(); i++) {
        layer_output = layers[i]->query(layer_output);
    }

    prep::Matrix errors = targets - layer_output;
    for (int i = static_cast<int>(layers.size()) - 1; i >= 0; i--) {
        layers[static_cast<size_t>(i)]->train(errors, learning_rate);
        errors = layers[static_cast<size_t>(i)]->getError(errors);
    }
}

prep::Matrix Neural::query(const prep::Matrix& input) {
    prep::Matrix layer_output = input;

    for (std::size_t i = 0; i < layers.size(); i++) {
        layer_output = layers[i]->query(layer_output);
    }

    return layer_output;
}

float Neural::getWeight(int layer_from, int n_from, int n_to) {
        return layers[static_cast<size_t>(layer_from)]->getWeight()(static_cast<size_t>(n_to), static_cast<size_t>(n_from));
    }

}  // namespace faai

