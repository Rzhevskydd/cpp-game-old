//
// Created by adminu on 03.12.2019.
//

#ifndef GAME_LEVELMANAGER_HPP
#define GAME_LEVELMANAGER_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <tinyxml2.h>

#include <memory>

class SceneNode;

namespace sf {
        class View;
}

class LevelManager {
public:
        LevelManager(SceneNode& scene_node, const sf::View& world_view);

        ///Размеры мира
        sf::Vector2f
        get_level_size() const;

private:
        /// Загружает в ноду сцены все необходимое для работы с с картой
        bool
        init_level(const char* level_file_path);

        /// Парсит размеры тайлов и размеры карты
        void
        parse_map_info(tinyxml2::XMLElement* root_element);

        /// Делает все необходимое чтобы распарсить тайлсеты и коллекции изображений,
        /// загрузить и нарезать текстуры и инициализировать m_gids_vect в нужном порядке
        void
        parse_and_load_level_graphic_data(tinyxml2::XMLElement* root_element);

        /// Парсит, загружает текстуру и пихает в вектор спрайт коллекций изображений
        void
        parse_and_load_img_collection(tinyxml2::XMLElement* root_element);

        /// Парсит, загружает текстуру, нарезает текстуру и пихает в вектор элементы тайлсета
        void
        parse_and_load_tileset(tinyxml2::XMLElement* root_element);

        static std::string
        get_file_name(std::string raw_str, const char* prefix_path);

        /// Парсим тайловый слой и создаем соответствующий узел
        void
        parse_and_create_tile_layout(tinyxml2::XMLElement* root_element);

        /// Парсим объекты на "поверхности" и добавляет в сцену (в собственный узел).
        void
        parse_and_create_level_objects(tinyxml2::XMLElement* root_element);

        /// Парсим и создаем слой с bounding box'ами и закидываем в сцену.
        void
        parse_and_create_bounding_box_layout(tinyxml2::XMLElement* root_element);

        SceneNode*
        getLevelObjectsNode();

        SceneNode& m_scene_graph;
        SceneNode* m_level_object_node{};


        int m_level_width{};   // в тайлах
        int m_level_height{};  // в тайлах
        int m_tile_width{};
        int m_tile_height{};

        std::vector<sf::Sprite>                   m_gids_vect;
        std::vector<std::unique_ptr<sf::Texture>> m_texture_store;  // вместо texture holder'a
        const sf::View& m_world_view;  // храним что бы передавать в LevelObject.
};


#endif  // GAME_LEVELMANAGER_HPP
