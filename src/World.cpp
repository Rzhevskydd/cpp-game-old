#include "World.hpp"

#include "BoundingBox.hpp"
#include "DataTables.hpp"
#include "Pickup.hpp"
#include "TextNode.hpp"
#include "Utility.hpp"

#include "BrdDebugConfig.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <LevelObject.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <random>


namespace {
        const std::vector<PersonData> table = initialize_person_data();
}


World::World(sf::RenderWindow& window)
                : m_window(window)
                , m_world_view(window.getDefaultView())
                , m_level_manager(m_scene_graph, m_world_view)
                , m_world_bounds(0.0F, 0.0F, m_level_manager.get_level_size().x, m_level_manager.get_level_size().y)
                , m_spawn_position(m_level_manager.get_level_size().x / 2.0F, m_level_manager.get_level_size().y / 2.0F)
                , m_player(nullptr)
                , m_number_of_enemies(15)
                , m_foreground_manager(m_scene_graph)
                , m_countdown(sf::Time::Zero)
                , m_wave_number(1)
                , m_is_wavetext_displayed(false)
                , m_wavetext_countdown(sf::Time::Zero) {
        m_commands = table[Person::Enemy].commands_map;
        load_textures();
        build_scene();

        // Prepare the view
        m_world_view.setCenter(m_spawn_position);
        m_world_view.zoom(0.825f);
}

