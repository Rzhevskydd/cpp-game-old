#include "bot.h"

BOT::BOT(std::string name_) :
                name(name_),
                ai(9, {{0, 0.01}, {1, 0.9}, {2, 0.3}, {3, -0.99}, {4, -0.5}, {5, -0.8}}, 25, name_) {}
// AI(std::size_t actions_count, std::map<int, float> objects, std::size_t input_count, std::string filename);

int BOT::step(const std::vector<int>& env) {
    // This implements the logic for the converting current environment to ai's input vector
    // and transforming ai's output to a relevant form indicating an action.
    float max = -100.0f;
    size_t index = 0;
    std::vector<float> network_output = ai.step(env);
    for (size_t i = 0; i < network_output.size(); i++) {
            //std::cout << network_output[i] << std::endl;
            if (network_output[i] > max) {
                    max = network_output[i];
                    index = i;
            }
    }
    return static_cast<int>(index) + 1;
}

void BOT::registerResult(const std::vector<int>& env, float award) {
    // This implements the logic of holding data
    // corresponding to the last ai's action and it's successfulness over
    // to train with it by the "trainAll" method.
    ai.registerResult(env, award);
}

void BOT::trainAll() {
    // This implements the logic of training
    // with all the data stored by the register result method.
    ai.trainAll();
}

void BOT::trainNow(float award) {
    // This implements the logic of training
    // with data from the last action and the award passed.
    ai.trainNow(award);
}

void BOT::save() {
    // This implements the logic of writting all the required model-related data
    // to the file 'name' (both relative and absolute paths are supported).
    ai.save(name);
}

