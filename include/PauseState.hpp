#ifndef BOOK_PAUSESTATE_HPP
#define BOOK_PAUSESTATE_HPP

#include "Container.hpp"
#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class PauseState : public State {
public:
        PauseState(StateStack& stack, Context context);

        void
        draw() override;
        bool
        update(sf::Time time) override;
        bool
        handle_event(const sf::Event& event) override;


private:
        sf::Sprite     m_background_sprite;
        sf::Text       m_paused_text;
        gui::Container m_gui_container;
};

#endif  // BOOK_PAUSESTATE_HPP