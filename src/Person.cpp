#include "Person.hpp"
#include "CommandQueue.hpp"
#include "DataTables.hpp"
#include "Pickup.hpp"
#include "TextNode.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cmath>


const std::vector<PersonData> table = initialize_person_data();

Person::Person(Person::Type type, const TextureHolder& textures, const FontHolder& fonts)
                : Entity(table[type].hitpoints)
                , m_type(type)
                , m_sprite_rect(sf::FloatRect(0.0F, 0.0F, 29.0F, 39.0F))
                , m_walking(textures.get(table[type].texture))
                , m_death(textures.get(table[type].texture))
                , m_remove_threshold(sf::seconds(120))
                , m_fire_direction(None)
                , m_fire_countdown(sf::Time::Zero)
                , m_is_firing(false)
                , m_is_boosted(false)
                , m_boost_countdown(sf::Time::Zero)
                , m_is_irradiated(false)
                , m_radiation_countdown(sf::Time::Zero)
                , m_is_dead(false)
                , m_fire_rate_level(1)
                , m_health_display(nullptr)
                , m_missile_display(nullptr)
                , m_is_active(false)
                , m_drop_flag(false)
                , m_bot("bot_behavior.nn") {
        m_walking.set_frame_size(sf::Vector2i(29, 39));
        m_walking.set_num_frames(1);
        m_walking.set_repeating(true);
        m_walking.set_duration(sf::milliseconds(750));
        center_origin(m_walking);

        m_death.set_frame_size(sf::Vector2i(29, 39));
        m_death.set_num_frames(3);
        m_death.set_repeating(false);
        m_death.set_duration(sf::seconds(1));
        center_origin(m_death);

        m_fire_command.category = Category::Scene;
        m_fire_command.action   = [this, &textures](SceneNode& node, sf::Time /*unused*/) {
                create_bullets(node, textures);
        };

        m_drop_pickup_command.category = Category::Scene;
        m_drop_pickup_command.action   = [this, &textures](SceneNode& node, sf::Time /*unused*/) {
                create_pickup(node, textures);
        };


        std::unique_ptr<TextNode> health_display(new TextNode(fonts, ""));
        m_health_display = health_display.get();
        attach_child(std::move(health_display));

        if (Person::get_category() == Category::Player) {
                std::unique_ptr<TextNode> missile_display(new TextNode(fonts, ""));
                missile_display->setPosition(0, 70);
                m_missile_display = missile_display.get();
                attach_child(std::move(missile_display));
        }

        update_texts();
}

void
Person::draw_current(sf::RenderTarget& target, sf::RenderStates states) const {
        if (!is_destroyed()) {
                target.draw(m_walking, states);
        } else {
                target.draw(m_death, states);
        }
}

unsigned
Person::get_category() const {
        if (is_allied()) {
                return Category::Player;
        }

        return Category::Enemy;
}

bool
Person::is_allied() const {
        return m_type == Player;
}

sf::FloatRect
Person::get_bounding_rect() const {
        return sf::FloatRect(get_world_position()
                                             - sf::Vector2f{ m_sprite_rect.width / 2.0F, m_sprite_rect.height / 2.0F },
                             sf::Vector2f(m_sprite_rect.width, m_sprite_rect.height));
}

bool
Person::is_marked_for_removal() const {
        return m_is_dead && (m_death_timer.getElapsedTime() >= m_remove_threshold);
}

void
Person::update_current(sf::Time dt, CommandQueue& commands) {
        if (m_is_dead) {
                return;
        }

        // Entity has been destroyed: Possibly drop pickup, mark for removal
        if (is_destroyed()) {
                check_pickup_drop(commands);

                m_death.update(dt);

                if (m_death.is_finished()) {
                        m_death_timer.restart();
                        m_is_dead = true;
                }

                m_health_display->set_string("DEAD");

                return;
        }

        // Check if bullets or missiles are fired
        check_projectile_launch(dt, commands);

        // Check if boost are picked up and boost 2x itself
        check_boost(dt);

        check_radiation(dt);

        // Update walking animation
        m_walking.update(dt);

        m_prev_position = getPosition();
        Entity::update_current(dt, commands);

        // Set apprpriate cahracter rotation
        set_rotation();

        // Update texts
        update_texts();
}

void
Person::update_texts() {
        m_health_display->set_string(std::to_string(get_hitpoints()) + " HP");
        m_health_display->setPosition(0.0F, 50.0F);
        m_health_display->setRotation(-getRotation());
}

void
Person::fire(FireDirection direction) {
        m_fire_direction = direction;

        set_rotation(direction);

        // Only people with fire interval != 0 are able to fire
        if (table[m_type].fire_interval != sf::Time::Zero) {
                m_is_firing = true;
        }
}


void
Person::check_pickup_drop(CommandQueue& commands) {
        if (!is_allied() && random_int(150) != 0 && !m_drop_flag) {
                commands.push(m_drop_pickup_command);
                m_drop_flag = true;
        }
}

