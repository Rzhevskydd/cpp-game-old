//
// Created by adminu on 05.12.2019.
//

#include "BoundingBox.hpp"
#include "Person.hpp"

BoundingBox::BoundingBox(Type box_type, sf::Rect<float> bounding_box)
                : SceneNode(Category::Type::BoundingBox), m_type(box_type), m_bounding_box(bounding_box) {}

sf::FloatRect
BoundingBox::get_bounding_rect() const {
        return get_world_transform().transformRect(m_bounding_box);
}

bool
BoundingBox::is_marked_for_removal() const {
        return false;
}

bool
BoundingBox::is_destroyed() const {
        return false;
}

void
BoundingBox::interact(Person& person) {
        // Do action, based on BB type.
        if (this->m_type == BoundingBox::Common) {
                person.set_previous_position();
                person.set_velocity(0, 0);

        } else if (this->m_type == BoundingBox::RadiationArea) {
                person.irradiate();
        }
}
