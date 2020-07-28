//
// Created by adminu on 20.12.2019.
//

#include "ForegroundManager.hpp"
#include "LevelObject.hpp"
#include "SceneNode.hpp"

ForegroundManager::ForegroundManager(SceneNode& scene_graph) : m_scene_graph(scene_graph), m_foreground_node(nullptr) {}

void
ForegroundManager::switch_to_foreground(LevelObject& lvl_object) {
        lvl_object.set_hidden(true);
        m_hidden_lvl_objects.push_back(&lvl_object);
        // TODO (G) Opacity change
        m_foreground_node->attach_child(lvl_object.get_copy());
}

void
ForegroundManager::return_lvl_object_back() {
        m_foreground_node->m_children.clear();
        for (auto lvl_obj : m_hidden_lvl_objects) {
                lvl_obj->set_hidden(false);
        }
}

void
ForegroundManager::initialize() {
        m_foreground_node = new SceneNode(Category::Foreground);
        m_scene_graph.attach_child(SceneNode::Ptr(m_foreground_node));
}
