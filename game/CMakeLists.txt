cmake_minimum_required(VERSION 3.26)

set(SOURCES
    src/main/main.cpp
)

set(HEADERS
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(asteracity ${SOURCES} ${HEADERS})
add_executable(Asteracity ALIAS asteracity)

target_include_directories(asteracity
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(asteracity PUBLIC cxx_std_20)

target_link_libraries(asteracity PRIVATE Asteracity::Engine)

assetc_compile(asteracity
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets"
)