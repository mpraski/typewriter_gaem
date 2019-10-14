cmake_minimum_required(VERSION 3.5)
project(typewriter_gaem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    # Force to always compile with W4
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif ()
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif ()

add_executable(typewriter_gaem main.cpp engine/page.cpp engine/page.hpp engine/printables/paragraph.cpp engine/printables/paragraph.hpp engine/TextEffect.cpp engine/TextEffect.hpp engine/System.cpp engine/System.hpp engine/AudioSystem.cpp engine/AudioSystem.hpp engine/engine.hpp engine/printables/printable.cpp engine/printables/printable.hpp engine/printables/dialog.cpp engine/printables/dialog.hpp engine/story/story.cpp engine/story/story.hpp engine/story/action.cpp engine/story/action.hpp engine/utilities/General.hpp engine/story/decision_node.cpp engine/story/decision_node.hpp engine/printables/printable_store.cpp engine/printables/printable_store.hpp engine/components/Transition.hpp engine/utilities/Tuples.hpp engine/Entity.cpp engine/Entity.hpp engine/Game.cpp engine/Game.hpp engine/event_bus/EventBus.hpp engine/event_bus/Listeners.hpp engine/event_bus/SynchronousListeners.hpp engine/components/Component.cpp engine/components/Component.hpp engine/components/translations/Translate.hpp engine/components/translations/TranslateVertical.hpp engine/components/translations/TranslateHorizontal.hpp engine/Identifiable.cpp engine/Identifiable.hpp engine/components/Mesh.cpp engine/components/Mesh.hpp engine/components/page/PageController.cpp engine/components/page/PageController.hpp engine/components/Collider.cpp engine/components/Collider.hpp engine/components/AnimatedMesh.cpp engine/components/AnimatedMesh.hpp engine/components/translations/Rotate.hpp engine/components/translations/Scale.hpp engine/components/page/Printable.cpp engine/components/page/Printable.hpp engine/Action.hpp engine/utilities/Cached.hpp engine/components/page/Paragraph.cpp engine/components/page/Paragraph.hpp engine/components/page/Dialog.cpp engine/components/page/Dialog.hpp engine/Action.cpp engine/components/page/DecisionNode.cpp engine/components/page/DecisionNode.hpp engine/components/Interactive.cpp engine/components/Interactive.hpp)

target_compile_definitions(typewriter_gaem PRIVATE BOOST_ALLOW_DEPRECATED_HEADERS)

# Tweeny for tweening out of the box
add_subdirectory(vendor/tweeny)
target_link_libraries(typewriter_gaem tweeny)

set(Boost_USE_STATIC_LIBS ON)
find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)
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
    )
endif ()
