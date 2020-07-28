#pragma once

#include "bot/classai.h"
#include <vector>
#include <string>

class BOT {
 private:
    std::string name;
    AI::AI ai;

 public:
    explicit BOT(std::string name);

    int step(const std::vector<int>& env);
    void registerResult(const std::vector<int>& env, float award);
    void trainAll();
    void trainNow(float award);
    void save();
};

