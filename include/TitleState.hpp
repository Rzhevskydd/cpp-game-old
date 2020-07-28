#ifndef BOOK_TITLESTATE_HPP
#define BOOK_TITLESTATE_HPP

#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class TitleState : public State {
public:
        TitleState(StateStack& stack, Context context);

        virtual void
        draw();
        virtual bool
        update(sf::Time dt);
        virtual bool
        handle_event(const sf::Event& event);


private:
        sf::Sprite m_background_sprite;
        sf::Text   m_text;

        bool     m_show_text;
        sf::Time m_text_effect_time;
};

#endif  // BOOK_TITLESTATE_HPP
