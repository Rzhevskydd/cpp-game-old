#ifndef BOOK_ANIMATION_HPP
#define BOOK_ANIMATION_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>


class Animation
                : public sf::Drawable
                , public sf::Transformable {
public:
        Animation();
        explicit Animation(const sf::Texture& texture);

        void
        set_texture(const sf::Texture& texture);
        const sf::Texture*
        get_texture() const;

        void
        set_frame_size(sf::Vector2i frame_size);
        sf::Vector2i
        get_frame_size() const;

        void
        set_start_frame(sf::Vector2i start_frame);
        sf::Vector2i
        get_start_frame();

        void
        set_num_frames(std::size_t num_frames);
        std::size_t
        get_num_frames() const;

        void
        set_duration(sf::Time duration);
        sf::Time
        get_duration() const;

        void
        set_repeating(bool flag);
        bool
        is_repeating() const;

        void
        restart();
        bool
        is_finished() const;

        sf::FloatRect
        get_local_bounds() const;
        sf::FloatRect
        get_global_bounds() const;

        void
        update(sf::Time dt);


private:
        void
        draw(sf::RenderTarget& target, sf::RenderStates states) const override;


private:
        sf::Sprite   m_sprite;
        sf::Vector2i m_frame_size;
        sf::Vector2i m_start_frame;
        std::size_t  m_num_frames{ 0 };
        std::size_t  m_current_frame{ 0 };
        sf::Time     m_duration;
        sf::Time     m_elapsed_time;
        bool         m_repeat;
};

#endif  // BOOK_ANIMATION_HPP
