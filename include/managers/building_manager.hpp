#pragma once
#include "../core/entity.hpp"
#include "../utils/types.hpp"
#include <memory>
#include <vector>
#include <string>

namespace dune {
	namespace managers {
        class BuildingManager {
        public:
            class Building : public core::Entity {
            public:
                Building(types::Camp type, std::string name, std::string description,
                    int build_cost, types::Position pos, int width, int height,
                    types::UnitType produced_unit);

                // Entity 인터페이스 구현
                char get_representation() const override;
                int get_color() const override;
                void print_info() const override;
                types::Position get_position() const override { return pos; }
                void move(types::Direction d) override;

                // Building 고유 메서드
                bool contains(const types::Position& p) const;
                types::Camp get_type() const { return type; }
                const std::string& get_name() const { return name; }
                const std::string& get_description() const { return description; }
                int get_height() const { return height; }
                int get_width() const { return width; }
                int get_health() const { return health; }
                void take_damage(int damage);
                bool is_destroyed() const { return health <= 0; }
                types::UnitType get_produced_unit() const { return produced_unit; }

            private:
                types::Camp type;
                std::string name;
                std::string description;
                int building_cost;
                types::Position pos;
                int width;
                int height;
                types::UnitType produced_unit;
                int health;
            };

            void add_building(std::unique_ptr<Building> building);
            Building* get_building_at(const types::Position& pos);
            const Building* get_building_at(const types::Position& pos) const;
            void remove_building(Building* building);
            const std::vector<std::unique_ptr<Building>>& get_buildings() const;
            void remove_destroyed_buildings();

        private:
            std::vector<std::unique_ptr<Building>> buildings;
        };
	} // namespace managers
} // namespace dune