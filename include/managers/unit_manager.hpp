#pragma once
#include "../core/entity.hpp"
#include "../utils/types.hpp"
#include <memory>
#include <vector>
#include <chrono>

namespace dune {
    namespace managers {

        class UnitManager {
        public:
            class Unit : public core::Entity {
            public:
                // 일반 유닛용 생성자
                Unit(types::UnitType type, int build_cost, int population, int speed,
                    int attack_power, int health, int sight_range, types::Position pos);

                // 특수 유닛용 생성자 (Sandworm)
                Unit(types::UnitType type, types::Position pos);

                // Entity 인터페이스 구현
                wchar_t get_representation() const override;
                int get_color() const override;
                void print_info() const override;
                types::Position get_position() const override { return pos; }
                void move(types::Direction d) override;

                // Unit 고유 메서드
                types::UnitType get_type() const { return type; }
                int get_health() const { return health; }
                int get_attack_power() const { return attack_power; }
                int get_sight_range() const { return sight_range; }
                int get_speed() const { return speed; }
                bool is_ready_to_move(std::chrono::milliseconds current_time) const {
                    return current_time - last_move_time >= std::chrono::milliseconds(speed);
                }
                void update_last_move_time(std::chrono::milliseconds current_time) {
                    last_move_time = current_time;
                }
                void move_to(const types::Position& new_pos) { pos = new_pos; }
                void consume_target() { length += 1; }
                bool can_excrete() const { return length > 1; }
                void excrete() { if (length > 1) length -= 1; }
                bool should_excrete() const { return length > 1 && (rand() % 100 < 30); }

            private:
                types::UnitType type;
                int build_cost;
                int population;
                int speed;
                int attack_power;
                int health;
                int sight_range;
                types::Position pos;
                int length = 1;  // 샌드웜 길이
                std::chrono::milliseconds last_move_time{ 0 };
            };

            void add_unit(std::unique_ptr<Unit> unit);
            Unit* get_unit_at(const types::Position& pos);
            const Unit* get_unit_at(const types::Position& pos) const;
            void remove_unit(Unit* unit);
            const std::vector<std::unique_ptr<Unit>>& get_units() const;

        private:
            std::vector<std::unique_ptr<Unit>> units;
        };
    } // namespace managers
} // namespace dune