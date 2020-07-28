#ifndef BOOK_GAME_HPP
#define BOOK_GAME_HPP

#include "User.hpp"
#include "World.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>


class Game {
public:
        Game();

        void
        run();

        Game(const Game&) = delete;
        Game&
        operator=(const Game&)
                        = delete;

private:
        void
        process_input();

        void
        update(sf::Time elapsed);

        void
        render();

        void
        update_statistics(sf::Time elapsed);


private:
        static const sf::Time time_per_frame;

        sf::RenderWindow m_window;
        World            m_world;
        User             m_player;

        sf::Font m_font;
        sf::Text m_statistics_text;
        sf::Time m_statistics_update_time;
        size_t   m_statistics_num_frames;
};

#endif  // BOOK_GAME_HPP
