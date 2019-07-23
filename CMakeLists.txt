cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
project(typewriter_gaem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(typewriter_gaem main.cpp engine/page.cpp engine/page.h engine/paragraph.cpp engine/paragraph.h engine/text_effect.cpp engine/text_effect.h engine/resources.cpp engine/resources.h engine/utils.cpp engine/utils.h engine/game_state.cpp engine/game_state.h engine/sounds.cpp engine/sounds.h engine/engine.h)

find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    if (APPLE)
        target_link_libraries(
                typewriter_gaem
                "-framework sfml-window"
                "-framework sfml-window"
                "-framework sfml-graphics"
                "-framework sfml-network"
                "-framework sfml-audio"
        )
        set_target_properties(typewriter_gaem PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
    endif ()
    if (UNIX AND NOT APPLE)
        target_link_libraries(
                typewriter_gaem
                sfml-system
                sfml-window
                sfml-graphics
                sfml-network
                sfml-audio
        )
    endif ()
endif ()
