#ifndef BRODILKA_PICKUP_HPP
#define BRODILKA_PICKUP_HPP

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Person;

class Pickup : public Entity {
public:
        enum Type { HealthRefill, SpeedBoost, TypeCount };

public:
        Pickup(Type type, const TextureHolder& textures);

        virtual unsigned
        get_category() const;

        virtual sf::FloatRect
        get_bounding_rect() const;

        void
        apply(Person& player) const;

private:
        virtual void
        draw_current(sf::RenderTarget& target, sf::RenderStates states) const;

private:
        virtual void
        set_rotation(){};

        Type       m_type;
        sf::Sprite m_sprite;
};

#endif  // BRODILKA_PICKUP_HPP
