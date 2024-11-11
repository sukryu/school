#pragma once
#include "../core/entity.hpp"
#include "../utils/types.hpp"
#include <vector>

namespace dune {
	namespace managers {
        class TerrainManager {
        public:
            class Terrain : public core::Entity {
            public:
                explicit Terrain(types::TerrainType type = types::TerrainType::Desert);

                // Entity 인터페이스 구현
                wchar_t get_representation() const override;
                int get_color() const override;
                void print_info() const override;
                types::Position get_position() const override { return pos; }
                void move(types::Direction d) override {} // 지형은 이동 불가

                // Terrain 고유 메서드
                bool is_buildable() const;
                bool is_walkable() const;
                bool can_harvest_spice() const;
                types::TerrainType get_type() const { return type; }

            private:
                types::TerrainType type;
                types::Position pos;
            };

            TerrainManager(int width, int height);

            const Terrain& get_terrain(const types::Position& pos) const;
            void set_terrain(const types::Position& pos, types::TerrainType type);
            bool is_valid_position(const types::Position& pos) const;

        private:
            int width;
            int height;
            std::vector<std::vector<Terrain>> terrain_map;
        };
	} // namespace managers
} // namespace dune