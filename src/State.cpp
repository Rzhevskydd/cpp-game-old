#include "State.hpp"
#include "StateStack.hpp"


#include <SFML/Window/Event.hpp>


State::Context::Context(sf::RenderWindow& t_window,
                        sf::Clock&        t_clock,
                        TextureHolder&    t_textures,
                        FontHolder&       t_fonts,
                        User&             t_user)
                : window(&t_window), clock(&t_clock), textures(&t_textures), fonts(&t_fonts), user(&t_user) {}

State::State(StateStack& stack, Context context) : m_stack(&stack), m_context(context) {}

State::~State() = default;

void
State::request_stack_push(States::ID state_id) {
        m_stack->push_state(state_id);
}

void
State::request_stack_pop() {
        m_stack->pop_state();
}

void
State::request_state_clear() {
        m_stack->clear_states();
}

State::Context
State::get_context() const {
        return m_context;
}
