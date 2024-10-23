#include "display.hpp"
#include "io.hpp"

Display::Display(int map_width, int map_height, int status_width)
    : map_width(map_width), map_height(map_height), status_width(status_width),
    total_width(map_width + status_width),
    total_height(RESOURCE_HEIGHT + map_height + SYSTEM_MESSAGE_HEIGHT)
{
    // 전체 필요한 크기로 버퍼 초기화
    backbuf.resize(total_height, std::vector<char>(total_width, ' '));
    frontbuf.resize(total_height, std::vector<char>(total_width, ' '));
    colorbuf.resize(total_height, std::vector<int>(total_width, COLOR_DEFAULT));
}

void Display::draw_border(int y, int x, int width, int height) {
    // 경계 검사 추가
    if (y < 0 || y + height > total_height || x < 0 || x + width > total_width) {
        return;  // 버퍼 범위를 벗어나면 그리지 않음
    }

    // 가로 테두리
    for (int j = x; j < x + width; j++) {
        backbuf[y][j] = '#';
        backbuf[y + height - 1][j] = '#';
    }

    // 세로 테두리
    for (int i = y; i < y + height; i++) {
        backbuf[i][x] = '#';
        backbuf[i][x + width - 1] = '#';
    }
}

void Display::update(const Resource& resource, const Map& map, const Cursor& cursor) {
    // 백버퍼 초기화
    for (int i = 0; i < total_height; ++i) {
        std::fill(backbuf[i].begin(), backbuf[i].end(), ' ');
    }

    // 리소스 정보 표시 (0번 줄)
    display_resource(resource);

    // 맵과 상태창 테두리 그리기 (1번 줄부터)
    draw_border(RESOURCE_HEIGHT, 0, map_width, map_height);
    draw_border(RESOURCE_HEIGHT, map_width, status_width, map_height);

    // 시스템 메시지창과 명령창 테두리 그리기
    int message_start_y = RESOURCE_HEIGHT + map_height;
    draw_border(message_start_y, 0, map_width, SYSTEM_MESSAGE_HEIGHT);
    draw_border(message_start_y, map_width, status_width, COMMAND_HEIGHT);

    // 맵 내용 업데이트
    display_map(map);

    // 나머지 창 업데이트
    display_status_window();
    display_system_messages();
    display_command_window();

    // 커서 업데이트
    display_cursor(cursor);

    // 화면 갱신
    update_screen();
}


void Display::display_resource(const Resource& resource) {
    std::string resource_info = "spice = " + std::to_string(resource.spice) + "/" +
        std::to_string(resource.spice_max) + ", population = " +
        std::to_string(resource.population) + "/" +
        std::to_string(resource.population_max);

    // 리소스 정보는 최상단에 표시
    for (size_t i = 0; i < resource_info.length(); i++) {
        backbuf[0][i] = resource_info[i];
        colorbuf[0][i] = COLOR_RESOURCE;
    }
}

void Display::display_map(const Map& map) {
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            char tile = ' ';
            int color = COLOR_DEFAULT;

            // 1. 테두리 확인
            auto border_tile = map.get_tile(0, { i, j });
            if (border_tile && *border_tile == '#') {
                tile = '#';
            }
            // 2. 테두리가 아닌 경우 내부 처리
            else {
                // 실제 맵 좌표로 변환
                Position map_pos = { i - 1, j - 1 };

                // 지형 확인
                const Terrain& terrain = map.get_terrain(map_pos);
                if (terrain.get_type() != TerrainType::Desert) {
                    tile = terrain.get_representation();
                    color = terrain.get_color();
                }

                // 건물 확인
                auto building = map.get_building_at(map_pos);
                if (building) {
                    tile = building->get_representation();
                    color = building->get_color();
                }

                // 유닛 확인
                auto unit = map.get_unit_at(map_pos);
                if (unit) {
                    tile = unit->get_representation();
                    color = unit->get_color();
                }
            }

            // 리소스 표시줄 고려
            backbuf[i + 1][j] = tile;
            colorbuf[i + 1][j] = color;
        }
    }
}

void Display::display_cursor(const Cursor& cursor) {
    Position prev = cursor.get_previous_position();
    Position curr = cursor.get_current_position();

    // 이전 커서 위치의 원래 내용 복원
    if (prev.row >= 0 && prev.row < map_height - 2 &&
        prev.column >= 0 && prev.column < map_width - 2) {
        char prev_ch = backbuf[prev.row + 2][prev.column + 1];
        int prev_color = colorbuf[prev.row + 2][prev.column + 1];
        frontbuf[prev.row + 2][prev.column + 1] = ' ';  // 강제로 업데이트하기 위해
        IO::printc({ prev.row + 2, prev.column + 1 }, prev_ch, prev_color);
    }

    // 현재 커서 위치의 내용 저장 및 커서 표시
    if (curr.row >= 0 && curr.row < map_height - 2 &&
        curr.column >= 0 && curr.column < map_width - 2) {
        char curr_ch = backbuf[curr.row + 2][curr.column + 1];
        frontbuf[curr.row + 2][curr.column + 1] = ' ';  // 강제로 업데이트하기 위해
        IO::printc({ curr.row + 2, curr.column + 1 }, curr_ch, COLOR_CURSOR);
    }
}

void Display::display_status_window() {
    const int start_x = map_width + 1;
    const std::string title = "Selected: None";

    for (size_t i = 0; i < title.length(); i++) {
        backbuf[2][start_x + 2 + i] = title[i];
        colorbuf[2][start_x + 2 + i] = COLOR_DEFAULT;
    }
}

void Display::display_system_messages() {
    const int start_y = map_height + 1;
    int current_y = start_y + 1;

    for (int i = static_cast<int>(system_messages.size()) - 1;
        i >= 0 && i >= static_cast<int>(system_messages.size()) - 3;
        i--, current_y++) {
        const std::string& msg = system_messages[i];
        for (size_t x = 0; x < msg.length() && x < static_cast<size_t>(map_width - 2); x++) {
            backbuf[current_y][x + 2] = msg[x];
            colorbuf[current_y][x + 2] = COLOR_DEFAULT;
        }
    }
}

void Display::display_command_window() {
    const int start_x = map_width + 1;
    const int start_y = map_height + 1;
    const std::vector<std::string> commands = {
        "Commands:",
        "B: Build",
        "T: Train",
        "Q: Quit"
    };

    for (size_t i = 0; i < commands.size(); i++) {
        const std::string& cmd = commands[i];
        for (size_t j = 0; j < cmd.length(); j++) {
            backbuf[start_y + 1 + i][start_x + 2 + j] = cmd[j];
            colorbuf[start_y + 1 + i][start_x + 2 + j] = COLOR_DEFAULT;
        }
    }
}

void Display::update_screen() {
    // 경계 검사를 포함한 화면 갱신
    for (int i = 0; i < total_height; i++) {
        for (int j = 0; j < total_width; j++) {
            if (backbuf[i][j] != frontbuf[i][j]) {
                IO::printc({ i, j }, backbuf[i][j], colorbuf[i][j]);
                frontbuf[i][j] = backbuf[i][j];
            }
        }
    }
}

void Display::add_system_message(const std::string& message) {
    system_messages.push_back(message);
    if (system_messages.size() > 5) {  // 최대 5개 메시지 유지
        system_messages.erase(system_messages.begin());
    }
}

void Display::add_command_message(const std::string& message) {
    command_messages.push_back(message);
    if (command_messages.size() > 5) {  // 최대 5개 메시지 유지
        command_messages.erase(command_messages.begin());
    }
}