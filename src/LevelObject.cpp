//
// Created by adminu on 04.12.2019.
//

#include "LevelObject.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>


LevelObject::LevelObject(sf::Sprite& sprite, sf::Vector2f position, const sf::View& world_view)
                : SceneNode(Category::Type::LevelObject)
                , m_sprite(sprite)
                , m_world_view(world_view)
                , m_is_hidden(false) {
        this->setPosition(position);
        m_sprite.setOrigin(0, m_sprite.getLocalBounds().height);
}

sf::FloatRect
LevelObject::get_bounding_rect() const {
        return get_world_transform().transformRect(
                        m_sprite.getGlobalBounds());  // магия, дающая rectangle на основе кордов внутри scene node и
                                                      // размеров спрайта
}

bool
LevelObject::is_marked_for_removal() const {
        return false;
}

bool
LevelObject::is_destroyed() const {
        return false;
}

sf::FloatRect
LevelObject::get_view_bounds() const {
        return sf::FloatRect(m_world_view.getCenter() - m_world_view.getSize() / 2.0F, m_world_view.getSize());
}

void
LevelObject::draw_current(sf::RenderTarget& target, sf::RenderStates states) const {
        // Потом стоит воспользоваться к-либо тру алгоритмом
        target.draw(m_sprite, states);
//        if (!m_is_hidden && this->get_bounding_rect().intersects(this->get_view_bounds())) {
//              target.draw(m_sprite, states);
//        }
}

void
LevelObject::set_hidden(bool is_hidden) {
        m_is_hidden = is_hidden;
}

SceneNode::Ptr
LevelObject::get_copy() {
        return SceneNode::Ptr(new LevelObject(this->m_sprite, this->getPosition(), this->m_world_view));
}
