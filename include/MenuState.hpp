#ifndef BOOK_MENUSTATE_HPP
#define BOOK_MENUSTATE_HPP

#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>

class MenuState : public State {
public:
        MenuState(StateStack& stack, Context context);

        virtual void
        draw();
        virtual bool
        update(sf::Time dt);
        virtual bool
        handle_event(const sf::Event& event);

        void
        update_option_text();


private:
        enum OptionNames {
                Play,
                Exit,
        };


private:
        sf::Sprite m_background_sprite;

        std::vector<sf::Text> m_options;
        size_t                m_option_index;
};

#endif  // BOOK_MENUSTATE_HPP
