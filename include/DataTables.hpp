#ifndef TEST_SFML_LIBRARY_DATATABLES_HPP
#define TEST_SFML_LIBRARY_DATATABLES_HPP

#include "Command.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

#include <functional>
#include <map>
#include <vector>


class Person;

struct PersonData {
        std::map<Commands::ID, Command> commands_map;
        sf::Time                        fire_interval;
        int                             hitpoints;
        float                           speed;
        Textures::ID                    texture;
};

struct ProjectileData {
        sf::Time     lifetime;
        int          damage;
        float        speed;
        Textures::ID texture;
};

struct PickupData {
        std::function<void(Person&)> action;
        Textures::ID                 texture;
};

std::vector<PersonData>
initialize_person_data();
std::vector<PickupData>
initialize_pickup_data();
std::vector<ProjectileData>
initialize_projectile_data();

#endif  // TEST_SFML_LIBRARY_DATATABLES_HPP
