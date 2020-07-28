//
// Created by adminu on 05.12.2019.
//

#ifndef GAME_BOUNDINGBOX_HPP
#define GAME_BOUNDINGBOX_HPP

#include "SceneNode.hpp"

#include <SFML/Graphics/Rect.hpp>


class Person;

/// Данный класс отвечает за столкновения с объектами пейзажа. Мы могли были ограничиться
/// границами тайла из LevelObject'a, но с этой техникой мы можем рализовать переключение
/// ГГ на передний/задний план, когда он заходит например за дерево и собстенно дать ГГ
/// возможность ходить там где это возможно (т.е. без этого ГГ дерево должен был
/// обходить по периметру спрайта)..
class BoundingBox : public SceneNode {
public:
        enum Type {
                Common,
                RadiationArea,  // Потом можно добавить к-нибудь опасыне зоны на карту и ломать ГГ.
                Count
        };

public:
        BoundingBox(Type box_type, sf::Rect<float> bounding_box);

        /// Будем двигать переданного перса назад. Позже можно будет все запилить на datatable и применять функциию
        /// согласно Type.
        // TODO(G) В дальнейшем поведение этого метода будет зависеть от m_type,и от static'a все равно придется
        // отказаться.
        void
        interact(Person& person);

        sf::FloatRect
        get_bounding_rect() const override;

        bool
        is_marked_for_removal() const override;

        bool
        is_destroyed() const override;

private:
// TODO(Artóm Bakri Al-Sarmini): redo it
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
        [[maybe_unused]] Type m_type;
#pragma GCC diagnostic pop
        sf::Rect<float> m_bounding_box;
};


#endif  // GAME_BOUNDINGBOX_HPP
