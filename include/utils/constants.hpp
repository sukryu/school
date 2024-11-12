#pragma once

namespace dune {
	namespace constants {
        // 시간 관련
        constexpr int TICK = 10;  // ms
        constexpr int DOUBLE_CLICK_INTERVAL = 100; // ms

        // 맵 관련
        constexpr int N_LAYER = 2;
        constexpr int MAP_WIDTH = 60;
        constexpr int MAP_HEIGHT = 18;

        // UI 관련
        constexpr int RESOURCE_HEIGHT = 1;
        constexpr int SYSTEM_MESSAGE_HEIGHT = 10;
        constexpr int DEFAULT_STATUS_WIDTH = 30;

        // 게임 관련
        constexpr int DEFAULT_HEALTH = 100;
        constexpr int SANDSTORM_SIZE = 2;
        constexpr int SANDSTORM_DAMAGE = 50;
        constexpr int SANDWORM_SPEED = 10000;
        constexpr int HARVESTER_SPEED = 2000;

        // 색상 관련
        namespace color {
            constexpr int DEFAULT = 7;        // White (흰색 글자)
            constexpr int CURSOR = 112;       // 커서 (흰색 배경에 검은 글자)
            constexpr int RESOURCE = 112;
            constexpr int ART_LADIES = 9;     // Blue
            constexpr int HARKONNEN = 12;     // Red
            constexpr int SANDWORM = 14;      // Yellow
            constexpr int PLATE = 0;          // Black
            constexpr int SPICE = 6;          // Orange
            constexpr int OTHER = 8;          // Gray
            constexpr int DESERT = 96;        // 연한 노란색 배경 (96 = 노란색 배경(6<<4) + 검은색 글자(0))
        } // namespace color
	} // namespace constants
} // namespace dune
