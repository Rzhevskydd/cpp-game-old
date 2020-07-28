#include "GameState.hpp"
#include "User.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>


GameState::GameState(StateStack& stack, Context context)
                : State{ stack, context }, m_world{ *context.window }, m_user{ *context.user } {}

void
GameState::draw() {
        m_world.draw();
}

bool
GameState::update(sf::Time dt) {
        m_world.update(dt);

        if (m_world.is_player_dead()) {
                request_stack_push(States::Death);
        }

        CommandQueue& commands = m_world.get_command_queue();
        m_user.handle_realtime_input(commands);

        return true;
}

bool
GameState::handle_event(const sf::Event& event) {
        // Game input handling
        CommandQueue& commands = m_world.get_command_queue();
        m_user.handle_event(event, commands);

        // Escape pressed, trigger the pause screen
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                request_stack_push(States::Pause);
        }

        return true;
}