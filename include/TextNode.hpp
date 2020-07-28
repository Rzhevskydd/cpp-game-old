#ifndef BRODILKA_TEXTNODE_HPP
#define BRODILKA_TEXTNODE_HPP

#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class TextNode : public SceneNode {
public:
        explicit TextNode(const FontHolder& fonts, const std::string& text);

        void
        set_string(const std::string& text);

        void
        destroy();

        virtual bool
        is_destroyed() const;

        void
        set_color(sf::Color color);


private:
        virtual void
        draw_current(sf::RenderTarget& target, sf::RenderStates states) const;


private:
        sf::Text m_text;
        bool     m_is_marked_to_destroy;
};


#endif  // BRODILKA_TEXTNODE_HPP
