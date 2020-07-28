#ifndef BOOK_GAMESTATE_HPP
#define BOOK_GAMESTATE_HPP

#include "State.hpp"
#include "World.hpp"

#include <SFML/System/Time.hpp>


class User;

namespace sf {
        class Event;
}

class GameState : public State {
public:
        GameState(StateStack& stack, Context context);

        virtual void
        draw();
        virtual bool
        update(sf::Time dt);
        virtual bool
        handle_event(const sf::Event& event);


private:
        World m_world;
        User& m_user;
};

#endif  // BOOK_GAMESTATE_HPP