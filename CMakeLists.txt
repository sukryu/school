cmake_minimum_required(VERSION 3.14)

# 프로젝트 이름과 언어 설정
project(school LANGUAGES CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include 디렉토리를 포함 경로에 추가
include_directories(${CMAKE_SOURCE_DIR}/include)

# 하위 디렉토리 추가
add_subdirectory(src)
