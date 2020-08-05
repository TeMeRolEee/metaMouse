cmake_minimum_required(VERSION 3.16)
project(metaMouse)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OM)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

add_executable(metaMouse main.cpp)

# linking
target_link_libraries(metaMouse Qt5::Core Qt5::Widgets Qt5::Gui)
