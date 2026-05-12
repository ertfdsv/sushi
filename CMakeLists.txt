cmake_minimum_required(VERSION 3.16)

project(sushi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(sushi
    main.cpp
    mainwindow.cpp
    SushiItem.cpp
    mainwindow.h
    SushiItem.h
)

target_link_libraries(sushi PRIVATE Qt6::Widgets)
