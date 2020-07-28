//
// Created by adminu on 20.12.2019.
//

#ifndef GAME_FOREGROUNDMANAGER_HPP
#define GAME_FOREGROUNDMANAGER_HPP

#include <vector>


class SceneNode;
class LevelObject;

/// This class must be created after LevelManger, and after enemy's node too (this node isnt created now)
class ForegroundManager {
public:
        ForegroundManager(SceneNode& scene_graph);

        void
        initialize();

        void
        switch_to_foreground(LevelObject& lvl_object);

        void
        return_lvl_object_back();

private:
        std::vector<LevelObject*> m_hidden_lvl_objects;
        SceneNode&                m_scene_graph;
        SceneNode*                m_foreground_node;
};


#endif  // GAME_FOREGROUNDMANAGER_HPP
