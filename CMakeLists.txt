cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
project(typewriter_gaem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(typewriter_gaem main.cpp engine/page.cpp engine/page.h engine/printables/paragraph.cpp engine/printables/paragraph.h engine/text_effect.cpp engine/text_effect.h engine/resources.cpp engine/resources.h engine/utilities/converters.cpp engine/utilities/converters.h engine/game_state.cpp engine/game_state.h engine/audio_system.cpp engine/audio_system.h engine/engine.h engine/printables/printable.cpp engine/printables/printable.h engine/utilities/text_buffer.cpp engine/utilities/text_buffer.h engine/printables/dialog.cpp engine/printables/dialog.h engine/story/story.cpp engine/story/story.h engine/story/action.cpp engine/story/action.h engine/utilities/general.cpp engine/utilities/general.h engine/story/decision_node.cpp engine/story/decision_node.h engine/printables/printable_store.cpp engine/printables/printable_store.h)

target_compile_definitions (typewriter_gaem PRIVATE BOOST_ALLOW_DEPRECATED_HEADERS)

find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)
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
    include_directories(${BOOST_INCLUDE_DIR})
    target_link_libraries(
            typewriter_gaem
            ${Boost_FILESYSTEM_LIBRARY}
            ${Boost_SYSTEM_LIBRARY}
    )
endif ()