void
World::update(sf::Time dt) {
        m_countdown += dt;
        m_foreground_manager.return_lvl_object_back();

        // Collision detection and response (may destroy entities)
        handle_collisions();

        sf::Vector2f player_position = m_player->getPosition();
        sf::Vector2f player_velocity = m_player->get_velocity();


        if ((player_position.x + static_cast<float>(m_world_view.getSize().x) / 2.0F)
                            <= (m_world_bounds.left + m_world_bounds.width)
            && (player_position.x - static_cast<float>(m_world_view.getSize().x) / 2.0F >= m_world_bounds.left)
            && static_cast<bool>(player_velocity.x)) {
                m_world_view.move(player_velocity.x * dt.asSeconds(), 0.0F);
        }

        if ((player_position.y + static_cast<float>(m_world_view.getSize().y) / 2.0F
             <= (m_world_bounds.top + m_world_bounds.height))
            && (player_position.y - static_cast<float>(m_world_view.getSize().y) / 2.0F >= m_world_bounds.top)
            && static_cast<bool>(player_velocity.y)) {
                m_world_view.move(0.0F, player_velocity.y * dt.asSeconds());
        }

        sf::Vector2f fire_pos1(player_position.x, player_position.y - 250.0F);
        sf::Vector2f fire_pos2(player_position.x, player_position.y + 250.0F);
        sf::Vector2f fire_pos3(player_position.x - 250.0F, player_position.y);
        sf::Vector2f fire_pos4(player_position.x + 250.0F, player_position.y);


        std::vector<sf::Vector2f> fire_positions;
        fire_positions.push_back(fire_pos1);
        fire_positions.push_back(fire_pos2);
        fire_positions.push_back(fire_pos3);
        fire_positions.push_back(fire_pos4);

        auto player_bounds = m_player->get_bounding_rect();
        for (int i = 1; i < 3; i++) {
                auto f{ static_cast<float>(i) };
                fire_positions.emplace_back(fire_pos1.x, fire_pos1.y - f * player_bounds.height * 2);
                fire_positions.emplace_back(fire_pos2.x, fire_pos2.y + f * player_bounds.height * 2);
                fire_positions.emplace_back(fire_pos3.x - f * player_bounds.width * 2, fire_pos3.y);
                fire_positions.emplace_back(fire_pos4.x + f * player_bounds.width * 2, fire_pos4.y);
        }

        // trained enemies block
        size_t n = 5;
        if (m_countdown >= sf::seconds(1.f / 40.f)) {
                for (auto enemy : m_active_enemies) {

                        auto enemy_pos = enemy->getPosition();
                        std::sort(fire_positions.begin(),
                                  fire_positions.end(),
                                  [&enemy_pos](sf::Vector2f lhs, sf::Vector2f rhs) {
                                          auto r1 = enemy_pos - lhs;
                                          auto r2 = enemy_pos - rhs;

                                          return length(r1) < length(r2);
                                  });

                        if (!enemy->is_active() && get_battlefield_bounds().contains(enemy_pos)) {
                                enemy->set_active();
                        } else if (!get_battlefield_bounds_2x().contains(enemy_pos) && enemy->is_active()) {
                                enemy->set_inactive();
                        }


                        if (enemy->is_active()) {
                                if (get_battlefield_bounds_2x().contains(enemy_pos)
                                    && !get_battlefield_bounds().contains(enemy_pos)) {
                                        enemy->set_velocity(0.0F, 0.0F);
                                        sf::Vector2f distance = fire_positions[0] - enemy_pos;


                                        if (std::abs(distance.x) < m_player->get_bounding_rect().width / 1.5f) {
                                                if (distance.y >= 0)
                                                        enemy->on_command(m_commands[Commands::MoveDown], dt);
                                                else if (distance.y <= 0)
                                                        enemy->on_command(m_commands[Commands::MoveUp], dt);
                                        } else if (std::abs(distance.y) < m_player->get_bounding_rect().height / 1.5f) {
                                                if (distance.x >= 0)
                                                        enemy->on_command(m_commands[Commands::MoveRight], dt);
                                                else if (distance.x <= 0)
                                                        enemy->on_command(m_commands[Commands::MoveLeft], dt);
                                        } else if (distance.x >= 0.f && distance.y <= 0.f) {
                                                enemy->on_command(m_commands[Commands::MoveRight], dt);
                                                enemy->on_command(m_commands[Commands::MoveUp], dt);
                                        } else if (distance.x >= 0.f && distance.y >= 0.f) {
                                                enemy->on_command(m_commands[Commands::MoveRight], dt);
                                                enemy->on_command(m_commands[Commands::MoveDown], dt);
                                        } else if (distance.x <= 0.f && distance.y >= 0.f) {
                                                enemy->on_command(m_commands[Commands::MoveLeft], dt);
                                                enemy->on_command(m_commands[Commands::MoveDown], dt);
                                        } else if (distance.x <= 0.f && distance.y <= 0.f) {
                                                enemy->on_command(m_commands[Commands::MoveLeft], dt);
                                                enemy->on_command(m_commands[Commands::MoveUp], dt);
                                        }
                                        sf::Vector2f velocity = enemy->get_velocity();
                                        // If moving diagonally, reduce velocity (to have always same velocity)
                                        if (velocity.x != 0.f && velocity.y != 0.f)
                                                enemy->set_velocity(velocity / std::sqrt(2.f));

                                        adapt_enemy_position(enemy);

                                        continue;
                                }


                                auto env = get_enviroment(enemy, n);
                                enemy->set_velocity(0.0F, 0.0F);
                                int action = enemy->step_bot(env);
                                switch (action) {
                                case 1:
                                        enemy->on_command(m_commands[Commands::MoveDown], dt);
                                        break;
                                case 2:
                                        enemy->on_command(m_commands[Commands::MoveUp], dt);
                                        break;
                                case 3:
                                        enemy->on_command(m_commands[Commands::MoveLeft], dt);
                                        break;
                                case 4:
                                        enemy->on_command(m_commands[Commands::MoveRight], dt);
                                        break;
                                case 5:
                                        enemy->fire(Person::Down);
                                        break;
                                case 6:
                                        enemy->fire(Person::Up);
                                        break;
                                case 7:
                                        enemy->fire(Person::Left);
                                        break;
                                case 8:
                                        enemy->fire(Person::Right);
                                        break;
                                case 9:
                                        break;
                                default:
                                        throw std::runtime_error{ "World::update(sf::Time): unsupported action" };
                                }
                                sf::Vector2f velocity = enemy->get_velocity();
                                // If moving diagonally, reduce velocity (to have always same velocity)
                                if (velocity.x != 0.0F && velocity.y != 0.0F) {
                                        enemy->set_velocity(velocity / std::sqrt(2.0F));
                                }

                                adapt_enemy_position(enemy);

                                auto fire_dist = player_position - enemy_pos;

                                auto new_enemy_pos = enemy_pos + velocity * dt.asSeconds();
                                auto new_dist      = length(fire_positions[0] - new_enemy_pos);
                                auto old_dist      = length(fire_positions[0] - enemy_pos);

                                auto new_fire_dist = player_position - new_enemy_pos;

                                size_t center = (n * n) / 2;
                                if (env[center] == 4 && enemy_pos == enemy->get_previous_position()) {
                                        enemy->train_now(0.f);
                                        continue;
                                }


                                if (std::abs(fire_dist.x) < m_player->get_bounding_rect().width / 1.5f) {

                                        if ((env[center] == 3 || env[center - n] == 3 || env[center - 2 * n] == 3
                                             || env[center + n] == 3 || env[center + 2 * n] == 3)) {
                                                if (velocity.x == 0.f && velocity.y == 0.f) {
                                                        BRD_DEBUG_PRINT("DODGE 1\n");
                                                        enemy->train_now(0.f);
                                                        enemy->save();
                                                        //                                                        continue;
                                                } else {
                                                        BRD_DEBUG_PRINT("DODGE 2\n");
                                                        if (velocity.y == 0.f) {
                                                                enemy->train_now(length(new_fire_dist) > length(
                                                                                                 fire_dist)
                                                                                                 ? 0.99f
                                                                                                 : 0.f);
                                                                enemy->save();
                                                                //                                                                continue;
                                                        }
                                                }
                                        } else {
                                                BRD_DEBUG_PRINT("2\n");
                                                switch (enemy->get_fire_dir()) {
                                                case Person::Down:
                                                        if (fire_dist.y > 0.f)
                                                                enemy->train_now(0.5f);
                                                        else
                                                                enemy->train_now(0.f);
                                                        break;
                                                case Person::Up:
                                                        if (fire_dist.y < 0.f)
                                                                enemy->train_now(0.5f);
                                                        else
                                                                enemy->train_now(0.f);
                                                        break;
                                                default:
                                                        enemy->train_now(0.f);
                                                        break;
                                                }
                                                enemy->save();
                                                //                                                continue;
                                        }

                                } else if (std::abs(fire_dist.y) < m_player->get_bounding_rect().height / 1.5f) {
                                        if ((env[center] == 3 || env[center - 1] == 3 || env[center - 2] == 3
                                             || env[center + 1] == 3 || env[center + 2] == 3)) {
                                                if (velocity.x == 0.f && velocity.y == 0.f) {
                                                        BRD_DEBUG_PRINT("DODGE 1\n");
                                                        enemy->train_now(-0.99f);
                                                        enemy->save();
                                                        //                                                        continue;
                                                } else {
                                                        BRD_DEBUG_PRINT("DODGE 2\n");
                                                        if (velocity.x == 0.f) {
                                                                enemy->train_now(length(new_fire_dist) > length(
                                                                                                 fire_dist)
                                                                                                 ? 0.99f
                                                                                                 : 0.f);
                                                                enemy->save();
                                                                //                                                                continue;
                                                        }
                                                }
                                        } else {
                                                BRD_DEBUG_PRINT("2\n");
                                                switch (enemy->get_fire_dir()) {
                                                case Person::Left:
                                                        if (fire_dist.x < 0.f)
                                                                enemy->train_now(0.5f);
                                                        else
                                                                enemy->train_now(0.f);
                                                        break;
                                                case Person::Right:
                                                        if (fire_dist.x > 0.f)
                                                                enemy->train_now(0.5f);
                                                        else
                                                                enemy->train_now(0.f);
                                                        break;
                                                default:
                                                        enemy->train_now(0.f);
                                                        break;
                                                }
                                                enemy->save();
                                                //                                                continue;
                                        }

                                } else if (env[center] == 3
                                           || env[center - n] == 3
                                           //                                           || env[center - 2 * n] == 3
                                           //                                           || env[center - 3 * n] == 3
                                           || env[center + n] == 3
                                           //                                           || env[center + 2 * n] == 3
                                           //                                           || env[center + 3 * n] == 3
                                           || env[center - 1] == 3
                                           //                                           || env[center - 2] == 3
                                           //                                           || env[center - 3] == 3
                                           || env[center + 1] == 3)
                                //                                           || env[center + 2] == 3
                                //                                           || env[center + 3] == 3)
                                {
                                        BRD_DEBUG_PRINT("DODGE 3\n");
                                        enemy->train_now(0.f);
                                        enemy->save();
                                        //                                        continue;
                                }


                                if (static_cast<bool>(velocity.x) || static_cast<bool>(velocity.y)) {
                                        if (new_dist > old_dist) {
                                                enemy->train_now(0.f);
                                        }

                                } else if (velocity.x == 0.f && velocity.y == 0.f) {
                                        enemy->train_now(0.f);
                                }


                                const float border_distance = 40.F;
                                if (new_enemy_pos.x >= (m_world_bounds.left + m_world_bounds.width - border_distance)
                                    || new_enemy_pos.x <= (m_world_bounds.left + border_distance)
                                    || new_enemy_pos.y >= (m_world_bounds.top + m_world_bounds.height - border_distance)
                                    || new_enemy_pos.y <= (m_world_bounds.top + border_distance)) {
                                        BRD_DEBUG_PRINT("BORDER\n");
                                        enemy->train_now(0.f);
                                }

                                enemy->save();
                        } else if (!enemy->is_active()) {
                                enemy->set_velocity(0.0F, 0.0F);
                                sf::Vector2f distance = player_position - enemy_pos;

                                if (std::abs(distance.x) < m_player->get_bounding_rect().width / 1.5f) {
                                        if (distance.y >= 0)
                                                enemy->on_command(m_commands[Commands::MoveDown], dt);
                                        else if (distance.y <= 0)
                                                enemy->on_command(m_commands[Commands::MoveUp], dt);
                                } else if (std::abs(distance.y) < m_player->get_bounding_rect().height / 1.5f) {
                                        if (distance.x >= 0)
                                                enemy->on_command(m_commands[Commands::MoveRight], dt);
                                        else if (distance.x <= 0)
                                                enemy->on_command(m_commands[Commands::MoveLeft], dt);
                                } else if (distance.x >= 0.f && distance.y <= 0.f) {
                                        enemy->on_command(m_commands[Commands::MoveRight], dt);
                                        enemy->on_command(m_commands[Commands::MoveUp], dt);
                                } else if (distance.x >= 0.f && distance.y >= 0.f) {
                                        enemy->on_command(m_commands[Commands::MoveRight], dt);
                                        enemy->on_command(m_commands[Commands::MoveDown], dt);
                                } else if (distance.x <= 0.f && distance.y >= 0.f) {
                                        enemy->on_command(m_commands[Commands::MoveLeft], dt);
                                        enemy->on_command(m_commands[Commands::MoveDown], dt);
                                } else if (distance.x <= 0.f && distance.y <= 0.f) {
                                        enemy->on_command(m_commands[Commands::MoveLeft], dt);
                                        enemy->on_command(m_commands[Commands::MoveUp], dt);
                                }


                                sf::Vector2f velocity = enemy->get_velocity();
                                velocity = unit_vector(velocity) * 15.f * static_cast<float>(m_wave_number);

                                // If moving diagonally, reduce velocity (to have always same velocity)
                                if (velocity.x != 0.f && velocity.y != 0.f)
                                        enemy->set_velocity(velocity / std::sqrt(2.f));

                                //                                auto new_enemy_pos = enemy_pos + velocity *
                                //                                dt.asSeconds();

                                if (velocity.x == 0.f && velocity.y == 0.f) {
                                        int num = random_int(4);
                                        switch (num) {
                                        case 0:
                                                enemy->on_command(m_commands[Commands::MoveDown], dt);
                                                break;
                                        case 1:
                                                enemy->on_command(m_commands[Commands::MoveUp], dt);
                                                break;
                                        case 2:
                                                enemy->on_command(m_commands[Commands::MoveLeft], dt);
                                                break;
                                        case 3:
                                                enemy->on_command(m_commands[Commands::MoveRight], dt);
                                                break;
                                        }
                                }

                                adapt_enemy_position(enemy);

                                continue;
                        }
                }
                m_countdown = sf::Time::Zero;
        }


        // Reset player velocity
        m_player->set_velocity(0.0F, 0.0F);
        // Forward commands to scene graph, adapt velocity (diagonal speed correction)
        while (!m_command_queue.is_empty()) {
                m_scene_graph.on_command(m_command_queue.pop(), dt);
        }
        adapt_player_velocity();

        //        // Collision detection and response (may destroy entities)
        //        handle_collisions();

        // Remove all enemies which request so
        auto wreck_field_begin = std::remove_if(m_active_enemies.begin(),
                                                m_active_enemies.end(),
                                                std::mem_fn(&SceneNode::is_marked_for_removal));
        m_active_enemies.erase(wreck_field_begin, m_active_enemies.end());

        m_scene_graph.remove_wrecks();

        // Spawn new enemy wave
        if (m_active_enemies.size() <= 0) {

                BRD_DEBUG_PRINT("WAVE\n");

                std::unique_ptr<TextNode> wave_display(new TextNode(m_fonts, ""));
                m_wave_display = wave_display.get();
                m_wave_display->set_string(std::to_string(m_wave_number) + "  WAVE!!! \n FIGHT!!! ");
                m_wave_display->setPosition(m_player->getPosition().x, m_player->getPosition().y - 150.f);
                m_wave_display->setScale(4.f, 4.f);

                m_scene_graph.attach_child(std::move(wave_display));
                m_is_wavetext_displayed = true;

                m_wave_number++;
                spawn_enemies();
        }

        if (m_is_wavetext_displayed) {
                m_wavetext_countdown += dt;

                m_wave_display->set_string(std::to_string(m_wave_number) + "  WAVE!!! \n FIGHT!!! ");
                m_wave_display->setPosition(m_player->getPosition().x, m_player->getPosition().y - 150.f);
                m_wave_display->setScale(4.f, 4.f);
                m_wave_display->set_color(sf::Color::Red);


                if (m_wavetext_countdown >= sf::seconds(5.f)) {
                        m_is_wavetext_displayed = false;
                        m_wavetext_countdown    = sf::Time::Zero;
                        m_wave_display->destroy();
                }
        }

        // Regular update step, adapt position (correct if outside view)
        m_scene_graph.update(dt, m_command_queue);
        adapt_player_position();

        m_living_enemies_counter = static_cast<unsigned>(m_number_of_enemies);
        for (auto enemy : m_active_enemies) {
                if (enemy->is_destroyed()) {
                        --m_living_enemies_counter;
                }
        }

        m_enemies_counter_display->set_string("Number of living enemies:  " + std::to_string(m_living_enemies_counter));
        //        m_enemies_counter_display->setPosition(m_world_view.getCenter() - m_world_view.getSize() / 2.2f);
        m_enemies_counter_display->setPosition(get_view_bounds().left + 35.f, get_view_bounds().top + 20.f);

        m_enemies_counter_display->set_color(sf::Color::Blue);

        m_game_objects.clear();
}

