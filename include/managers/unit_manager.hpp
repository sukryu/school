#pragma once
#include "../core/entity.hpp"
#include "../utils/types.hpp"
#include "../managers/unit_manager.hpp"
#include "../managers/building_manager.hpp"
#include <memory>
#include <vector>
#include <chrono>

namespace dune {
	namespace core { class Map; } // 전방 선언

	namespace managers {
        class UnitManager {
        public:
            class Unit : public core::Entity {
            public:
                // 일반 유닛용 생성자
                Unit(types::UnitType type, int build_cost, int population, int speed,
                    int attack_power, int health, int sight_range, types::Position pos);

                // 특수 유닛용 생성자 (Sandworm & air unit)
                Unit(types::UnitType type, types::Position pos);

                // Entity 인터페이스 구현
                char get_representation() const override;
                int get_color() const override;
                void print_info() const override;
                types::Position get_position() const override { return pos; }
                void move(types::Direction d) override;

                // Unit 고유 메서드
                static constexpr int SANDSTORM_SIZE = 2;
                void check_and_damage_area(core::Map& map, const types::Position& pos);
                types::UnitType get_type() const { return type; }
                void update(std::chrono::milliseconds current_time, core::Map& map);
                void move_towards(const types::Position& target);
                void consume_unit(Unit& target_unit);
                void excrete(core::Map& map);
                bool is_air_unit() const;

                // 게임 플레이 관련 메서드
                int get_health() const { return health; }
                int get_attack_power() const { return attack_power; }
                int get_sight_range() const { return sight_range; }
                bool is_ready_to_move(std::chrono::milliseconds current_time) const;

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
                std::chrono::milliseconds creation_time{ 0 };
            };

            void add_unit(std::unique_ptr<Unit> unit);
            Unit* get_unit_at(const types::Position& pos);
            const Unit* get_unit_at(const types::Position& pos) const;
            void remove_unit(Unit* unit);
            const std::vector<std::unique_ptr<Unit>>& get_units() const;
            void update_all_units(std::chrono::milliseconds current_time, core::Map& map);

        private:
            std::vector<std::unique_ptr<Unit>> units;
        };
	} // namespace managers
} // namespace dune