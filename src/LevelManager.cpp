//
// Created by adminu on 03.12.2019.
//

#include "LevelManager.hpp"
#include "BoundingBox.hpp"
#include "LevelObject.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "TileLayout.hpp"

#include "BrdDebugConfig.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/View.hpp>

#include <cstddef>

constexpr auto map_path{ "./media/xml/map/map_ex2.tmx" };
constexpr auto tilesets_dir{ "./media/textures/tilesets" };
constexpr auto imgs_dir{ "./media/textures/img" };


LevelManager::LevelManager(SceneNode& scene_node, const sf::View& world_view)
                : m_scene_graph(scene_node), m_world_view(world_view) {
        init_level(map_path);
}

bool
LevelManager::init_level(const char* level_file_path) {

        // Загружаем xml уровня
        tinyxml2::XMLDocument level_xml;
        if (level_xml.LoadFile(level_file_path) != tinyxml2::XML_SUCCESS) {
                using namespace std::string_literals;
                throw std::runtime_error{ "Loading level \""s + level_file_path + "\" failed." };
        }

        auto map_tag = level_xml.FirstChildElement("map");
        parse_map_info(map_tag);
        parse_and_load_level_graphic_data(map_tag);
        parse_and_create_tile_layout(map_tag->FirstChildElement("layer"));
        parse_and_create_level_objects(map_tag->FirstChildElement("objectgroup"));
        parse_and_create_bounding_box_layout(
                        map_tag->FirstChildElement("objectgroup")->NextSiblingElement("objectgroup"));
        return true;
}

void
LevelManager::parse_map_info(tinyxml2::XMLElement* root_element) {
        m_level_width  = atoi(root_element->Attribute("width"));
        m_level_height = atoi(root_element->Attribute("height"));
        m_tile_width   = atoi(root_element->Attribute("tilewidth"));
        m_tile_height  = atoi(root_element->Attribute("tileheight"));
}

void
LevelManager::parse_and_load_level_graphic_data(tinyxml2::XMLElement* root_element) {
        tinyxml2::XMLElement* tileset_tag{ root_element->FirstChildElement("tileset") };
        if (!tileset_tag) {
                throw std::runtime_error{
                        "LevelManager::parse_and_load_level_graphic_data(tinyxml2::XMLElement): tileset not found"
                };
        }

        while (tileset_tag) {
                if (strcmp(tileset_tag->FirstChildElement()->Name(), "grid") == 0) {
                        parse_and_load_img_collection(tileset_tag);
                } else {
                        parse_and_load_tileset(tileset_tag);
                }
                tileset_tag = tileset_tag->NextSiblingElement("tileset");
        }
}

void
LevelManager::parse_and_load_tileset(tinyxml2::XMLElement* root_element) {
        auto image_tag = root_element->FirstChildElement("image");

        sf::Image tileset_img;
        if (!tileset_img.loadFromFile(get_file_name(image_tag->Attribute("source"), tilesets_dir))) {
                throw std::runtime_error{
                        "LevelManager::parse_and_load_tileset(tinyxml2::XMLElement*): failed loading tileset"
                };
        }

        auto texture = std::make_unique<sf::Texture>();
        texture->loadFromImage(tileset_img);

        // Получаем количество строк-столбцов в тайлсете
        int columns = static_cast<int>(tileset_img.getSize().x / static_cast<unsigned>(m_tile_width));
        int rows    = static_cast<int>(tileset_img.getSize().y / static_cast<unsigned>(m_tile_height));

        // Нарежем текстуру на отдельные тайлы и сохраним готовый спрайт-тайл
        for (int current_row = 0; current_row < rows; current_row++) {
                for (int current_column = 0; current_column < columns; current_column++) {
                        sf::Rect<int> rect;
                        rect.top    = current_row * m_tile_height;
                        rect.height = m_tile_height;
                        rect.left   = current_column * m_tile_width;
                        rect.width  = m_tile_width;

                        sf::Sprite tile;
                        tile.setTexture(*texture);
                        tile.setTextureRect(rect);
                        m_gids_vect.push_back(tile);
                }
        }
        m_texture_store.push_back(std::move(texture));
}

void
LevelManager::parse_and_load_img_collection(tinyxml2::XMLElement* root_element) {
        auto tile_tag = root_element->FirstChildElement("tile");

        while (tile_tag) {

                auto      image_tag = tile_tag->FirstChildElement("image");
                sf::Image level_object_img;
                if (!level_object_img.loadFromFile(get_file_name(image_tag->Attribute("source"), imgs_dir))) {
                        throw std::runtime_error{ "LevelManager::parse_and_load_img_collection(tinyxml2::XMLElement*): "
                                                  "failed loading img collection" };
                }

                auto texture = std::make_unique<sf::Texture>();
                texture->loadFromImage(level_object_img);

                sf::Sprite tile;
                tile.setTexture(*texture);
                m_gids_vect.push_back(tile);

                m_texture_store.push_back(std::move(texture));
                tile_tag = tile_tag->NextSiblingElement("tile");
        }
}

