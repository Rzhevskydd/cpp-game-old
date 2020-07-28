#include "Entity.hpp"

#include <stdexcept>


Entity::Entity(int hitpoints) : m_hitpoints{ hitpoints }, m_is_damaged{ false } {}

void
Entity::set_velocity(sf::Vector2f velocity) {
        m_velocity = velocity;
}

void
Entity::set_velocity(float vx, float vy) {
        m_velocity.x = vx;
        m_velocity.y = vy;
}

sf::Vector2f
Entity::get_velocity() const {
        return m_velocity;
}

void
Entity::accelerate(sf::Vector2f velocity) {
        m_velocity += velocity;
}

void
Entity::accelerate(float vx, float vy) {
        m_velocity.x += vx;
        m_velocity.y += vy;
}

int
Entity::get_hitpoints() const {
        return m_hitpoints;
}

void
Entity::repair(int points) {
        if (points <= 0) {
                throw std::runtime_error{ "Entity::repair(int): negative arg0" };
        }

        m_hitpoints += points;
}

void
Entity::damage(int points) {
        if (points <= 0) {
                throw std::runtime_error{ "Entity::damage(int): negative arg0" };
        }

        m_hitpoints -= points;
        m_is_damaged = true;
}

void
Entity::destroy() {
        m_hitpoints = 0;
}

bool
Entity::is_destroyed() const {
        return m_hitpoints <= 0;
}


void
Entity::update_current(sf::Time dt, [[maybe_unused]] CommandQueue& commands) {
        move(m_velocity * dt.asSeconds());
        m_is_damaged = false;
}

bool
Entity::is_damaged() {
        return m_is_damaged;
}
