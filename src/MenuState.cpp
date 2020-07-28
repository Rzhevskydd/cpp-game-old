#include "MenuState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>


MenuState::MenuState(StateStack& stack, Context context) : State{ stack, context }, m_option_index{ 0 } {
        sf::Texture& texture = context.textures->get(Textures::TitleScreen);
        sf::Font&    font    = context.fonts->get(Fonts::Main);

        m_background_sprite.setTexture(texture);

        // A simple menu demonstration
        sf::Text play_option;
        play_option.setFont(font);
        play_option.setString("Play");
        center_origin(play_option);
        play_option.setPosition(context.window->getView().getSize() / 2.0F);
        m_options.push_back(play_option);

        sf::Text exit_option;
        exit_option.setFont(font);
        exit_option.setString("Exit");
        center_origin(exit_option);
        exit_option.setPosition(play_option.getPosition() + sf::Vector2f(0.0F, 30.0F));
        m_options.push_back(exit_option);

        update_option_text();
}

void
MenuState::draw() {
        sf::RenderWindow& window = *get_context().window;

        window.setView(window.getDefaultView());
        window.draw(m_background_sprite);

        for (const sf::Text& text : m_options) {
                window.draw(text);
        }
}

bool
MenuState::update(sf::Time /*dt*/) {
        return true;
}

bool
MenuState::handle_event(const sf::Event& event) {
        sf::Clock& game_clock = *get_context().clock;

        // The demonstration menu logic
        if (event.type != sf::Event::KeyPressed) {
                return false;
        }

        if (event.key.code == sf::Keyboard::Return) {
                if (m_option_index == Play) {
                        request_stack_pop();
                        request_stack_push(States::Game);
                        game_clock.restart();
                } else if (m_option_index == Exit) {
                        // The exit option was chosen, by removing itself, the stack will be empty, and the game will
                        // know it is time to close.
                        request_stack_pop();
                }
        }

        else if (event.key.code == sf::Keyboard::Up) {
                // Decrement and wrap-around
                if (m_option_index > 0) {
                        m_option_index--;
                } else {
                        m_option_index = m_options.size() - 1;
                }

                update_option_text();
        }

        else if (event.key.code == sf::Keyboard::Down) {
                // Increment and wrap-around
                if (m_option_index < m_options.size() - 1) {
                        m_option_index++;
                } else {
                        m_option_index = 0;
                }

                update_option_text();
        }

        return true;
}

void
MenuState::update_option_text() {
        if (m_options.empty()) {
                return;
        }

        // White all texts
        for (sf::Text& text : m_options) {
                text.setFillColor(sf::Color::White);
        }

        // Red the selected text
        m_options[m_option_index].setFillColor(sf::Color::Red);
}
