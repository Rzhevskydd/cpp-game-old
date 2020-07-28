#include <Container.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <limits>


namespace gui {

        Container::Container() : m_selected_child{ std::numeric_limits<size_t>::max() } {}

        void
        Container::pack(Component::Ptr component) {
                m_children.push_back(component);

                if (!has_selection() && component->is_selectable()) {
                        select(m_children.size() - 1);
                }
        }

        bool
        Container::is_selectable() const {
                return false;
        }

        void
        Container::handle_event(const sf::Event& event) {
                // If we have selected a child then give it events
                if (has_selection() && m_children[m_selected_child]->is_active()) {
                        m_children[m_selected_child]->handle_event(event);
                } else if (event.type == sf::Event::KeyReleased) {
                        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
                                select_previous();
                        } else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
                                select_next();
                        } else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space) {
                                if (has_selection()) {
                                        m_children[m_selected_child]->activate();
                                }
                        }
                }
        }

        void
        Container::draw(sf::RenderTarget& target, sf::RenderStates states) const {
                states.transform *= getTransform();

                for (const Component::Ptr& child : m_children) {
                        target.draw(*child, states);
                }
        }

        bool
        Container::has_selection() const {
                return m_selected_child != std::numeric_limits<size_t>::max();
        }

        void
        Container::select(size_t index) {
                if (m_children[index]->is_selectable()) {
                        if (has_selection()) {
                                m_children[m_selected_child]->deselect();
                        }

                        m_children[index]->select();
                        m_selected_child = index;
                }
        }

        void
        Container::select_next() {
                if (!has_selection()) {
                        return;
                }

                // Search next component that is selectable, wrap around if necessary
                size_t next{ m_selected_child };
                do {
                        next = (next + 1) % m_children.size();
                } while (!m_children[next]->is_selectable());

                // Select that component
                select(next);
        }

        void
        Container::select_previous() {
                if (!has_selection()) {
                        return;
                }

                // Search previous component that is selectable, wrap around if necessary
                size_t prev{ m_selected_child };
                do {
                        prev = (prev + m_children.size() - 1) % m_children.size();
                } while (!m_children[prev]->is_selectable());

                // Select that component
                select(prev);
        }

}  // namespace gui
