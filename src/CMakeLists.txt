# 소스 파일 목록 정의
set(SOURCES
    engine.cpp
)

# 실행 파일 생성
add_executable(main ${SOURCES} "display.cpp" "io.cpp" "game.cpp"  "cursor.cpp" "map.cpp"  "renderer.cpp" "resource_bar.cpp" "message_window.cpp" "command_window.cpp" "status_window.cpp" "terrain_manager.cpp" "unit_manager.cpp" "building_manager.cpp" "map_renderer.cpp")

# 필요한 경우 라이브러리 링크
# target_link_libraries(MyExecutable PRIVATE SomeLibrary)
