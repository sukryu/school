#include "../include/core/map.hpp"
#include "../include/ui/window/message_window.hpp"
#include "../include/utils/utils.hpp"

namespace dune {
    namespace core {

        Map::Map(int width, int height, ui::MessageWindow* msg_window)
            : width(width)
            , height(height)
            , terrain_manager(width, height)
            , unit_manager()
            , building_manager()
            , message_window(msg_window) {}

        void Map::update(std::chrono::milliseconds current_time) {
            // 각 유닛 타입별로 업데이트
            for (const auto& unit : unit_manager.get_units()) {
                switch (unit->get_type()) {
                case types::UnitType::Sandworm:
                    update_sandworm(unit.get(), current_time);
                    break;
                //case types::UnitType::Harvester:
                //    update_harvester(unit.get(), current_time);  // 아직 미구현
                //    break;
                    // 나중에 다른 유닛들의 update 로직 추가
                default:
                    // 일반 유닛들의 기본 동작 (이동, 전투 등)
                    break;
                }
            }
        }

        void Map::add_unit(std::unique_ptr<managers::UnitManager::Unit> unit) {
            unit_manager.add_unit(std::move(unit));
        }

        void Map::add_building(std::unique_ptr<managers::BuildingManager::Building> building) {
            building_manager.add_building(std::move(building));
        }

        void Map::set_terrain(const types::Position& pos, types::TerrainType type) {
            terrain_manager.set_terrain(pos, type);
        }

        types::Position Map::find_nearest_unit(const types::Position& from_pos, types::UnitType exclude_type) {
            types::Position nearest_pos = from_pos;
            int min_distance = std::numeric_limits<int>::max();

            for (const auto& unit : unit_manager.get_units()) {
                // 자기 자신이나 같은 타입은 제외
                if (unit->get_type() == exclude_type) {
                    continue;
                }

                // 잡아먹을 수 없는 유닛은 제외
                if (!is_valid_sandworm_target(unit.get())) {
                    continue;
                }

                types::Position unit_pos = unit->get_position();
                int distance = utils::manhattan_distance(unit_pos, from_pos);

                if (distance < min_distance) {
                    min_distance = distance;
                    nearest_pos = unit_pos;
                }
            }

            return nearest_pos;
        }

        void Map::remove_unit(Unit* unit) {
            if (unit) {
                unit_manager.remove_unit(unit);
            }
        }

        void Map::damage_building_at(const types::Position& pos, int damage) {
            if (auto* building = building_manager.get_building_at(pos)) {
                building->take_damage(damage);
                if (building->is_destroyed()) {
                    add_system_message(L"The building was destroyed!");
                }
            }
        }

        void Map::remove_destroyed_buildings() {
            building_manager.remove_destroyed_buildings();
        }

        void Map::add_system_message(const std::wstring& message) {
            if (message_window) {
                message_window->add_message(message);
            }
        }

        void Map::update_sandworm(Unit* sandworm, std::chrono::milliseconds current_time) {
            if (!sandworm->is_ready_to_move(current_time)) {
                return;
            }

            // 디버깅용 로그 추가
            add_system_message(L"Attempting to move the sandworm...");

            // 가장 가까운 유닛 찾기
            types::Position target_pos = find_nearest_unit(sandworm->get_position(), types::UnitType::Sandworm);

            // 목표 확인용 로그
            if (auto* target = get_entity_at<Unit>(target_pos)) {
                add_system_message(L"Target found:" + get_unit_type_name(target->get_type()));
            }

            // 대상 위치로 이동
            if (target_pos != sandworm->get_position()) {
                types::Position new_pos = calculate_sandworm_move(sandworm, target_pos);
                // 이동 확인용 로그
                add_system_message(L"movement: (" +
                    std::to_wstring(sandworm->get_position().row) + L"," +
                    std::to_wstring(sandworm->get_position().column) + L") -> (" +
                    std::to_wstring(new_pos.row) + L"," +
                    std::to_wstring(new_pos.column) + L")");

                sandworm->move_to(new_pos);

                // 새 위치에서 유닛 확인
                if (Unit* target_unit = get_entity_at<Unit>(new_pos)) {
                    if (is_valid_sandworm_target(target_unit)) {
                        // 유닛 잡아먹기
                        sandworm->consume_target();
                        add_system_message(L"Sandworm " + get_unit_type_name(target_unit->get_type()) +
                            L"caught it!");
                        remove_unit(target_unit);

                        // 스파이스 생성 여부 확인
                        if (sandworm->should_excrete()) {
                            sandworm->excrete();
                            set_terrain(new_pos, types::TerrainType::Spice);
                            add_system_message(L"The sandworm left behind some spice.");
                        }
                    }
                }
            }

            sandworm->update_last_move_time(current_time);
        }

        types::Position Map::calculate_sandworm_move(const Unit* sandworm, const types::Position& target_pos) {
            types::Position current_pos = sandworm->get_position();
            types::Position next_pos = current_pos;

            // 수평 거리와 수직 거리 계산
            int dx = target_pos.column - current_pos.column;
            int dy = target_pos.row - current_pos.row;

            // 더 긴 거리 방향으로 이동 시도
            if (abs(dx) > abs(dy)) {
                // 수평 이동 시도
                types::Position try_pos = current_pos;
                try_pos.column += (dx > 0) ? 1 : -1;

                // 바위를 만나면 수직 이동 시도
                if (terrain_manager.get_terrain(try_pos).get_type() == types::TerrainType::Rock) {
                    try_pos = current_pos;
                    try_pos.row += (dy > 0) ? 1 : -1;
                }
                next_pos = try_pos;
            }
            else {
                // 수직 이동 시도
                types::Position try_pos = current_pos;
                try_pos.row += (dy > 0) ? 1 : -1;

                // 바위를 만나면 수평 이동 시도
                if (terrain_manager.get_terrain(try_pos).get_type() == types::TerrainType::Rock) {
                    try_pos = current_pos;
                    try_pos.column += (dx > 0) ? 1 : -1;
                }
                next_pos = try_pos;
            }

            return next_pos;
        }

        bool Map::is_valid_sandworm_target(const Unit* target) const {
            if (!target) return false;
            auto target_type = target->get_type();
            return target_type != types::UnitType::Sandworm &&
                target_type != types::UnitType::HeavyTank;
        }

        std::wstring Map::get_unit_type_name(types::UnitType type) const {
            switch (type) {
            case types::UnitType::Harvester: return L"Harvester";
            case types::UnitType::Fremen: return L"Fremen";
            case types::UnitType::Soldier: return L"Soldier";
            case types::UnitType::Fighter: return L"Fighter";
            default: return L"unit";
            }
        }

    } // namespace core
} // namespace dune