#ifndef BOOK_RESOURCEIDENTIFIERS_HPP
#define BOOK_RESOURCEIDENTIFIERS_HPP

#include "ResourceHolder.hpp"

// Forward declaration of SFML classes
namespace sf {
        class Texture;
        class Font;
}  // namespace sf

namespace Textures {
        enum ID {
                MainCharacter,  // clang-format break
                Bandit,
                Bullet,
                HealthRefill,
                SpeedBoost,
                TitleScreen,
                ButtonNormal,
                ButtonSelected,
                ButtonPressed,
        };
}

namespace Fonts {
        enum ID {
                Main,
        };
}

// Forward declaration and a few type definitions
// template <typename Resource, typename Identifier>
// class ResourceHolder;

// template<> class ResourceHolder<sf::Texture, Textures::ID>;

using TextureHolder = ResourceHolder<sf::Texture, Textures::ID>;
using FontHolder    = ResourceHolder<sf::Font, Fonts::ID>;

#endif  // BOOK_RESOURCEIDENTIFIERS_HPP
