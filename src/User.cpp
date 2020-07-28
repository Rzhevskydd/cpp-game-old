#include "User.hpp"
#include "CommandQueue.hpp"
#include "Person.hpp"
#include "Utility.hpp"

#include <SFML/Window/Event.hpp>

#include <algorithm>
#include <map>
#include <string>

User::User() {
        // Set initial key bindings
        m_key_binding[sf::Keyboard::A]     = MoveLeft;
        m_key_binding[sf::Keyboard::D]     = MoveRight;
        m_key_binding[sf::Keyboard::W]     = MoveUp;
        m_key_binding[sf::Keyboard::S]     = MoveDown;
        m_key_binding[sf::Keyboard::Left]  = FireLeft;
        m_key_binding[sf::Keyboard::Right] = FireRight;
        m_key_binding[sf::Keyboard::Up]    = FireUp;
        m_key_binding[sf::Keyboard::Down]  = FireDown;

        // Set initial action bindings
        initialize_actions();

        for (auto& pair : m_action_binding) {
                pair.second.category = Category::Player;
        }
}

void
User::handle_event(const sf::Event& event, CommandQueue& commands) {
        if (event.type == sf::Event::KeyPressed) {
                // Check if pressed key appears in key binding, trigger command if so
                auto found = m_key_binding.find(event.key.code);
                if (found != m_key_binding.end() && !is_realtime_action(found->second)) {
                        commands.push(m_action_binding[found->second]);
                }
        }
}

void
User::handle_realtime_input(CommandQueue& commands) {
        // Traverse all assigned keys and check if they are pressed
        for (auto pair : m_key_binding) {
                // If key is pressed, lookup action and trigger corresponding command
                if (sf::Keyboard::isKeyPressed(pair.first) && is_realtime_action(pair.second)) {
                        commands.push(m_action_binding[pair.second]);
                }
        }
}

void
User::assign_key(Action action, sf::Keyboard::Key key) {
        // Remove all keys that already map to action
        for (auto itr = m_key_binding.begin(); itr != m_key_binding.end();) {
                if (itr->second == action) {
                        m_key_binding.erase(itr++);
                } else {
                        ++itr;
                }
        }

        // Insert new binding
        m_key_binding[key] = action;
}

sf::Keyboard::Key
User::get_assigned_key(Action action) const {
        for (auto pair : m_key_binding) {
                if (pair.second == action) {
                        return pair.first;
                }
        }

        return sf::Keyboard::Unknown;
}

void
User::initialize_actions() {
        const float player_speed = 200.0F;

        m_action_binding[MoveLeft].action  = derived_action<Person>(PersonMover{ -player_speed, 0.0F });
        m_action_binding[MoveRight].action = derived_action<Person>(PersonMover{ +player_speed, 0.0F });
        m_action_binding[MoveUp].action    = derived_action<Person>(PersonMover{ 0.0F, -player_speed });
        m_action_binding[MoveDown].action  = derived_action<Person>(PersonMover{ 0.0F, +player_speed });
        m_action_binding[FireLeft].action  = derived_action<Person>([](Person& person, [[maybe_unused]] sf::Time time) {
                person.fire(Person::FireDirection::Left);
        });
        m_action_binding[FireRight].action = derived_action<Person>([](Person& person, [[maybe_unused]] sf::Time time) {
                person.fire(Person::FireDirection::Right);
        });
        m_action_binding[FireUp].action    = derived_action<Person>([](Person& person, [[maybe_unused]] sf::Time time) {
                person.fire(Person::FireDirection::Up);
        });
        m_action_binding[FireDown].action  = derived_action<Person>([](Person& person, [[maybe_unused]] sf::Time time) {
                person.fire(Person::FireDirection::Down);
        });
}

bool
User::is_realtime_action(Action action) {
        switch (action) {
        case MoveLeft:
        case MoveRight:
        case MoveDown:
        case MoveUp:
        case FireLeft:
        case FireRight:
        case FireUp:
        case FireDown:
                return true;

        default:
                return false;
        }
}
