#include "Animation.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


Animation::Animation() : m_duration(sf::Time::Zero), m_elapsed_time(sf::Time::Zero), m_repeat(false) {
        m_sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
}

Animation::Animation(const sf::Texture& texture)
                : m_sprite(texture), m_duration(sf::Time::Zero), m_elapsed_time(sf::Time::Zero), m_repeat(false) {
        m_sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
}

void
Animation::set_texture(const sf::Texture& texture) {
        m_sprite.setTexture(texture);
}

const sf::Texture*
Animation::get_texture() const {
        return m_sprite.getTexture();
}

void
Animation::set_frame_size(sf::Vector2i frame_size) {
        m_frame_size = frame_size;
}

sf::Vector2i
Animation::get_frame_size() const {
        return m_frame_size;
}

void
Animation::set_start_frame(sf::Vector2i start_frame) {
        m_start_frame = start_frame;
}

sf::Vector2i
Animation::get_start_frame() {
        return m_start_frame;
}

void
Animation::set_num_frames(std::size_t num_frames) {
        m_num_frames = num_frames;
}

std::size_t
Animation::get_num_frames() const {
        return m_num_frames;
}

void
Animation::set_duration(sf::Time duration) {
        m_duration = duration;
}

sf::Time
Animation::get_duration() const {
        return m_duration;
}

void
Animation::set_repeating(bool flag) {
        m_repeat = flag;
}

bool
Animation::is_repeating() const {
        return m_repeat;
}

void
Animation::restart() {
        m_current_frame = 0;
}

bool
Animation::is_finished() const {
        return m_current_frame >= m_num_frames;
}

sf::FloatRect
Animation::get_local_bounds() const {
        return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(get_frame_size()));
}

sf::FloatRect
Animation::get_global_bounds() const {
        return getTransform().transformRect(get_local_bounds());
}

void
Animation::update(sf::Time dt) {
        sf::Time time_per_frame = m_duration / static_cast<float>(m_num_frames);
        m_elapsed_time += dt;

        sf::Vector2i texture_bounds(m_sprite.getTexture()->getSize());
        sf::IntRect  texture_rect = m_sprite.getTextureRect();

        if (m_current_frame == 0) {
                texture_rect = sf::IntRect(m_start_frame.x, m_start_frame.y, m_frame_size.x, m_frame_size.y);
        }

        // While we have a frame to process
        while (m_elapsed_time >= time_per_frame && (m_current_frame <= m_num_frames || m_repeat)) {
                // Move the texture rect left
                texture_rect.left += texture_rect.width;

                // If we reach the end of the texture
                if (texture_rect.left + texture_rect.width > texture_bounds.x) {
                        // Move it down one line
                        texture_rect.left = 0;
                        texture_rect.top += texture_rect.height;
                }

                // And progress to next frame
                m_elapsed_time -= time_per_frame;
                if (m_repeat) {
                        m_current_frame = (m_current_frame + 1) % m_num_frames;

                        if (m_current_frame == 0) {
                                texture_rect = sf::IntRect(
                                                m_start_frame.x, m_start_frame.y, m_frame_size.x, m_frame_size.y);
                        }
                } else {
                        m_current_frame++;
                }
        }

        m_sprite.setTextureRect(texture_rect);
}

void
Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(m_sprite, states);
}
