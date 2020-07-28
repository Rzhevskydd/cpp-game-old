#ifndef BOOK_SCENENODE_HPP
#define BOOK_SCENENODE_HPP

#include "Category.hpp"
#include "Command.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

#include <memory>
#include <set>
#include <vector>


struct Command;

class CommandQueue;

class SceneNode
                : public sf::Transformable
                , public sf::Drawable {

        friend class ForegroundManager;

public:
        using Ptr  = std::unique_ptr<SceneNode>;
        using Pair = std::pair<SceneNode*, SceneNode*>;

        void
        update(sf::Time dt, CommandQueue& commands);

public:
        explicit SceneNode(Category::Type category = Category::None);

        SceneNode(const SceneNode&) = delete;
        SceneNode&
        operator=(const SceneNode&)
                        = delete;

        void
        attach_child(Ptr child);

        Ptr
        detach_child(const SceneNode& node);

        sf::Vector2f
        get_world_position() const;

        sf::Transform
        get_world_transform() const;

        void
        on_command(const Command& command, sf::Time dt);

        virtual unsigned
        get_category() const;

        virtual sf::FloatRect
        get_bounding_rect() const;

        void
        check_scene_collision(SceneNode&               scene_graph,
                              std::set<Pair>&          collision_pairs,
                              std::vector<SceneNode*>& game_objects);

        void
        check_node_collision(SceneNode& node, std::set<Pair>& collision_pairs, std::vector<SceneNode*>& game_objects);

        void
        remove_wrecks();

        virtual bool
        is_marked_for_removal() const;

        virtual bool
        is_destroyed() const;

private:
        virtual void
        update_current(sf::Time dt, CommandQueue& commands);

        void
        update_children(sf::Time dt, CommandQueue& commands);

        virtual void
        draw(sf::RenderTarget& target, sf::RenderStates states) const;

        virtual void
        draw_current(sf::RenderTarget& target, sf::RenderStates states) const;

        void
        draw_children(sf::RenderTarget& target, sf::RenderStates states) const;


private:
        std::vector<Ptr> m_children;
        SceneNode*       m_parent;
        Category::Type   m_default_category;
};

bool
collision(const SceneNode& lhs, const SceneNode& rhs);

#endif  // BOOK_SCENENODE_HPP
