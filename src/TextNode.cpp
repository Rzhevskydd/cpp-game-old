#include "TextNode.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>


TextNode::TextNode(const FontHolder& fonts, const std::string& text) : m_is_marked_to_destroy(false) {
        m_text.setFont(fonts.get(Fonts::Main));
        m_text.setCharacterSize(20);
        set_string(text);
}

void
TextNode::draw_current(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_text, states);
}

void
TextNode::set_string(const std::string& text) {
        m_text.setString(text);
        center_origin(m_text);
}

void
TextNode::destroy() {
        m_is_marked_to_destroy = true;
}

bool
TextNode::is_destroyed() const {
        return m_is_marked_to_destroy;
}

void
TextNode::set_color(sf::Color color) {
        m_text.setFillColor(color);
}
