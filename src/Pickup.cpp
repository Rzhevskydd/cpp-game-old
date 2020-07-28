#include "Pickup.hpp"
#include "DataTables.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>


static const std::vector<PickupData> pickup_data_table{ initialize_pickup_data() };

Pickup::Pickup(Pickup::Type type, const TextureHolder& textures)
                : Entity{ 1 }, m_type{ type }, m_sprite{ textures.get(pickup_data_table[type].texture) } {
        center_origin(m_sprite);
}

unsigned
Pickup::get_category() const {
        return Category::Pickup;
}


sf::FloatRect
Pickup::get_bounding_rect() const {
        return get_world_transform().transformRect(m_sprite.getGlobalBounds());
}

void
Pickup::apply(Person& player) const {
        pickup_data_table[m_type].action(player);
}

void
Pickup::draw_current(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_sprite, states);
}
