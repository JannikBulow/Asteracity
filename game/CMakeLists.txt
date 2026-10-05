cmake_minimum_required(VERSION 3.26)

set(SOURCES
    src/character/character_controller.cpp

    src/main/main.cpp

    src/player/player.cpp
    src/player/player_input.cpp

    src/game.cpp
)

set(HEADERS
    include/asteracity/character/character_controller.h
    include/asteracity/character/character_intent.h
    include/asteracity/character/character_stats.h

    include/asteracity/player/player.h
    include/asteracity/player/player_input.h
    include/asteracity/tile/registry.h

    include/asteracity/game.h
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(asteracity ${SOURCES} ${HEADERS})
add_executable(Asteracity ALIAS asteracity)

target_include_directories(asteracity
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(asteracity PUBLIC cxx_std_20)

target_link_libraries(asteracity PRIVATE Asteracity::Engine Asteracity::Addon::Input)

assetc_compile(asteracity
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets"
)

target_compile_options(asteracity PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
    $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
)

target_link_options(asteracity PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)