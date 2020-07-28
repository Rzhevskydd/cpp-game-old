#include "StateStack.hpp"


StateStack::StateStack(State::Context context) : m_context{ context } {}

void
StateStack::update(sf::Time dt) {
        // Iterate from top to bottom, stop as soon as update() returns false
        for (auto itr{ m_stack.rbegin() }; itr != m_stack.rend(); ++itr) {
                if (!(*itr)->update(dt)) {
                        break;
                }
        }

        apply_pending_changes();
}

void
StateStack::draw() {
        // Draw all active states from bottom to top
        for (State::Ptr& state : m_stack) {
                state->draw();
        }
}

void
StateStack::handle_event(const sf::Event& event) {
        // Iterate from top to bottom, stop as soon as handleEvent() returns false
        for (auto itr{ m_stack.rbegin() }; itr != m_stack.rend(); ++itr) {
                if (!(*itr)->handle_event(event)) {
                        break;
                }
        }

        apply_pending_changes();
}

void
StateStack::push_state(States::ID state_id) {
        m_pending_list.emplace_back(Push, state_id);
}

void
StateStack::pop_state() {
        m_pending_list.emplace_back(Pop);
}

void
StateStack::clear_states() {
        m_pending_list.emplace_back(Clear);
}

bool
StateStack::is_empty() const {
        return m_stack.empty();
}

State::Ptr
StateStack::create_state(States::ID state_id) {
        auto found{ m_factories.find(state_id) };
        if (found == m_factories.end()) {
                throw std::runtime_error{ "StateStack::create_state(States::ID): no id in factories" };
        }

        return found->second();
}

void
StateStack::apply_pending_changes() {
        for (PendingChange& change : m_pending_list) {
                switch (change.action) {
                case Push:
                        m_stack.push_back(create_state(change.state_id));
                        break;

                case Pop:
                        m_stack.pop_back();
                        break;

                case Clear:
                        m_stack.clear();
                        break;
                }
        }

        m_pending_list.clear();
}

StateStack::PendingChange::PendingChange(Action t_action, States::ID t_id) : action{ t_action }, state_id{ t_id } {}
