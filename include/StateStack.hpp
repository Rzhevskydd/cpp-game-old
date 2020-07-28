#ifndef BOOK_STATESTACK_HPP
#define BOOK_STATESTACK_HPP

#include "State.hpp"
#include "StateIdentifiers.hpp"

#include "SFML/System/Time.hpp"

#include <functional>
#include <map>
#include <utility>
#include <vector>


namespace sf {
        class Event;
        class RenderWindow;
}  // namespace sf

class StateStack {
public:
        enum Action {
                Push,
                Pop,
                Clear,
        };


public:
        explicit StateStack(State::Context context);

        StateStack(const StateStack&) = delete;
        StateStack&
        operator=(const StateStack&)
                        = delete;

        template <typename T>
        void
        register_state(States::ID state_id) {
                m_factories[state_id] = [this]() {
                        return std::make_unique<T>(*this, m_context);
                };
        }

        void
        update(sf::Time dt);
        void
        draw();
        void
        handle_event(const sf::Event& event);

        void
        push_state(States::ID state_id);
        void
        pop_state();
        void
        clear_states();

        bool
        is_empty() const;


private:
        State::Ptr
        create_state(States::ID state_id);
        void
        apply_pending_changes();


private:
        struct PendingChange {
                PendingChange(Action t_action, States::ID t_id = States::None);
                Action     action;
                States::ID state_id;
        };


private:
        std::vector<State::Ptr>    m_stack;
        std::vector<PendingChange> m_pending_list;

        State::Context                                    m_context;
        std::map<States::ID, std::function<State::Ptr()>> m_factories;
};

#endif  // BOOK_STATESTACK_HPP