void
World::draw() {
        m_window.setView(m_world_view);
        m_window.draw(m_scene_graph);
}

CommandQueue&
World::get_command_queue() {
        return m_command_queue;
}

void
World::load_textures() {

        m_fonts.load(Fonts::Main, "media/Sansation.ttf");

        m_textures.load(Textures::Bullet, "media/img/Bullet.png");

        m_textures.load(Textures::MainCharacter, "media/img/characters/MainCharacter.png");
        m_textures.load(Textures::Bandit, "media/img/characters/Bandit.png");

        m_textures.load(Textures::HealthRefill, "media/img/pickups/HealthRefill.png");
        m_textures.load(Textures::SpeedBoost, "media/img/pickups/SpeedBoost.png");
}

void
World::build_scene() {
        BRD_DEBUG_PRINT(m_world_bounds.left,
                        "   ",
                        m_world_bounds.top,
                        '\n',
                        m_world_bounds.width,
                        "   ",
                        m_world_bounds.height,
                        '\n',
                        m_spawn_position.x,
                        "   ",
                        m_spawn_position.y,
                        '\n');

        Category::Type category = Category::Scene;
        SceneNode::Ptr layer(new SceneNode(category));
        m_scene_graph.attach_child(std::move(layer));

        std::unique_ptr<Person> player(new Person(Person::Player, m_textures, m_fonts));
        m_player = player.get();
        m_player->setPosition(m_spawn_position);
        m_scene_graph.attach_child(std::move(player));

        BRD_DEBUG_PRINT(get_battlefield_bounds().left,
                        "   ",
                        get_battlefield_bounds().top,
                        '\n',
                        get_battlefield_bounds().width,
                        "   ",
                        get_battlefield_bounds().height,
                        '\n');

        std::unique_ptr<TextNode> wave_display(new TextNode(m_fonts, ""));
        m_wave_display = wave_display.get();
        m_wave_display->set_string(std::to_string(m_wave_number) + "  WAVE!!! \n WAVE!!!");
        m_wave_display->setPosition(m_player->getPosition().x, m_player->getPosition().y - 150.f);
        m_wave_display->setScale(4.f, 4.f);

        m_scene_graph.attach_child(std::move(wave_display));
        m_is_wavetext_displayed = true;

        m_living_enemies_counter = static_cast<unsigned>(m_number_of_enemies);
        std::unique_ptr<TextNode> enemies_counter_display(new TextNode(m_fonts, ""));
        m_enemies_counter_display = enemies_counter_display.get();
        m_enemies_counter_display->set_string("Number of living enemies:  " + std::to_string(m_living_enemies_counter));
        //        m_enemies_counter_display->setPosition(m_world_view.getCenter() - m_world_view.getSize() / 2.2f);
        m_enemies_counter_display->setPosition(get_view_bounds().left + 35.f, get_view_bounds().top + 20.f);

        m_enemies_counter_display->set_color(sf::Color::Blue);

        m_scene_graph.attach_child(std::move(enemies_counter_display));

        // Add enemies
        add_enemies();
        m_foreground_manager.initialize();
        //        spawn_enemies();
}

