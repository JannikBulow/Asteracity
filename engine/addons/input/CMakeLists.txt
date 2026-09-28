set(SOURCES
)

set(HEADERS
    include/engine/addons/input/components.h
    include/engine/addons/input/systems.h
    include/engine/addons/input/binding.h
    src/a.cpp
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(asteracity-addon-input STATIC ${SOURCES} ${HEADERS})
add_library(Asteracity::Addon::Input ALIAS asteracity-addon-input)

target_include_directories(asteracity-addon-input
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(asteracity-addon-input PUBLIC cxx_std_20)

target_link_libraries(asteracity-addon-input PUBLIC asteracity-engine)