//
// Created by adminu on 04.12.2019.
//

#ifndef GAME_LEVELOBJECT_HPP
#define GAME_LEVELOBJECT_HPP

#include "SceneNode.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>


namespace sf {
        class RenderTarget;
        class Sprite;
        class View;
}  // namespace sf

/// Данный класс представляет объекты карты.
/// Прежде всего должен уметь рисоваться и во вторую очередь вычислять коллизии.
class LevelObject : public SceneNode {
public:
        LevelObject(sf::Sprite& sprite, sf::Vector2f position, const sf::View& world_view);

        sf::FloatRect
        get_bounding_rect() const override;
        bool
        is_marked_for_removal() const override;

        bool
        is_destroyed() const override;

        void
        set_hidden(bool is_hidden);

        Ptr
        get_copy();

private:
        void
        draw_current(sf::RenderTarget& target, sf::RenderStates states) const override;

        sf::FloatRect
        get_view_bounds() const;

        sf::Sprite& m_sprite;
        const sf::View& m_world_view;  // храним что бы получать квадрат для вычисления коллизий

        bool m_is_hidden;
};

#endif  // GAME_LEVELOBJECT_HPP
