#include "CommandQueue.hpp"
#include "SceneNode.hpp"


void
CommandQueue::push(const Command& command) {
        m_queue.push(command);
}

Command
CommandQueue::pop() {
        Command command = m_queue.front();
        m_queue.pop();
        return command;
}

bool
CommandQueue::is_empty() const {
        return m_queue.empty();
}
