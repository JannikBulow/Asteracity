cmake_minimum_required(VERSION 3.26)

set(HEADERS
    include/engine/addons/input/system.h
    include/engine/addons/input/binding.h
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${HEADERS})

add_library(asteracity-addon-input INTERFACE ${HEADERS})
add_library(Asteracity::Addon::Input ALIAS asteracity-addon-input)

target_include_directories(asteracity-addon-input
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(asteracity-addon-input INTERFACE cxx_std_20)

target_link_libraries(asteracity-addon-input INTERFACE asteracity-engine)