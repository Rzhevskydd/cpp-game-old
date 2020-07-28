#ifndef BOOK_APPLICATION_HPP
#define BOOK_APPLICATION_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "StateStack.hpp"
#include "User.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>


class Application {
public:
        Application();
        void
        run();

private:
        void
        process_input();
        void
        update(sf::Time dt);
        void
        render();

        void
        update_statistics(sf::Time dt);
        void
        register_states();


private:
        static const sf::Time time_per_frame;

        sf::RenderWindow m_window;
        sf::Clock        m_clock;
        TextureHolder    m_textures;
        FontHolder       m_fonts;
        User             m_user;

        StateStack m_state_stack;

        sf::Text m_statistics_text;
        sf::Time m_statistics_update_time;
        size_t   m_statistics_num_frames;
};

#endif  // BOOK_APPLICATION_HPP
