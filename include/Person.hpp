#ifndef BOOK_PERSON_HPP
#define BOOK_PERSON_HPP

#include "Animation.hpp"
#include "Command.hpp"
#include "Entity.hpp"
#include "Projectile.hpp"
#include "ResourceIdentifiers.hpp"
#include "bot.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>


class CommandQueue;
class SceneNode;
class TextNode;

class Person : public Entity {
public:
        enum Type { Player, Enemy, TypeCount };

public:
        enum FireDirection {
                None  = 0,  // clang-format break
                Left  = 1 << 0,
                Right = 1 << 1,
                Up    = 1 << 2,
                Down  = 1 << 3
        };

public:
        Person(Type type, const TextureHolder& textures, const FontHolder& fonts);

        unsigned
        get_category() const override;

        sf::FloatRect
        get_bounding_rect() const override;

        bool
        is_marked_for_removal() const override;

        bool
        is_allied() const;

        void
        fire(FireDirection direction);

        unsigned int
        get_fire_dir() const;

        void
        boost(int increase);

        void
        irradiate();

        bool
        is_active() const;

        void
        set_active();

        void
        set_inactive();

        void
        set_previous_position();

        sf::Vector2f
        get_previous_position() const;

        int
        step_bot(const std::vector<int>& env);

        void
        train_now(float award);

        void
        save();

private:
        void
        draw_current(sf::RenderTarget& target, sf::RenderStates states) const override;

        void
        update_current(sf::Time dt, CommandQueue& commands) override;

        void
        check_pickup_drop(CommandQueue& commands);

        void
        check_projectile_launch(sf::Time dt, CommandQueue& commands);

        void
        check_boost(sf::Time dt);

        void
        create_bullets(SceneNode& node, const TextureHolder& textures) const;

        void
        create_projectile(SceneNode& node, Projectile::Type type, const TextureHolder& textures) const;

        void
        set_rotation() override;

        void
        set_rotation(FireDirection fire_direction);

        void
        check_radiation(sf::Time dt);

private:
        void
        create_pickup(SceneNode& node, const TextureHolder& textures) const;

        void
        update_texts();

private:
        Type          m_type;
        sf::FloatRect m_sprite_rect;

        Animation m_walking;
        Animation m_death;

        sf::Clock m_death_timer;
        sf::Time  m_remove_threshold;

        Command  m_fire_command;
        unsigned m_fire_direction;
        sf::Time m_fire_countdown;
        bool     m_is_firing;

        bool     m_is_boosted;
        sf::Time m_boost_countdown;

        bool     m_is_irradiated;
        sf::Time m_radiation_countdown;

        bool m_is_dead;

        int m_fire_rate_level;

        Command m_drop_pickup_command;

        TextNode* m_health_display;
        TextNode* m_missile_display;

        bool m_is_active;

        bool m_drop_flag;

        sf::Vector2f m_prev_position;  // in case collision on next iteration we use it to roll back

        BOT m_bot;
};

#endif  // BOOK_PERSON_HPP
