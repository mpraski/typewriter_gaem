cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
project(typewriter_gaem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(typewriter_gaem main.cpp engine/page.cpp engine/page.hpp engine/printables/paragraph.cpp engine/printables/paragraph.hpp engine/text_effect.cpp engine/text_effect.hpp engine/system.cpp engine/system.hpp engine/utilities/converters.cpp engine/utilities/converters.hpp engine/audio_system.cpp engine/audio_system.hpp engine/engine.hpp engine/printables/printable.cpp engine/printables/printable.hpp engine/utilities/text_buffer.cpp engine/utilities/text_buffer.hpp engine/printables/dialog.cpp engine/printables/dialog.hpp engine/story/story.cpp engine/story/story.hpp engine/story/action.cpp engine/story/action.hpp engine/utilities/general.cpp engine/utilities/general.hpp engine/story/decision_node.cpp engine/story/decision_node.hpp engine/printables/printable_store.cpp engine/printables/printable_store.hpp engine/animation/animation.hpp engine/animation/translate.hpp engine/utilities/tuples.hpp engine/page_container.cpp engine/page_container.hpp engine/game_object.cpp engine/game_object.hpp engine/scene_node.cpp engine/scene_node.hpp engine/game.cpp engine/game.hpp engine/event_bus/event_bus.hpp engine/event_bus/callback_container.hpp engine/event_bus/synchronous_callback_container.hpp)

target_compile_definitions(typewriter_gaem PRIVATE BOOST_ALLOW_DEPRECATED_HEADERS)

# Tweeny for tweening out of the box
add_subdirectory(vendor/tweeny)
target_link_libraries(typewriter_gaem tweeny)

set(Boost_USE_STATIC_LIBS ON)
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
