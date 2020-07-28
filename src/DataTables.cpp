#include "DataTables.hpp"
#include "Person.hpp"
#include "Pickup.hpp"
#include "Projectile.hpp"
#include "Utility.hpp"


std::vector<PersonData>
initialize_person_data() {
        std::vector<PersonData> data(Person::TypeCount);

        data[Person::Player].hitpoints     = 150;
        data[Person::Player].speed         = 100.0F;
        data[Person::Player].texture       = Textures::MainCharacter;
        data[Person::Player].fire_interval = sf::seconds(1);

        data[Person::Enemy].hitpoints     = 80;
        data[Person::Enemy].speed         = 130.0F;
        data[Person::Enemy].texture       = Textures::Bandit;
        data[Person::Enemy].fire_interval = sf::seconds(1.5);

        auto speed = data[Person::Enemy].speed;

        data[Person::Enemy].commands_map[Commands::MoveLeft].action
                        = derived_action<Person>(PersonMover{ -speed, 0.0F });
        data[Person::Enemy].commands_map[Commands::MoveRight].action
                        = derived_action<Person>(PersonMover{ +speed, 0.0F });
        data[Person::Enemy].commands_map[Commands::MoveUp].action = derived_action<Person>(PersonMover{ 0.0F, -speed });
        data[Person::Enemy].commands_map[Commands::MoveDown].action
                        = derived_action<Person>(PersonMover{ 0.0F, +speed });

        data[Person::Enemy].commands_map[Commands::MoveLeft].category  = Category::Enemy;
        data[Person::Enemy].commands_map[Commands::MoveRight].category = Category::Enemy;
        data[Person::Enemy].commands_map[Commands::MoveUp].category    = Category::Enemy;
        data[Person::Enemy].commands_map[Commands::MoveDown].category  = Category::Enemy;

        return data;
}

std::vector<ProjectileData>
initialize_projectile_data() {
        std::vector<ProjectileData> data(Projectile::Typecount);

        data[Projectile::AlliedBullet].damage   = 10;
        data[Projectile::AlliedBullet].speed    = 1000.0F;
        data[Projectile::AlliedBullet].texture  = Textures::Bullet;
        data[Projectile::AlliedBullet].lifetime = sf::seconds(1);


        data[Projectile::EnemyBullet].damage   = 2;
        data[Projectile::EnemyBullet].speed    = 500.0F;
        data[Projectile::EnemyBullet].texture  = Textures::Bullet;
        data[Projectile::EnemyBullet].lifetime = sf::seconds(1.5);

        return data;
}

std::vector<PickupData>
initialize_pickup_data() {
        std::vector<PickupData> data(Pickup::TypeCount);

        data[Pickup::HealthRefill].texture = Textures::HealthRefill;
        data[Pickup::HealthRefill].action  = [](Person& person) {
                person.repair(15);
        };

        data[Pickup::SpeedBoost].texture = Textures::SpeedBoost;
        data[Pickup::SpeedBoost].action  = [](Person& person) {
                person.boost(2);
        };


        return data;
}
