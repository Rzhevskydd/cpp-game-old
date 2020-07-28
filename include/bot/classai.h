#pragma once

#include <map>
#include <vector>
#include <string>
#include "networks.h"
#include "layers.h"

namespace AI {
struct NetworkData {
    prep::Matrix input;
    prep::Matrix output;
    float result;
};

class AI {
 private:
    faai::Neural network;
    std::map<int, float> objects;
    std::vector<NetworkData> data;
    prep::Matrix last_output;
    prep::Matrix last_input;

 public:
    AI(std::size_t actions_count,
        std::map<int, float> objects,
        std::size_t input_count,
        std::string filename);

    std::vector<float> step(const std::vector<int>& env);
    void registerResult(const std::vector<int>& env, float result);
    void trainAll();
    void trainNow(float result);

    void save(std::string filename) const;
};
}  // namespace AI
