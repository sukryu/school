# 소스 파일 목록 정의
set(SOURCES
    "core/engine.cpp"
)

# 실행 파일 생성
add_executable(main ${SOURCES} "ui/display.cpp"  "core/game.cpp"  "ui/cursor.cpp" "core/map.cpp"  "ui/window/renderer.cpp" "ui/window/resource_bar.cpp" "ui/window/message_window.cpp" "ui/window/command_window.cpp" "ui/window/status_window.cpp" "managers/terrain_manager.cpp" "managers/unit_manager.cpp" "managers/building_manager.cpp" "ui/window/map_renderer.cpp" "ui/window/base_window.cpp" "core/io.cpp" "utils/utils.cpp"   "spatial/quad_tree.cpp" "entity/unit.cpp" "entity/building.cpp" "entity/terrain.cpp"  "entity/sandworm_state.cpp" "entity/sandworm_ai.cpp"   "entity/harvester_ai.cpp" "entity/harvester_state.cpp" "entity/combat_unit_state.cpp" "entity/combat_unit_ai.cpp")

# 필요한 경우 라이브러리 링크
# target_link_libraries(MyExecutable PRIVATE SomeLibrary)
