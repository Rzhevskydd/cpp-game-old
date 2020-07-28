#pragma once

#include <map>
#include <cmath>
#include <utility>
#include <string>
#include <functional>

namespace faai {
namespace activation {

namespace {
float sigmoid_func(float x) {
    return 1.0f / (1.0f + std::exp(-x));
}

float derivativeSigmoid(float x) {
    return sigmoid_func(x) * (1.0f- sigmoid_func(x));
}

float th_func(float x) {
    return (std::exp(x) - std::exp(-x))/(std::exp(x) + std::exp(-x));
}

float derivativeTh(float x) {
    return (1.0F- std::pow(th_func(x), 2.0F));
}

float relu_func(float x) {
    return x <= 0.0f ? 0.0f : x;
}

float derivativeRelu(float x) {
    return x <= 0.0f ? 0.0f : 1.0f;
}

float identity_func(float x) {
    return x;
}

float derivativeIdentity(float x) {
    x += 1;
    return 1.0;
}
}  // namespace

struct float_func {
    std::string name;
    std::function<float(float)> func;
    std::function<float(float)> derivative_func;
};

static float_func sigmoid  = {"sigmoid", sigmoid_func, derivativeSigmoid};
static float_func th = {"th", th_func, derivativeTh};
static float_func relu = {"relu", relu_func, derivativeRelu};
static float_func identity = {"identity", identity_func, derivativeIdentity};

static std::map<std::string, float_func> function_dict = {{"sigmoid", sigmoid},
                                                          {"th", th},
                                                          {"relu", relu},
                                                          {"identity", identity}};

}  // namespace activation
}  // namespace faai