void
Person::check_projectile_launch(sf::Time dt, CommandQueue& commands) {
        // Check for automatic gunfire, allow only in intervals
        if (m_is_firing && m_fire_countdown <= sf::Time::Zero) {
                // Interval expired: We can fire a new bullet
                commands.push(m_fire_command);
                m_fire_countdown += table[m_type].fire_interval / (static_cast<float>(m_fire_rate_level) + 1.0F);
                m_is_firing = false;
        } else if (m_fire_countdown > sf::Time::Zero) {
                // Interval not expired: Decrease it further
                m_fire_countdown -= dt;
                m_is_firing = false;
        }
}

void
Person::create_bullets(SceneNode& node, const TextureHolder& textures) const {
        Projectile::Type type = is_allied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;

        create_projectile(node, type, textures);
}

void
Person::create_projectile(SceneNode& node, Projectile::Type type, const TextureHolder& textures) const {

        std::unique_ptr<Projectile> projectile(new Projectile(type, textures));
        sf::Vector2f                velocity;
        auto                        max_speed = projectile->get_max_speed();

        switch (m_fire_direction) {
        case Left:
                velocity = sf::Vector2f(-max_speed, 0.0F);

                break;

        case Right:
                velocity = sf::Vector2f(max_speed, 0.0F);

                break;

        case Up:
                velocity = sf::Vector2f(0.0F, -max_speed);
                break;

        case Down:
                velocity = sf::Vector2f(0.0F, +max_speed);
                break;
        }

        projectile->setPosition(get_world_position());
        projectile->set_velocity(velocity);
        node.attach_child(std::move(projectile));
}

void
Person::create_pickup(SceneNode& node, const TextureHolder& textures) const {
        auto type = static_cast<Pickup::Type>(random_int(Pickup::TypeCount));

        std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
        pickup->setPosition(get_world_position());
        pickup->set_velocity(0.0F, 0.0F);
        node.attach_child(std::move(pickup));
}


// TODO(Artöm Bakri Al-Sarmini): remove arg if always unused
void
Person::boost([[maybe_unused]] int increase) {
        m_is_boosted      = true;
        m_boost_countdown = sf::seconds(5);
}

void
Person::check_boost(sf::Time dt) {
        if (m_is_boosted && m_boost_countdown >= sf::Time::Zero) {
                auto new_velocity = get_velocity();
                new_velocity.x *= 1.3F;
                new_velocity.y *= 1.3F;
                Entity::accelerate(new_velocity);
                m_boost_countdown -= dt;
        } else {
                m_is_boosted = false;
        }
}

void
Person::set_rotation() {
        // TODO(Alexey Astrakhantsev): remove the magic numbers
        if (get_velocity().x > 0) {
                m_walking.set_start_frame(sf::Vector2i(87, 78));
                m_walking.set_num_frames(5);

                m_death.set_start_frame(sf::Vector2i(0, 507));
        } else if (get_velocity().x < 0) {
                m_walking.set_start_frame(sf::Vector2i(29, 39));
                m_walking.set_num_frames(5);

                m_death.set_start_frame(sf::Vector2i(0, 468));
        } else if (get_velocity().y > 0) {
                m_walking.set_start_frame(sf::Vector2i(29, 156));
                m_walking.set_num_frames(4);

                m_death.set_start_frame(sf::Vector2i(0, 546));
        } else if (get_velocity().y < 0) {
                m_walking.set_start_frame(sf::Vector2i(0, 0));
                m_walking.set_num_frames(4);

                m_death.set_start_frame(sf::Vector2i(0, 429));
        } else if (get_velocity() == sf::Vector2f(0, 0)) {
                // Stop the animation here, we are standing still
                m_walking.set_num_frames(1);
        }
}

void
Person::set_rotation(Person::FireDirection fire_direction) {
        switch (fire_direction) {
        case Left:
                m_walking.set_start_frame(sf::Vector2i(29, 39));
                break;

        case Right:
                m_walking.set_start_frame(sf::Vector2i(87, 78));
                break;

        case Up:
                m_walking.set_start_frame(sf::Vector2i(0, 0));
                break;

        case Down:
                m_walking.set_start_frame(sf::Vector2i(29, 156));
                break;

        default:
                break;
        }
}

bool
Person::is_active() const {
        return m_is_active;
}

void
Person::set_active() {
        m_is_active = true;
}

void Person::set_inactive() {
        m_is_active = false;
}

void
Person::set_previous_position() {
        setPosition(m_prev_position);
}

int
Person::step_bot(const std::vector<int>& env) {
        return m_bot.step(env);
}

void
Person::train_now(float award) {
        m_bot.trainNow(award);
}

void
Person::save() {
        m_bot.save();
}

unsigned int
Person::get_fire_dir() const {
        if (m_fire_countdown > sf::Time::Zero) {
                return m_fire_direction;
        }
        return None;
}

sf::Vector2f
Person::get_previous_position() const {
        return m_prev_position;
}

void Person::irradiate() {
        m_is_irradiated = true;
        m_radiation_countdown = sf::seconds(7.F);
}

void Person::check_radiation(sf::Time dt) {
        if (m_is_irradiated && m_radiation_countdown > sf::Time::Zero) {
                m_radiation_countdown -= dt;
                // set damage every sec
                if (m_radiation_countdown.asMilliseconds() % 1000 == 0) {
                        Entity::damage(4);
                }

        } else {
                m_is_irradiated = false;
        }
}

