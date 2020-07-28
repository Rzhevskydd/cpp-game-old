#ifndef BOOK_STATE_HPP
#define BOOK_STATE_HPP

#include "ResourceIdentifiers.hpp"
#include "StateIdentifiers.hpp"

#include <SFML/System/Time.hpp>

#include <memory>


namespace sf {
        class Clock;
        class Event;
        class RenderWindow;
}  // namespace sf

class StateStack;
class User;

class State {
public:
        using Ptr = std::unique_ptr<State>;

        struct Context {
                Context(sf::RenderWindow& t_window,
                        sf::Clock&        t_clock,
                        TextureHolder&    t_textures,
                        FontHolder&       t_fonts,
                        User&             t_user);

                sf::RenderWindow* window;
                sf::Clock*        clock;
                TextureHolder*    textures;
                FontHolder*       fonts;
                User*             user;
        };


public:
        State(StateStack& stack, Context context);
        virtual ~State();

        virtual void
        draw() = 0;
        virtual bool
        update(sf::Time dt)
                        = 0;
        virtual bool
        handle_event(const sf::Event& event)
                        = 0;


protected:
        void
        request_stack_push(States::ID state_id);
        void
        request_stack_pop();
        void
        request_state_clear();

        Context
        get_context() const;


private:
        StateStack* m_stack;
        Context     m_context;
};

#endif  // BOOK_STATE_HPP
