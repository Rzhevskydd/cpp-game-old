#ifndef BRODILKA_PROJECTILE_HPP
#define BRODILKA_PROJECTILE_HPP

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>


class CommandQueue;

class Projectile : public Entity {
public:
        enum Type { AlliedBullet, EnemyBullet, Typecount };

public:
        Projectile(Type type, const TextureHolder& textures);

        virtual unsigned
        get_category() const;

        virtual sf::FloatRect
        get_bounding_rect() const;

        float
        get_max_speed() const;

        int
        get_damage() const;

        virtual bool
        is_marked_for_removal() const;


private:
        virtual void
        update_current(sf::Time dt, CommandQueue& commands);

        virtual void
        draw_current(sf::RenderTarget& target, sf::RenderStates states) const;


private:
        virtual void
        set_rotation();

        Type         m_type;
        sf::Sprite   m_sprite;
        sf::Vector2f m_target_direction;

        sf::Time m_lifetime;
        sf::Time m_countdown;
        // bool m_is_marked_for_removal;  // TODO: remove if not used
};


#endif  // BRODILKA_PROJECTILE_HPP
