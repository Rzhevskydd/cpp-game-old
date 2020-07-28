#include "TitleState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>


TitleState::TitleState(StateStack& stack, Context context)
                : State(stack, context), m_show_text(true), m_text_effect_time(sf::Time::Zero) {
        m_background_sprite.setTexture(context.textures->get(Textures::TitleScreen));

        m_text.setFont(context.fonts->get(Fonts::Main));
        m_text.setString("Press any key to start");
        center_origin(m_text);
        m_text.setPosition(sf::Vector2f(context.window->getSize() / 2U));
}

void
TitleState::draw() {
        sf::RenderWindow& window = *get_context().window;
        window.draw(m_background_sprite);

        if (m_show_text) {
                window.draw(m_text);
        }
}

bool
TitleState::update(sf::Time dt) {
        m_text_effect_time += dt;

        if (m_text_effect_time >= sf::seconds(0.5F)) {
                m_show_text        = !m_show_text;
                m_text_effect_time = sf::Time::Zero;
        }

        return true;
}

bool
TitleState::handle_event(const sf::Event& event) {
        // If any key is pressed, trigger the next screen
        if (event.type == sf::Event::KeyReleased) {
                request_stack_pop();
                request_stack_push(States::Menu);
        }

        return true;
}