cmake_minimum_required(VERSION 3.14)
project(typewriter_gaem)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(typewriter_gaem main.cpp engine/page.cpp engine/page.h engine/font_utils.cpp engine/font_utils.h engine/paragraph.cpp engine/paragraph.h engine/text_effect.cpp engine/text_effect.h)

find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(
            typewriter_gaem
            sfml-system
            sfml-window
            sfml-graphics
            sfml-network
            sfml-audio
    )
endif()
