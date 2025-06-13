cmake_minimum_required(VERSION 3.10)
project(game_of_life)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")

# Для отладки
set(CMAKE_BUILD_TYPE Debug)

add_executable(game_of_life ${SOURCES} ${HEADERS})

target_include_directories(game_of_life PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include 
)

if(NOT DEFINED SDL2_DIR)
    message(STATUS "Переменная SDL2_DIR не задана, попробуем найти SDL2 стандартным способом")
else()
    list(APPEND CMAKE_PREFIX_PATH "${SDL2_DIR}")
endif()

# Автоматический поиск SDL2
find_package(SDL2 REQUIRED CONFIG)

if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 не найден! Укажите SDL2_DIR или установите SDL2.")
endif()

# Добавляем include dirs SDL2
target_include_directories(game_of_life PRIVATE ${SDL2_INCLUDE_DIRS})

# Линкуем SDL2 библиотеки
target_link_libraries(game_of_life PRIVATE ${SDL2_LIBRARIES})