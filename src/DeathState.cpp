#include <Button.hpp>
#include <DeathState.hpp>
#include <ResourceHolder.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <cmath>

DeathState::DeathState(StateStack& stack, Context context) : State(stack, context) {
        sf::Font&    font = context.fonts->get(Fonts::Main);
        sf::Vector2f window_size(context.window->getSize());
        sf::Time     play_time = context.clock->getElapsedTime();

        m_death_text.setFont(font);
        m_death_text.setString("Game Over");
        m_death_text.setCharacterSize(70);
        center_origin(m_death_text);
        m_death_text.setPosition(0.5F * window_size.x, 0.4F * window_size.y);

        m_play_time_text.setFont(font);
        m_play_time_text.setCharacterSize(24);
        m_play_time_text.setString("You survived for "
                                   + std::to_string(static_cast<int>(std::round(play_time.asSeconds()))) + " seconds");
        m_play_time_text.setPosition(0.5F * window_size.x - 155, 0.4F * window_size.y + 65);

        auto back_to_menu_button = std::make_shared<gui::Button>(*context.fonts, *context.textures);
        back_to_menu_button->setPosition(0.5F * window_size.x - 100, 0.4F * window_size.y + 140);
        back_to_menu_button->set_text("Back to menu");
        back_to_menu_button->set_callback([this]() {
                request_state_clear();
                request_stack_push(States::Menu);
        });

        m_gui_container.pack(back_to_menu_button);
}

void
DeathState::draw() {
        sf::RenderWindow& window = *get_context().window;
        window.setView(window.getDefaultView());

        sf::RectangleShape background_shape;
        background_shape.setFillColor(sf::Color(0, 0, 0, 150));
        background_shape.setSize(window.getView().getSize());

        window.draw(background_shape);
        window.draw(m_death_text);
        window.draw(m_play_time_text);
        window.draw(m_gui_container);
}

bool
DeathState::update(sf::Time /*dt*/) {
        return false;
}

bool
DeathState::handle_event(const sf::Event& event) {
        m_gui_container.handle_event(event);
        return false;
}