void
World::adapt_player_position() {
        // Keep player's position inside the screen bounds, at least borderDistance units from the border
        sf::FloatRect view_bounds(m_world_view.getCenter() - m_world_view.getSize() / 2.0F, m_world_view.getSize());
        const float   border_distance = 40.0F;

        sf::Vector2f position = m_player->getPosition();
        position.x            = std::max(position.x, view_bounds.left + border_distance);
        position.x            = std::min(position.x, view_bounds.left + view_bounds.width - border_distance);
        position.y            = std::max(position.y, view_bounds.top + border_distance);
        position.y            = std::min(position.y, view_bounds.top + view_bounds.height - border_distance);
        m_player->setPosition(position);
}

void
World::adapt_player_velocity() {
        sf::Vector2f velocity = m_player->get_velocity();

        // If moving diagonally, reduce velocity (to have always same velocity)
        if (velocity.x != 0.0F && velocity.y != 0.0F) {
                m_player->set_velocity(velocity / std::sqrt(2.0F));
        }
}

bool
match_categories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2) {
        unsigned category1 = colliders.first->get_category();
        unsigned category2 = colliders.second->get_category();

        // Make sure first pair entry has category type1 and second has type2
        if (((type1 & category1) != 0U) && ((type2 & category2) != 0U)) {
                return true;
        }
        if (((type1 & category2) != 0U) && ((type2 & category1) != 0U)) {
                std::swap(colliders.first, colliders.second);
                return true;
        }
        return false;
}

