//
// Created by adminu on 05.12.2019.
//

#ifndef GAME_TILELAYOUT_HPP
#define GAME_TILELAYOUT_HPP

#include "SceneNode.hpp"

#include <SFML/Graphics/Sprite.hpp>


namespace sf {
        class View;
}

/// Данный класс представляет узел - тайловый фон. Прежде всего он должен уметь рисовать тайлы,
/// на поиск коллизий отвечать false, и делать (оционально) проверку при рисовании тайла что он будет показан на экране.
class TileLayout : public SceneNode {
public:
        TileLayout(const sf::View& world_view);

        void
        append_tile(sf::Sprite& tile);  // Протесить ссылкой и без: типо спрайт созданый в лоакльной области
                                        // и переданный по ссылке будет не оч. Или нужно юзать move или вектор сам
                                        // копирует...

        bool
        is_marked_for_removal() const override;

        bool
        is_destroyed() const override;

private:
        void
        draw_current(sf::RenderTarget& target, sf::RenderStates states) const override;

        sf::FloatRect
        get_view_bounds() const;

        const sf::View&         m_world_view;  // храним что бы получать квадрат для вычисления нужно ли рисовать тайл
        std::vector<sf::Sprite> m_level_tiles;
};


#endif  // GAME_TILELAYOUT_HPP
