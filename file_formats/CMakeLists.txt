cmake_minimum_required(VERSION 3.26)

set(SOURCES
    src/binary/binary_reader.cpp
    src/binary/binary_writer.cpp
)

set(HEADERS
    include/file_formats/binary/binary_reader.h
    include/file_formats/binary/binary_writer.h

    include/file_formats/common/asset_header.h
    include/file_formats/common/resource.h

    include/file_formats/sprite.h
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(asteracity-file-formats STATIC ${SOURCES} ${HEADERS})
add_library(Asteracity::FileFormats ALIAS asteracity-file-formats)

target_include_directories(asteracity-file-formats
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(asteracity-file-formats PUBLIC cxx_std_20)