void
World::handle_collisions() {
        std::set<SceneNode::Pair> collision_pairs;
        m_scene_graph.check_scene_collision(m_scene_graph, collision_pairs, m_game_objects);

        // TODO(G) Взаимодействие с Lvl object'om - для определения на какой план переместить спрайт.
        // TODO(G) Взаимодействие c AABB - .interact(Person).
        for (SceneNode::Pair pair : collision_pairs) {
                if (match_categories(pair, Category::Player, Category::Enemy)) {
                        auto& player = dynamic_cast<Person&>(*pair.first);
                        auto& enemy  = dynamic_cast<Person&>(*pair.second);

                        // Collision: Player damage = enemy's remaining HP
                        player.damage(1.f);
                        enemy.damage(1.f);
                }

                else if (match_categories(pair, Category::Player, Category::Pickup)) {
                        auto& player = dynamic_cast<Person&>(*pair.first);
                        auto& pickup = dynamic_cast<Pickup&>(*pair.second);

                        // Apply pickup effect to player, destroy projectile
                        pickup.apply(player);
                        pickup.destroy();
                }

                else if (match_categories(pair, Category::Enemy, Category::AlliedProjectile)
                         || match_categories(pair, Category::Player, Category::EnemyProjectile)) {
                        auto& person     = dynamic_cast<Person&>(*pair.first);
                        auto& projectile = dynamic_cast<Projectile&>(*pair.second);

                        person.damage(projectile.get_damage());
                        projectile.destroy();
                }

                else if (match_categories(pair, Category::LevelObject, Category::AlliedProjectile)
                         || match_categories(pair, Category::LevelObject, Category::EnemyProjectile)) {
                        auto& projectile = dynamic_cast<Projectile&>(*pair.second);
                        projectile.destroy();
                }

                //<<<<<<< HEAD
                ////                else if (match_categories(pair, Category::Player, Category::BoundingBox)
                ////                        || match_categories(pair, Category::Enemy, Category::BoundingBox)) {
                ////
                ////                        auto& player = dynamic_cast<Person&>(*pair.first);
                ////                        // TODO(G) Раскомментировать, когда добавлю опасные зоны..
                ////                        // auto& bounding_box = dynamic_cast<BoundingBox&>(*pair.second);
                ////                        // TODO (G) bounding_box
                ////                        BoundingBox::interact(player);
                ////
                ////                } else if (match_categories(pair, Category::Player, Category::LevelObject)) {
                ////=======
                //                else if (match_categories(pair, Category::Player, Category::BoundingBox)
                //                        || match_categories(pair, Category::Enemy, Category::BoundingBox)) {
                //=======
                else if (match_categories(pair, Category::Player, Category::BoundingBox)
                         || match_categories(pair, Category::Enemy, Category::BoundingBox)) {

                        auto& player       = dynamic_cast<Person&>(*pair.first);
                        auto& bounding_box = dynamic_cast<BoundingBox&>(*pair.second);
                        bounding_box.interact(player);
                }

                else if (match_categories(pair, Category::Player, Category::LevelObject)
                         || match_categories(pair, Category::Enemy, Category::LevelObject)) {

                        auto& player       = dynamic_cast<Person&>(*pair.first);
                        auto& level_object = dynamic_cast<LevelObject&>(*pair.second);
                        if (level_object.getPosition().y
                            > (player.getPosition().y + (player.get_bounding_rect().height / 2))) {
                                m_foreground_manager.switch_to_foreground(level_object);
                        }

                } else if (match_categories(pair, Category::Enemy, Category::Enemy)) {
                        auto& enemy1 = dynamic_cast<Person&>(*pair.first);
                        auto& enemy2 = dynamic_cast<Person&>(*pair.second);

                        auto enemy1_pos = enemy1.getPosition();
                        auto enemy2_pos = enemy2.getPosition();

                        auto enemy1_velocity = enemy1.get_velocity();
                        //                        auto enemy2_velocity = enemy2.get_velocity();


                        if (enemy1_pos.x > enemy2_pos.x && enemy1_velocity.x < 0.0F)  // двигается влево
                        {
                                enemy1.set_velocity(0.0F, 0.0F);
                                //                                enemy2.setPosition(enemy1_pos.x - 100.0F,
                                //                                enemy1_pos.y);
                                enemy2.set_velocity(enemy1_velocity.x - 100.0F, enemy1_velocity.y);
                        } else if (enemy1_pos.x < enemy2_pos.x && enemy1_velocity.x > 0.0F)  // двигается вправо
                        {
                                enemy1.set_velocity(0.0F, 0.0F);
                                //                                enemy2.setPosition(enemy1_pos.x + 100.0F,
                                //                                enemy1_pos.y);
                                enemy2.set_velocity(enemy1_velocity.x + 100.0F, enemy1_velocity.y);
                        }

                        //                        if (enemy2_pos.x > enemy1_pos.x && enemy2_velocity.x < 0.0F) //
                        //                        двигается влево
                        //                        {
                        //                                enemy1.set_velocity(enemy2_velocity.x - 100.0F,
                        //                                enemy2_velocity.y);
                        //                        }
                        //                        else if (enemy2_pos.x > enemy1_pos.x && enemy2_velocity.x >0.0F) //
                        //                        двигается вправо
                        //                        {
                        //                                enemy2.set_velocity(0.0F, 0.0F);
                        //                                enemy1.set_velocity(enemy2_velocity.x + 100.0F,
                        //                                enemy2_velocity.y);
                        //                        }

                        if (enemy1_pos.y > enemy2_pos.y && enemy1_velocity.y < 0.0F)  // двигается вверх
                        {
                                enemy1.set_velocity(0.0F, 0.0F);
                                enemy2.set_velocity(enemy1_velocity.x, enemy1_velocity.y - 100.0F);

                        } else if (enemy1_pos.y < enemy2_pos.y && enemy1_velocity.y > 0.0F)  // двигается вниз
                        {
                                enemy1.set_velocity(0.0F, 0.0F);
                                enemy2.set_velocity(enemy1_velocity.x, enemy1_velocity.y + 100.0F);
                        }

                        //                        if (enemy2_pos.y > enemy1_pos.y && enemy2_velocity.y < 0.0F) //
                        //                        двигается вверх
                        //                        {
                        //                                enemy2.set_velocity(0.0F, 0.0F);
                        //                                enemy1.set_velocity(enemy2_velocity.x, enemy2_velocity.y -
                        //                                100.0F);
                        //                        }
                        //                        else if (enemy2_pos.y < enemy1_pos.y && enemy2_velocity.y > 0.0F) //
                        //                        двигается вниз
                        //                        {
                        //                                enemy2.set_velocity(0.0F, 0.0F);
                        //                                enemy1.set_velocity(enemy2_velocity.x, enemy2_velocity.y +
                        //                                100.0F);
                        //                        }
                }
        }
}


