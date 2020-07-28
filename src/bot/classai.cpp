#include "bot/classai.h"

namespace AI {
AI::AI(std::size_t actions_count,
            std::map<int, float> objects_,
            std::size_t input_count,
            std::string filename) :
        network(faai::Neural(input_count, 0.2f)),
        objects(objects_) {
    std::ifstream fin;
    fin.open(filename);
    if (fin) {
        fin.close();
        network = faai::Neural(filename);

    } else {
        network.addLayer(faai::Biased(20, faai::activation::sigmoid));
        network.addLayer(faai::Biased(20, faai::activation::sigmoid));
        network.addLayer(faai::Biased(actions_count, faai::activation::sigmoid));
    }
}

std::vector<float> AI::step(const std::vector<int>& env) {   // matrix nxn => {1, 9, 3, 0 ... input_count}
    std::vector<float> env_tofloat;
    for (std::size_t i = 0; i < env.size(); i++) {
        env_tofloat.push_back(objects[env[i]]);
    }

    prep::Matrix input_vector(env_tofloat, true);
    last_input = input_vector;

    prep::Matrix output_vector = network.query(input_vector);
    last_output = output_vector;

    std::vector<float> output;

    for (std::size_t i = 0; i < output_vector.getRows(); i++) {
        output.push_back(output_vector(i, 0));
    }

    return output;  // {0.1, 0.5, 0.9}
}

void AI::registerResult(const std::vector<int>& env, float result) {
    std::vector<float> env_tofloat;
    for (std::size_t i = 0; i < env.size(); i++) {
        env_tofloat.push_back(objects[env[i]]);
    }

    prep::Matrix input_vector(env_tofloat, true);

    data.push_back({input_vector, network.query(input_vector), result});
}

void AI::trainAll() {
    for (std::size_t i = 0; i < data.size() - 1; i++) {
        prep::Matrix train_vector = data[i].output;
        std::size_t max_index = train_vector.findMaxArg().first;
        train_vector(max_index, 0) = data[i].result;
        network.train(data[i].input, train_vector);
    }
}

void AI::trainNow(float result) {
    prep::Matrix train_vector = last_output;  // prep::Matrix(actions_count, 1, 1.0 - result);
    std::size_t max_index = last_output.findMaxArg().first;
    train_vector(max_index, 0) = result;
    network.train(last_input, train_vector);
}

void AI::save(std::string filename) const {
    network.save(filename);
}
}  // namespace AI


