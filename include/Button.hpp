
#ifndef BOOK_BUTTON_HPP
#define BOOK_BUTTON_HPP

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>


namespace gui {

        class Button : public Component {
        public:
                using Ptr      = std::shared_ptr<Button>;
                using Callback = std::function<void()>;


        public:
                Button(const FontHolder& fonts, const TextureHolder& textures);

                void
                set_callback(Callback callback);
                void
                set_text(const std::string& text);
                void
                set_toggle(bool flag);

                bool
                is_selectable() const override;
                void
                select() override;
                void
                deselect() override;

                void
                activate() override;
                void
                deactivate() override;

                void
                handle_event(const sf::Event& event) override;


        private:
                void
                draw(sf::RenderTarget& target, sf::RenderStates states) const override;


        private:
                Callback           m_callback;
                const sf::Texture& m_normal_texture;
                const sf::Texture& m_selected_texture;
                const sf::Texture& m_pressed_texture;
                sf::Sprite         m_sprite;
                sf::Text           m_text;
                bool               m_is_toggle;
        };

}  // namespace gui

#endif  // BOOK_BUTTON_HPP