std::string
LevelManager::get_file_name(std::string raw_str, const char* prefix_path) {
        return prefix_path + raw_str.substr(raw_str.find_last_of('/'));
}

void
LevelManager::parse_and_create_tile_layout(tinyxml2::XMLElement* root_element) {
        if (strcmp(root_element->Attribute("name"), "tile-layout") != 0) {
                throw std::runtime_error{
                        "LevelManager::parse_and_create_tile_layout(tinyxml2::XMLElement*): tile layout not found"
                };
        }
        auto data_tag = root_element->FirstChildElement("data");
        if (!data_tag) {
                throw std::runtime_error{
                        "LevelManager::parse_and_create_tile_layout(tinyxml2::XMLElement*): data tag not found"
                };
        }

        tinyxml2::XMLElement* tile_tag{ data_tag->FirstChildElement("tile") };
        if (!tile_tag) {
                throw std::runtime_error{
                        "LevelManager::parse_and_create_tile_layout(tinyxml2::XMLElement*): tile not found"
                };
        }

        auto tile_layout = std::make_unique<TileLayout>(m_world_view);
        int  x           = 0;
        int  y           = 0;
        while (tile_tag) {
                auto       tile_gid = static_cast<uint64_t>(atoi(tile_tag->Attribute("gid")));
                sf::Sprite tile(m_gids_vect[tile_gid - 1]);
                tile.setPosition(sf::Vector2f(static_cast<float>(x * m_tile_width),
                                              static_cast<float>(y * m_tile_height)));
                tile_layout->append_tile(tile);  // MAY BE ERROR!!!

                x++;
                if (x == m_level_width) {
                        x = 0;
                        y++;
                        if (y == m_level_height) {
                                y = 0;
                        }
                }
                tile_tag = tile_tag->NextSiblingElement("tile");
        }
        m_scene_graph.attach_child(std::move(tile_layout));
}

void
LevelManager::parse_and_create_level_objects(tinyxml2::XMLElement* root_element) {
        if (strcmp(root_element->Attribute("name"), "map-object-layout") != 0) {
                throw std::runtime_error{
                        "LevelManager::parse_and_create_level_objects(tinyxml2::XMLElement*): object layout not found"
                };
        }

        tinyxml2::XMLElement* obj_tag{ root_element->FirstChildElement("object") };
        if (!obj_tag) {
                throw std::runtime_error{
                        "LevelManager::parse_and_create_level_objects(tinyxml2::XMLElement*): object not found"
                };
        }

        auto lvl_object_layout = std::make_unique<SceneNode>(Category::Type::LevelObjectLayout);
        m_level_object_node    = lvl_object_layout.get();
        while (obj_tag) {
                auto         object_gid = static_cast<uint64_t>(atoi(obj_tag->Attribute("gid")));
                sf::Vector2f object_position(static_cast<float>(atof(obj_tag->Attribute("x"))),
                                             static_cast<float>(atof(obj_tag->Attribute("y"))));

                // Создаем узел объекта и добавляет на слой-узел:
                lvl_object_layout->attach_child(std::make_unique<LevelObject>(
                                m_gids_vect[object_gid - 1], object_position, m_world_view));

                obj_tag = obj_tag->NextSiblingElement("object");
        }
        m_scene_graph.attach_child(std::move(lvl_object_layout));
}

sf::Vector2f
LevelManager::get_level_size() const {
        return sf::Vector2f(static_cast<float>(m_tile_width * m_level_width),
                            static_cast<float>(m_tile_height * m_level_height));
}

void
LevelManager::parse_and_create_bounding_box_layout(tinyxml2::XMLElement* root_element) {
        if (strcmp(root_element->Attribute("name"), "aabb-layout") != 0) {
                throw std::runtime_error{ "AABB-layout layout not found!" };
        }

        tinyxml2::XMLElement* obj_tag{ root_element->FirstChildElement("object") };
        if (!obj_tag) {
                throw std::runtime_error{
                        "LevelManager::parse_and_create_bounding_box_layout(tinyxml2::XMLElement*): no <object> in root"
                };
        }

        auto bounding_box_layout{ std::make_unique<SceneNode>(Category::Type::BoundingBoxLayout) };
        while (obj_tag) {
                BoundingBox::Type box_type = strcmp(obj_tag->Attribute("type"), "radiation") == 0
                                                             ? BoundingBox::Type::RadiationArea
                                                             : BoundingBox::Type::Common;
                sf::Rect<float> bounding_rect;
                bounding_rect.top    = static_cast<float>(atof(obj_tag->Attribute("y")));
                bounding_rect.left   = static_cast<float>(atof(obj_tag->Attribute("x")));
                bounding_rect.height = static_cast<float>(atof(obj_tag->Attribute("height")));
                bounding_rect.width  = static_cast<float>(atof(obj_tag->Attribute("width")));

                bounding_box_layout->attach_child(std::make_unique<BoundingBox>(box_type, bounding_rect));

                obj_tag = obj_tag->NextSiblingElement("object");
        }
        m_scene_graph.attach_child(std::move(bounding_box_layout));
}

SceneNode*
LevelManager::getLevelObjectsNode() {
        return m_level_object_node;
}