void
World::add_enemies() {
        // Add enemies to the spawn point container by random positions
        // тут сделал так, чтобы вокруг игрока в радиусе генерились в противники в рандомных координатах
        std::random_device rd;
        std::mt19937       rng(rd());

        float player_defence_radius = get_battlefield_bounds().height;

        std::uniform_real_distribution<float> dist_left((-m_world_bounds.width) / 2, -player_defence_radius);
        std::uniform_real_distribution<float> dist_right(player_defence_radius, m_world_bounds.width / 2);
        std::uniform_real_distribution<float> dist_top(player_defence_radius, m_world_bounds.height / 2);
        std::uniform_real_distribution<float> dist_down((-m_world_bounds.height) / 2, -player_defence_radius);

        std::uniform_real_distribution<float> dist_height((-m_world_bounds.height) / 2, m_world_bounds.height / 2);
        std::uniform_real_distribution<float> dist_width((-m_world_bounds.width) / 2, m_world_bounds.width / 2);

        for (size_t i = m_number_of_enemies; i > 0; i--) {
                switch (i % 4) {
                case 0:
                        add_enemy(Person::Enemy, dist_left(rng), dist_height(rng));
                        break;
                case 1:
                        add_enemy(Person::Enemy, dist_right(rng), dist_height(rng));
                        break;
                case 2:
                        add_enemy(Person::Enemy, dist_width(rng), dist_down(rng));
                        break;
                case 3:
                        add_enemy(Person::Enemy, dist_width(rng), dist_top(rng));
                        break;
                }
        }


        // Sort all enemies according to their distance to the player, such that nearest enemies are checked first for
        // spawning
        std::sort(m_enemy_spawn_points.begin(),
                  m_enemy_spawn_points.end(),
                  [](const SpawnPoint& lhs, const SpawnPoint& rhs) {
                          return sqrt(lhs.x * lhs.x + lhs.y * lhs.y) < sqrt(rhs.x * rhs.x + rhs.y * rhs.y);
                  });


        while (!m_enemy_spawn_points.empty()) {
                SpawnPoint spawn = m_enemy_spawn_points.back();

                std::unique_ptr<Person> enemy(new Person(spawn.type, m_textures, m_fonts));
                enemy->setPosition(spawn.x, spawn.y);

                BRD_DEBUG_PRINT(spawn.x, "   ", spawn.y, '\n');

                m_active_enemies.push_back(enemy.get());
                m_scene_graph.attach_child(std::move(enemy));

                // Enemy is spawned, remove from the list to spawn
                m_enemy_spawn_points.pop_back();
        }
}

