#include "../include/core/game.hpp"
#include <locale>

using namespace dune::core;

int main() {
    // 콘솔 출력 코드 페이지를 UTF-8로 설정
    SetConsoleOutputCP(CP_UTF8);

    // 콘솔 입력 코드 페이지를 UTF-8로 설정 (필요한 경우)
    SetConsoleCP(CP_UTF8);

    // 로케일 설정 (유니코드 출력 지원)
    std::locale::global(std::locale(""));

    // 유니코드 출력 시 BOM(Byte Order Mark) 방지를 위해 널 문자 설정
    std::wcout.imbue(std::locale(""));

    // 프로그램 실행 코드
    Game game;
    game.run();

    return 0;
}
