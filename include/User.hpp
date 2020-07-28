#ifndef BOOK_PLAYER_HPP
#define BOOK_PLAYER_HPP

#include "Command.hpp"

#include <SFML/Window/Keyboard.hpp>

#include <map>


class CommandQueue;

namespace sf {
        class Event;
}

class User {
public:
        enum Action {
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


public:
        User();

        void
        handle_event(const sf::Event& event, CommandQueue& commands);

        void
        handle_realtime_input(CommandQueue& commands);

        void
        assign_key(Action action, sf::Keyboard::Key key);


        sf::Keyboard::Key
        get_assigned_key(Action action) const;

private:
        void
        initialize_actions();

        static bool
        is_realtime_action(Action action);


private:
        std::map<sf::Keyboard::Key, Action> m_key_binding;
        std::map<Action, Command>           m_action_binding;
};

#endif  // BOOK_PLAYER_HPP
