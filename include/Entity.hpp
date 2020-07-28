#ifndef BOOK_ENTITY_HPP
#define BOOK_ENTITY_HPP

#include "SceneNode.hpp"


class Entity : public SceneNode {
public:
        explicit Entity(int hitpoints);

        void
        set_velocity(sf::Vector2f velocity);

        void
        set_velocity(float vx, float vy);

        void
        accelerate(sf::Vector2f velocity);

        void
        accelerate(float vx, float vy);

        sf::Vector2f
        get_velocity() const;

        int
        get_hitpoints() const;

        void
        repair(int points);

        void
        damage(int points);

        void
        destroy();

        virtual bool
        is_destroyed() const;

        virtual bool
        is_damaged();

protected:
        virtual void
        update_current(sf::Time dt, CommandQueue& commands);


private:
        virtual void
        set_rotation() = 0;

        sf::Vector2f m_velocity;
        int          m_hitpoints;

        bool m_is_damaged;
};

#endif  // BOOK_ENTITY_HPP
