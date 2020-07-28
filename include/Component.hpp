#ifndef BOOK_COMPONENT_HPP
#define BOOK_COMPONENT_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>


namespace sf {
        class Event;
}

namespace gui {

        class Component
                        : public sf::Drawable
                        , public sf::Transformable {
        public:
                using Ptr = std::shared_ptr<Component>;


        public:
                Component();

                Component(const Component&) = delete;
                Component&
                operator=(const Component&)
                                = delete;

                virtual ~Component();

                virtual bool
                is_selectable() const = 0;
                bool
                is_selected() const;
                virtual void
                select();
                virtual void
                deselect();

                virtual bool
                is_active() const;
                virtual void
                activate();
                virtual void
                deactivate();

                virtual void
                handle_event(const sf::Event& event)
                                = 0;


        private:
                bool m_is_selected;
                bool m_is_active;
        };

}  // namespace gui

#endif  // BOOK_COMPONENT_HPP
