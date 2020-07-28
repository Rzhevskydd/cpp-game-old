#include "Button.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>


namespace gui {
        Button::Button(const FontHolder& fonts, const TextureHolder& textures)
                        : m_normal_texture(textures.get(Textures::ButtonNormal))
                        , m_selected_texture(textures.get(Textures::ButtonSelected))
                        , m_pressed_texture(textures.get(Textures::ButtonPressed))
                        , m_text("", fonts.get(Fonts::Main), 16)
                        , m_is_toggle(false) {
                m_sprite.setTexture(m_normal_texture);

                sf::FloatRect bounds = m_sprite.getLocalBounds();
                m_text.setPosition(bounds.width / 2.0F, bounds.height / 2.0F);
        }

        void
        Button::set_callback(Callback callback) {
                m_callback = std::move(callback);
        }

        void
        Button::set_text(const std::string& text) {
                m_text.setString(text);
                center_origin(m_text);
        }

        void
        Button::set_toggle(bool flag) {
                m_is_toggle = flag;
        }

        bool
        Button::is_selectable() const {
                return true;
        }

        void
        Button::select() {
                Component::select();

                m_sprite.setTexture(m_selected_texture);
        }

        void
        Button::deselect() {
                Component::deselect();

                m_sprite.setTexture(m_normal_texture);
        }

        void
        Button::activate() {
                Component::activate();

                // If we are toggle then we should show that the button is pressed and thus "toggled".
                if (m_is_toggle) {
                        m_sprite.setTexture(m_pressed_texture);
                }

                if (m_callback) {
                        m_callback();
                }

                // If we are not a toggle then deactivate the button since we are just momentarily activated.
                if (!m_is_toggle) {
                        deactivate();
                }
        }

        void
        Button::deactivate() {
                Component::deactivate();

                if (m_is_toggle) {
                        // Reset texture to right one depending on if we are selected or not.
                        if (is_selected()) {
                                m_sprite.setTexture(m_selected_texture);
                        } else {
                                m_sprite.setTexture(m_normal_texture);
                        }
                }
        }

        void
        Button::handle_event(const sf::Event& /*event*/) {}

        void
        Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
                states.transform *= getTransform();
                target.draw(m_sprite, states);
                target.draw(m_text, states);
        }

}  // namespace gui
