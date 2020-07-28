#ifndef BOOK_COMMAND_HPP
#define BOOK_COMMAND_HPP

#include "Category.hpp"

#include <SFML/System/Time.hpp>

#include <functional>
#include <stdexcept>


class SceneNode;

namespace Commands {
        enum ID {
                MoveLeft,  // clang-format break
                MoveRight,
                MoveUp,
                MoveDown,
                FireLeft,
                FireRight,
                FireUp,
                FireDown,
                ActionCount
        };
}

struct Command {
        // TODO check if sf::Time is needed
        std::function<void(SceneNode&, sf::Time)> action;
        unsigned                                  category{ Category::None };
};

// TODO rename?
template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)>
derived_action(Function&& fn) {
        return [fn = std::forward<Function>(fn)](SceneNode& node, sf::Time dt) {
                GameObject* go_node{ dynamic_cast<GameObject*>(&node) };
                // Check if cast succeeded
                if (!go_node) {
                        throw std::runtime_error{ "derived_action: cast failed" };
                }

                fn(*go_node, dt);
        };
}

#endif  // BOOK_COMMAND_HPP
