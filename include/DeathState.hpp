#ifndef BOOK_DEATHSTATE_HPP
#define BOOK_DEATHSTATE_HPP

#include "Container.hpp"
#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class DeathState : public State {
public:
        DeathState(StateStack& stack, Context context);

        virtual void
        draw();
        virtual bool
        update(sf::Time dt);
        virtual bool
        handle_event(const sf::Event& event);


private:
        sf::Sprite     m_background_sprite;
        sf::Text       m_death_text;
        sf::Text       m_play_time_text;
        gui::Container m_gui_container;
};

#endif  // BOOK_DEATHSTATE_HPP