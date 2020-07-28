#include "PauseState.hpp"
#include "Button.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


PauseState::PauseState(StateStack& stack, Context context) : State(stack, context) {
        sf::Font&    font = context.fonts->get(Fonts::Main);
        sf::Vector2f window_size(context.window->getSize());

        m_paused_text.setFont(font);
        m_paused_text.setString("Game Paused");
        m_paused_text.setCharacterSize(70);
        center_origin(m_paused_text);
        m_paused_text.setPosition(0.5F * window_size.x, 0.4F * window_size.y);

        auto return_button = std::make_shared<gui::Button>(*context.fonts, *context.textures);
        return_button->setPosition(0.5F * window_size.x - 100, 0.4F * window_size.y + 75);
        return_button->set_text("Return");
        return_button->set_callback([this]() {
                request_stack_pop();
        });

        auto back_to_menu_button = std::make_shared<gui::Button>(*context.fonts, *context.textures);
        back_to_menu_button->setPosition(0.5F * window_size.x - 100, 0.4F * window_size.y + 125);
        back_to_menu_button->set_text("Back to menu");
        back_to_menu_button->set_callback([this]() {
                request_state_clear();
                request_stack_push(States::Menu);
        });

        m_gui_container.pack(return_button);
        m_gui_container.pack(back_to_menu_button);
}

void
PauseState::draw() {
        sf::RenderWindow& window = *get_context().window;
        window.setView(window.getDefaultView());

        sf::RectangleShape background_shape;
        background_shape.setFillColor(sf::Color(0, 0, 0, 150));
        background_shape.setSize(window.getView().getSize());

        window.draw(background_shape);
        window.draw(m_paused_text);
        window.draw(m_gui_container);
}

bool
PauseState::update([[maybe_unused]] sf::Time time) {
        return false;
}

bool
PauseState::handle_event(const sf::Event& event) {
        m_gui_container.handle_event(event);
        return false;
}