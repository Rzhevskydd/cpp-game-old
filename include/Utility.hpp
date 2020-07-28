//
// Created by user on 27.11.2019.
//

#ifndef BRODILKA_UTILITY_HPP
#define BRODILKA_UTILITY_HPP

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>


namespace sf {
        class Sprite;
        class Text;
}  // namespace sf

class Person;

struct PersonMover {
        PersonMover(float vx, float vy);

        void
        operator()(Person& person, sf::Time time) const;

private:
        sf::Vector2f velocity;
};

// Call set_origin() with the center of the object
void
center_origin(sf::Sprite& sprite);
void
center_origin(sf::Text& text);
void
center_origin(sf::Sprite& sprite, sf::FloatRect& rect);
class Animation;
void
center_origin(Animation& animation);

// Degree/radian conversion
float
to_degree(float radian);
float
to_radian(float degree);

// Random number generation
int
random_int(int exclusive_max);

// Vector operations
float
length(sf::Vector2f vector);
sf::Vector2f
unit_vector(sf::Vector2f vector);

#endif  // BRODILKA_UTILITY_HPP
