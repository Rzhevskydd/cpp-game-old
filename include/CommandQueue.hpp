#ifndef BOOK_COMMANDQUEUE_HPP
#define BOOK_COMMANDQUEUE_HPP

#include "Command.hpp"

#include <queue>


class CommandQueue {
public:
        void
        push(const Command& command);

        Command
        pop();

        bool
        is_empty() const;


private:
        std::queue<Command> m_queue;
};

#endif  // BOOK_COMMANDQUEUE_HPP
