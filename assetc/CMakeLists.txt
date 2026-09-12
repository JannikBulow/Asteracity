cmake_minimum_required(VERSION 3.26)

include(FetchContent)

FetchContent_Declare(vlex
    GIT_REPOSITORY https://github.com/JannikBulow/vlex
    GIT_TAG master
)
FetchContent_MakeAvailable(vlex)

set(SOURCES
    src/parser/common.cpp
    src/parser/sprite_parser.cpp

    src/main.cpp
)

set(HEADERS
    include/assetc/parser/common.h
    include/assetc/parser/sprite_parser.h
    include/assetc/parser/token_stream.h

    include/assetc/util/exceptions.h
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(asteracity-assetc ${SOURCES} ${HEADERS})
add_executable(Asteracity::assetc ALIAS asteracity-assetc)

target_include_directories(asteracity-assetc
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(asteracity-assetc PUBLIC cxx_std_20)

target_link_libraries(asteracity-assetc PRIVATE asteracity-file-formats)

vlex_generate(asteracity-assetc src/lexer/lexer.vlex src/lexer include/assetc/lexer FILE_NAME_STYLE snake_case)

function(assetc_compile TARGET)
    set(options)
    set(oneValueArgs SOURCE_DIR OUTPUT_DIR)
    set(multiValueArgs)

    cmake_parse_arguments(
        ASSETS
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    if (NOT ASSETS_OUTPUT_DIR)
        message(FATAL_ERROR "missing OUTPUT_DIR")
    endif()

    file(GLOB_RECURSE ASSET_FILES
        CONFIGURE_DEPENDS
        "${ASSETS_SOURCE_DIR}/*"
    )

    set(OUTPUT_FILES)

    foreach(SOURCE IN LISTS ASSET_FILES)
        if(IS_DIRECTORY "${SOURCE}")
            continue()
        endif()

        file(RELATIVE_PATH RELATIVE_SOURCE
            "${ASSETS_SOURCE_DIR}"
            "${SOURCE}"
        )

        if(RELATIVE_SOURCE MATCHES "\\.source$")
            string(REGEX REPLACE "\\.source$" "" RELATIVE_OUTPUT "${RELATIVE_SOURCE}")

            set(OUTPUT "${ASSETS_OUTPUT_DIR}/${RELATIVE_OUTPUT}")

            get_filename_component(OUTPUT_DIRECTORY "${OUTPUT}" DIRECTORY)

            add_custom_command(
                OUTPUT "${OUTPUT}"

                COMMAND
                    ${CMAKE_COMMAND}
                    -E make_directory
                    "${OUTPUT_DIRECTORY}"

                COMMAND
                    $<TARGET_FILE:asteracity-assetc>
                    --in "${SOURCE}"
                    --out "${OUTPUT}"

                DEPENDS
                    asteracity-assetc
                    "${SOURCE}"

                COMMENT "compiling asset ${RELATIVE_SOURCE}"

                VERBATIM
            )
        else()
            set(OUTPUT "${ASSETS_OUTPUT_DIR}/${RELATIVE_SOURCE}")

            get_filename_component(OUTPUT_DIRECTORY "${OUTPUT}" DIRECTORY)

            add_custom_command(
                OUTPUT "${OUTPUT}"

                COMMAND
                    ${CMAKE_COMMAND}
                    -E make_directory
                    "${OUTPUT_DIRECTORY}"

                COMMAND
                    ${CMAKE_COMMAND}
                    -E copy_if_different
                    "${SOURCE}"
                    "${OUTPUT}"

                DEPENDS "${SOURCE}"

                COMMENT "copying asset ${RELATIVE_SOURCE}"

                VERBATIM
            )
        endif()

        list(APPEND OUTPUT_FILES "${OUTPUT}")
    endforeach()

    add_custom_target(
        "${TARGET}_assets"
        DEPENDS ${OUTPUT_FILES}
    )

    add_dependencies("${TARGET}" "${TARGET}_assets")
endfunction()