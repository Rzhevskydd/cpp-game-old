#include "Projectile.hpp"
#include "DataTables.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

static const std::vector<ProjectileData> projectile_data_table{ initialize_projectile_data() };


Projectile::Projectile(Projectile::Type type, const TextureHolder& textures)
                : Entity(1)
                , m_type(type)
                , m_sprite(textures.get(projectile_data_table[type].texture))
                , m_lifetime(projectile_data_table[type].lifetime)
                , m_countdown(sf::Time::Zero)
/* , m_is_marked_for_removal(false) */ {
        center_origin(m_sprite);
}

unsigned
Projectile::get_category() const {
        if (m_type == EnemyBullet) {
                return Category::EnemyProjectile;
        }
        return Category::AlliedProjectile;
}

sf::FloatRect
Projectile::get_bounding_rect() const {
        return get_world_transform().transformRect(m_sprite.getGlobalBounds());
}


float
Projectile::get_max_speed() const {
        return projectile_data_table[m_type].speed;
}

int
Projectile::get_damage() const {
        return projectile_data_table[m_type].damage;
}

void
Projectile::update_current(sf::Time dt, CommandQueue& commands) {

        m_countdown += dt;
        if (m_countdown >= m_lifetime) {
                this->destroy();
                m_countdown = sf::Time::Zero;
                return;
        }

        Entity::update_current(dt, commands);

        // Set appropriate rotation
        set_rotation();
}

void
Projectile::draw_current(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_sprite, states);
}

void
Projectile::set_rotation() {
        if (get_velocity().x > 0) {
                m_sprite.setRotation(90.0F);
        } else if (get_velocity().x < 0) {
                m_sprite.setRotation(-90.0F);
        } else if (get_velocity().y > 0) {
                m_sprite.setRotation(180.0F);
        } else if (get_velocity().y < 0) {
                m_sprite.setRotation(-180.0F);
        } else {
                m_sprite.setRotation(0.0F);
        }
}

bool
Projectile::is_marked_for_removal() const {
        return is_destroyed();
}