void
World::add_enemy(Person::Type type, float rel_x, float rel_y) {
        SpawnPoint spawn{ type, m_spawn_position.x + rel_x, m_spawn_position.y + rel_y };
        m_enemy_spawn_points.push_back(spawn);
}

void
World::spawn_enemies() {
        std::mt19937 rng(std::random_device{}());

        std::uniform_real_distribution dist_height{ 0.0F, m_world_bounds.height };
        std::uniform_real_distribution dist_width{ 0.0F, m_world_bounds.width };

        while (m_active_enemies.size() < m_number_of_enemies) {
                float x = dist_width(rng);
                float y = dist_height(rng);

                sf::FloatRect area = get_battlefield_bounds();

                while (area.contains(x, y)) {
                        x = dist_width(rng);
                        y = dist_height(rng);
                }


                std::unique_ptr<Person> enemy(new Person(Person::Enemy, m_textures, m_fonts));
                enemy->setPosition(x, y);

                BRD_DEBUG_PRINT(enemy->getPosition().x, "   ", enemy->getPosition().y, '\n');

                m_active_enemies.push_back(enemy.get());
                m_scene_graph.attach_child(std::move(enemy));
        }
}

sf::FloatRect
World::get_view_bounds() const {
        return sf::FloatRect(m_world_view.getCenter() - m_world_view.getSize() / 2.0F, m_world_view.getSize());
}


