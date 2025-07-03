cmake_minimum_required(VERSION 3.16)
project(pomodoro-timer)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(pomodoro-timer src/main.cpp)
target_link_libraries(pomodoro-timer Qt6::Core Qt6::Widgets)
