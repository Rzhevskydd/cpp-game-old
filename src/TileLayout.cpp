//
// Created by adminu on 05.12.2019.
//

#include "TileLayout.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>


TileLayout::TileLayout(const sf::View& world_view) : SceneNode(Category::Type::TileLayout), m_world_view(world_view) {}

sf::FloatRect
TileLayout::get_view_bounds() const {
        return sf::FloatRect(m_world_view.getCenter() - m_world_view.getSize() / 2.0F, m_world_view.getSize());
}

void
TileLayout::draw_current(sf::RenderTarget& target, sf::RenderStates states) const {
        for (auto& tile : m_level_tiles) {
                target.draw(tile, states);
                // TODO(G) Раскоментировать только в случае жестких лагов
                //                if (tile.getGlobalBounds().intersects(get_view_bounds())) {
                //                        std::cout << "current tile will draw\n";
                //                        target.draw(tile, states);
                //                }
                //                else: std::cout << "current tile will NOT draw\n";
        }
}

bool
TileLayout::is_marked_for_removal() const {
        return false;
}

bool
TileLayout::is_destroyed() const {
        return false;
}

void
TileLayout::append_tile(sf::Sprite& tile) {
        m_level_tiles.push_back(tile);
}
