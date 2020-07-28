#include "Application.hpp"
#include "DeathState.hpp"
#include "GameState.hpp"
#include "MenuState.hpp"
#include "PauseState.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "TitleState.hpp"
#include "Utility.hpp"

#include <SFML/Window/Event.hpp>


const sf::Time Application::time_per_frame = sf::seconds(1.0F / 60.0F);

Application::Application()
                : m_window(sf::VideoMode(1920, 1080), "Game", sf::Style::Close)
                , m_state_stack(State::Context(m_window, m_clock, m_textures, m_fonts, m_user))
                , m_statistics_num_frames(0) {
        m_window.setKeyRepeatEnabled(false);
        m_window.setFramerateLimit(60);  // Limit FPS to avid graphical glitches

        m_fonts.load(Fonts::Main, "media/Sansation.ttf");

        m_textures.load(Textures::TitleScreen, "media/img/menu/TitleScreen" + std::to_string(m_window.getSize().x) + "x" + std::to_string(m_window.getSize().y) + ".png");
        m_textures.load(Textures::ButtonNormal, "media/img/menu/ButtonNormal.png");
        m_textures.load(Textures::ButtonSelected, "media/img/menu/ButtonSelected.png");
        m_textures.load(Textures::ButtonPressed, "media/img/menu/ButtonPressed.png");

        m_statistics_text.setFont(m_fonts.get(Fonts::Main));
        m_statistics_text.setPosition(5.0F, 5.0F);
        m_statistics_text.setCharacterSize(10U);

        register_states();
        m_state_stack.push_state(States::Title);
}

void
Application::run() {
        sf::Clock clock;
        sf::Time  time_since_last_update = sf::Time::Zero;

        while (m_window.isOpen()) {
                sf::Time dt = clock.restart();
                time_since_last_update += dt;
                while (time_since_last_update > time_per_frame) {
                        time_since_last_update -= time_per_frame;

                        process_input();
                        update(time_per_frame);

                        // Check inside this loop, because stack might be empty before update() call
                        if (m_state_stack.is_empty()) {
                                m_window.close();
                        }
                }

                update_statistics(dt);
                render();
        }
}

void
Application::process_input() {
        sf::Event event{};
        while (m_window.pollEvent(event)) {
                m_state_stack.handle_event(event);

                if (event.type == sf::Event::Closed) {
                        m_window.close();
                }
        }
}

void
Application::update(sf::Time dt) {
        m_state_stack.update(dt);
}

void
Application::render() {
        m_window.clear();

        m_state_stack.draw();

        m_window.draw(m_statistics_text);

        m_window.display();
}

void
Application::update_statistics(sf::Time dt) {
        m_statistics_update_time += dt;
        m_statistics_num_frames += 1;
        if (m_statistics_update_time >= sf::seconds(1.0F)) {
                m_statistics_text.setString("FPS: " + std::to_string(m_statistics_num_frames));

                m_statistics_update_time -= sf::seconds(1.0F);
                m_statistics_num_frames = 0;
        }
}

void
Application::register_states() {
        m_state_stack.register_state<TitleState>(States::Title);
        m_state_stack.register_state<MenuState>(States::Menu);
        m_state_stack.register_state<GameState>(States::Game);
        m_state_stack.register_state<PauseState>(States::Pause);
        m_state_stack.register_state<DeathState>(States::Death);
}
