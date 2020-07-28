#include "Game.hpp"

#include <SFML/Window/Event.hpp>

const sf::Time Game::time_per_frame = sf::seconds(1.0F / 60.0F);

Game::Game()
                : m_window{ sf::VideoMode{ 1980, 1024 }, "Test-map", sf::Style::Fullscreen }
                , m_world{ m_window }
                , m_statistics_num_frames{ 0 } {
        m_window.setKeyRepeatEnabled(false);

        // TODO(Artöm Bakri Al-Sarmini): check return
        m_font.loadFromFile("media/Sansation.ttf");
        m_statistics_text.setFont(m_font);
        m_statistics_text.setPosition(5.0F, 5.0F);
        m_statistics_text.setCharacterSize(10);
}

void
Game::run() {
        sf::Clock clock;
        sf::Time  time_since_last_update = sf::Time::Zero;
        while (m_window.isOpen()) {
                sf::Time elapsed_time = clock.restart();
                time_since_last_update += elapsed_time;
                while (time_since_last_update > time_per_frame) {
                        time_since_last_update -= time_per_frame;

                        process_input();
                        update(time_per_frame);
                }

                update_statistics(elapsed_time);
                render();
        }
}

void
Game::process_input() {
        CommandQueue& commands = m_world.get_command_queue();

        sf::Event event{};
        while (m_window.pollEvent(event)) {
                m_player.handle_event(event, commands);

                if (event.type == sf::Event::Closed) {
                        m_window.close();
                }
        }

        m_player.handle_realtime_input(commands);
}

void
Game::update(sf::Time elapsed) {
        m_world.update(elapsed);
}

void
Game::render() {
        m_window.clear();
        m_world.draw();

        //	mWindow.setView(mWindow.getDefaultView());
        m_window.draw(m_statistics_text);
        m_window.display();
}

void
Game::update_statistics(sf::Time elapsed) {
        m_statistics_update_time += elapsed;
        m_statistics_num_frames += 1;

        if (m_statistics_update_time >= sf::seconds(1.0F)) {
                m_statistics_text.setString("Frames / Second = " + std::to_string(m_statistics_num_frames)
                                            + "\nTime / Update = "
                                            + std::to_string(m_statistics_update_time.asMicroseconds()
                                                             / static_cast<sf::Int64>(m_statistics_num_frames))
                                            + "us");

                m_statistics_update_time -= sf::seconds(1.0F);
                m_statistics_num_frames = 0;
        }
}