sf::FloatRect
World::get_battlefield_bounds() const {
        sf::FloatRect player_bounds = m_player->get_bounding_rect();
        float         side          = std::max(player_bounds.width, player_bounds.height);

        sf::RectangleShape rect(sf::Vector2f(side, side));
        rect.setOrigin(sf::Vector2f(side / 2.0F, side / 2.0F));
        rect.setScale(25.0F, 25.0F);
        rect.setPosition(m_player->get_world_position());

        return rect.getGlobalBounds();
}

sf::FloatRect
World::get_battlefield_bounds_2x() const {
        sf::FloatRect player_bounds = m_player->get_bounding_rect();
        float         side          = std::max(player_bounds.width, player_bounds.height);

        sf::RectangleShape rect(sf::Vector2f(side, side));
        rect.setOrigin(sf::Vector2f(side / 2.0F, side / 2.0F));
        rect.setScale(40.0F, 40.0F);
        rect.setPosition(m_player->get_world_position());

        return rect.getGlobalBounds();
}

sf::FloatRect
World::get_battlefield_bounds(Person* enemy) const {
        sf::FloatRect enemy_bounds = enemy->get_bounding_rect();
        float         side         = std::max(enemy_bounds.width, enemy_bounds.height);

        sf::RectangleShape rect(sf::Vector2f(side, side));
        rect.setOrigin(sf::Vector2f(side / 2.0F, side / 2.0F));
        rect.setScale(40.0F, 40.0F);
        rect.setPosition(enemy->get_world_position());

        return rect.getGlobalBounds();
}

bool
World::is_player_dead() {
        return m_player->is_destroyed();
}

void
World::adapt_enemy_position(Person* enemy) {
        // Keep enemy's position inside the screen bounds, at least borderDistance units from the border
        //        sf::FloatRect viewBounds(m_world_view.getCenter() - m_world_view.getSize() / 2.f,
        //        m_world_view.getSize());
        const float border_distance = 40.F;

        sf::Vector2f position = enemy->getPosition();
        position.x            = std::max(position.x, m_world_bounds.left + border_distance);
        position.x            = std::min(position.x, m_world_bounds.left + m_world_bounds.width - border_distance);
        position.y            = std::max(position.y, m_world_bounds.top + border_distance);
        position.y            = std::min(position.y, m_world_bounds.top + m_world_bounds.height - border_distance);
        enemy->setPosition(position);
}

std::vector<int>
World::get_enviroment(Person* enemy, size_t n) {
        sf::Vector2f position  = enemy->getPosition();
        auto         bot_bound = enemy->get_bounding_rect();
        auto         side      = bot_bound.height * 5.f;

        //        sf::RectangleShape shape(static_cast<sf::Vector2f>(sf::Vector2u(static_cast<size_t>(side) * (n),
        //        static_cast<size_t>(side) * (n))));
        sf::RectangleShape shape(sf::Vector2f((side) * static_cast<float>(n), (side) * static_cast<float>(n)));
        shape.setOrigin(sf::Vector2f((side) * static_cast<float>(n) / 2.f, (side) * static_cast<float>(n) / 2.f));
        shape.setPosition(position);
        auto bound = shape.getGlobalBounds();

        sf::FloatRect rect(position, sf::Vector2f(bound.height * 5.f, bound.height * 5.f));

        std::vector<int> env(n * n);

        sf::Vector2f player_pos = m_player->getPosition();
        for (float i = 0; i < static_cast<float>(n); i++) {
                float top = bound.top + i * side;
                for (float j = 0; j < static_cast<float>(n); j++) {
                        size_t ind = static_cast<size_t>(j + i * static_cast<float>(n));

                        float left = bound.left + j * side;

                        if ((left <= m_world_bounds.left && (left + side) > m_world_bounds.left)
                            || ((left < (m_world_bounds.left + m_world_bounds.width))
                                && ((left + side) >= (m_world_bounds.left + m_world_bounds.width)))
                            || (top <= m_world_bounds.top && (top + side) > m_world_bounds.top)
                            || ((top < (m_world_bounds.top + m_world_bounds.height))
                                && ((top + side) >= (m_world_bounds.top + m_world_bounds.height))))
                                env[ind] = 5;
                        else if (player_pos.x >= left && player_pos.x <= left + side && player_pos.y >= top
                                 && player_pos.y <= top + side)
                                env[ind] = 1;
                        else
                                for (auto obj : m_game_objects) {
                                        sf::Vector2f pos = obj->getPosition();
                                        if (pos.x >= left && pos.x <= left + side && pos.y >= top
                                            && pos.y <= top + side) {
                                                if (obj->get_category() == Category::AlliedProjectile)
                                                        env[ind] = 3;
                                                else if (obj->get_category() == Category::Enemy)
                                                        env[ind] = 2;
                                                else if (obj->get_category() == Category::LevelObject)
                                                        env[ind] = 4;
                                                else if (obj->get_category() == Category::BoundingBox)
                                                        env[ind] = 5;
                                                break;
                                        }
                                }
                }
        }

        return env;
}
