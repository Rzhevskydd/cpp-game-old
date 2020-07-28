#include "Utility.hpp"
#include "Person.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cmath>
#include <random>

static std::mt19937 random_engine{ std::random_device{}() };

PersonMover::PersonMover(float vx, float vy) : velocity{ vx, vy } {}

void
PersonMover::operator()(Person& person, [[maybe_unused]] sf::Time time) const {
        person.accelerate(velocity);
}

void
center_origin(sf::Sprite& sprite) {
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.0F), std::floor(bounds.top + bounds.height / 2.0F));
}

void
center_origin(sf::Text& text) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(std::floor(bounds.left + bounds.width / 2.0F), std::floor(bounds.top + bounds.height / 2.0F));
}

void
center_origin(sf::Sprite& sprite, sf::FloatRect& rect) {
        sprite.setOrigin(std::floor(rect.left + rect.width / 2.0F), std::floor(rect.top + rect.height / 2.0F));
}

void
center_origin(Animation& animation) {
        sf::FloatRect bounds = animation.get_local_bounds();
        animation.setOrigin(std::floor(bounds.left + bounds.width / 2.0F),
                            std::floor(bounds.top + bounds.height / 2.0F));
}

float
to_degree(float radian) {
        return 180.0F / 3.141592653589793238462643383F * radian;
}

float
to_radian(float degree) {
        return 3.141592653589793238462643383F / 180.0F * degree;
}

int
random_int(int exclusive_max) {
        std::uniform_int_distribution dist{ 0, exclusive_max - 1 };
        return dist(random_engine);
}

float
length(sf::Vector2f vector) {
        return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f
unit_vector(sf::Vector2f vector) {
        if (vector == sf::Vector2f{ 0, 0 }) {
                throw std::runtime_error{ "unit_vector(sf::Vector2f): zerodiv" };
        }

        return vector / length(vector);
}