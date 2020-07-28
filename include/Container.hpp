#ifndef BOOK_CONTAINER_HPP
#define BOOK_CONTAINER_HPP

#include "Component.hpp"

#include <memory>
#include <vector>


namespace gui {

        class Container : public Component {
        public:
                using Ptr = std::shared_ptr<Container>;

        public:
                Container();

                void
                pack(Component::Ptr component);

                bool
                is_selectable() const override;
                void
                handle_event(const sf::Event& event) override;


        private:
                using Component::select;

                void
                draw(sf::RenderTarget& target, sf::RenderStates states) const override;

                bool
                has_selection() const;
                void
                select(size_t index);
                void
                select_next();
                void
                select_previous();


        private:
                std::vector<Component::Ptr> m_children;
                size_t                      m_selected_child;
        };

}  // namespace gui

#endif  // BOOK_CONTAINER_HPP
