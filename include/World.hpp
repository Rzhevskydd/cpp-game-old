#ifndef BOOK_WORLD_HPP
#define BOOK_WORLD_HPP

#include "Command.hpp"
#include "CommandQueue.hpp"
#include "ForegroundManager.hpp"
#include "LevelManager.hpp"
#include "Person.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>

#include <array>
#include <queue>


// Forward declaration
namespace sf {
        class RenderWindow;
}

class World {
public:
        explicit World(sf::RenderWindow& window);

        World(const World&) = delete;
        World&
        operator=(const World&)
                        = delete;

        void
        update(sf::Time dt);
        void
        draw();

        // TODO overload
        CommandQueue&
        get_command_queue();

        bool
        is_player_dead();

private:
        void
        load_textures();
        void
        build_scene();
        void
        adapt_player_position();
        void
        adapt_enemy_position(Person* enemy);
        void
        adapt_player_velocity();
        void
        handle_collisions();


        void
        add_enemies();
        void
        add_enemy(Person::Type type, float rel_x, float rel_y);
        void
        spawn_enemies();

        sf::FloatRect
        get_view_bounds() const;

        // Return an area, in which enemies start to fight
        sf::FloatRect
        get_battlefield_bounds() const;

        sf::FloatRect
        get_battlefield_bounds_2x() const;

        sf::FloatRect
        get_battlefield_bounds(Person* enemy) const;

        std::vector<int>
        get_enviroment(Person* enemy, size_t n);

private:
        struct SpawnPoint {
                SpawnPoint(Person::Type t_type, float t_x, float t_y) : type{ t_type }, x{ t_x }, y{ t_y } {};
                Person::Type type;
                float        x;
                float        y;
        };

        sf::RenderWindow& m_window;
        sf::View          m_world_view;
        TextureHolder     m_textures;
        FontHolder        m_fonts;
        SceneNode         m_scene_graph;

        LevelManager m_level_manager;
        CommandQueue m_command_queue;

        sf::FloatRect m_world_bounds;
        sf::Vector2f  m_spawn_position;
        Person*       m_player;

        std::vector<SpawnPoint> m_enemy_spawn_points;
        std::vector<Person*>    m_active_enemies;
        size_t                  m_number_of_enemies;

        std::vector<SceneNode*> m_game_objects;
        ForegroundManager       m_foreground_manager;

        std::map<Commands::ID, Command> m_commands;
        sf::Time                        m_countdown;

        unsigned  m_wave_number;
        TextNode* m_wave_display;
        bool      m_is_wavetext_displayed;
        sf::Time  m_wavetext_countdown;

        unsigned  m_living_enemies_counter;
        TextNode* m_enemies_counter_display;
};

#endif  // BOOK_WORLD_HPP
