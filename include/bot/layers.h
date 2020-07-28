#pragma once

#include <utility>
#include <map>
#include "matrix.h"
#include "functions.h"

namespace faai {

class ILayer {
 public:
    virtual prep::Matrix getWeight() const = 0;
    virtual std::size_t getNodesCount() const = 0;
    virtual void createWeights(std::size_t back_layernodes) = 0;
    virtual prep::Matrix getError(const prep::Matrix& error) = 0;

    virtual void writeToStream(std::ostream& stream) const = 0;
    virtual void train(const prep::Matrix& error, float learning_rate) = 0;
    virtual const prep::Matrix& query(const prep::Matrix& back_layeroutput) = 0;
//    virtual ~ILayer() = 0;
};


class Layer : public ILayer {
 protected:
    std::size_t nodes_count;
    prep::Matrix weight;
    prep::Matrix input;
    prep::Matrix output;
    prep::Matrix back_layerout;
    activation::float_func func;

    virtual prep::Matrix function(const prep::Matrix& layer_input) const;
    virtual prep::Matrix derivative(const prep::Matrix& layer_input) const;

 public:
    Layer(std::size_t nodes_count, activation::float_func funcs);
    explicit Layer(std::istream& stream);

    prep::Matrix getWeight() const override;
    std::size_t getNodesCount() const override;
    void createWeights(std::size_t back_layernodes) override;
    prep::Matrix getError(const prep::Matrix& error) override;
//    ~Layer() = default;
};


class Dense final : public Layer {
 public:
    Dense(std::size_t nodes_count, activation::float_func funcs);
    explicit Dense(std::istream& stream);

    void writeToStream(std::ostream& stream) const override;
    void train(const prep::Matrix& error, float learning_rate) override;
    const prep::Matrix& query(const prep::Matrix& back_layeroutput) override;
//    ~Dense() = default;

};


class Biased final : public Layer {
 private:
    prep::Matrix bias_weight;

 public:
    Biased(std::size_t nodes_count, activation::float_func funcs);
    explicit Biased(std::istream& stream);

    void writeToStream(std::ostream& stream) const override;
    void train(const prep::Matrix& error, float learning_rate) override;
    const prep::Matrix& query(const prep::Matrix& back_layeroutput) override;
//    ~Biased() = default;
};


class Recurrent final : public Layer {
 private:
    prep::Matrix previous_output;
    prep::Matrix rnn_weight;

 public:
    Recurrent(std::size_t nodes_count, activation::float_func funcs);
    explicit Recurrent(std::istream& stream);

    void writeToStream(std::ostream& stream) const override;
    void train(const prep::Matrix& error, float learning_rate) override;
    const prep::Matrix& query(const prep::Matrix& back_layeroutput) override;
//    ~Recurrent() = default;
};
}  // namespace faai


