#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <memory>
#include "matrix.h"
#include "layers.h"

namespace faai {
class Neural {
 private:
    float learning_rate;
    std::size_t output_nodes;
    std::vector<std::shared_ptr<ILayer>> layers;

 public:
    Neural(std::size_t input_nodes, float learning_rate);
    explicit Neural(std::string filename);

    template<class TLayer>
    void addLayer(TLayer layer);
    void save(std::string filename) const;
    void setLearningRate(float learningrate);
    prep::Matrix query(const prep::Matrix& input);
    void train(const prep::Matrix& input, const prep::Matrix& target);
    float getWeight(int layer_from, int n_from, int n_to);
};

template<class TLayer>
void Neural::addLayer(TLayer layer) {
    layer.createWeights(output_nodes);
    layers.push_back(std::make_shared<TLayer>(std::move(layer)));
    output_nodes = layer.getNodesCount();
}
}  // namespace faai

