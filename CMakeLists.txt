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

add_executable(typewriter_gaem main.cpp engine/TextEffect.cpp engine/TextEffect.hpp engine/System.cpp engine/System.hpp engine/AudioSystem.cpp engine/AudioSystem.hpp engine/Engine.hpp engine/Utilities/General.hpp engine/Components/Transition.hpp engine/Utilities/Tuples.hpp engine/Entity.cpp engine/Entity.hpp engine/Game.cpp engine/Game.hpp engine/EventBus/EventBus.hpp engine/EventBus/Listeners.hpp engine/EventBus/SynchronousListeners.hpp engine/Components/Component.cpp engine/Components/Component.hpp engine/Components/Translations/Translate.hpp engine/Components/Translations/TranslateVertical.hpp engine/Components/Translations/TranslateHorizontal.hpp engine/Identifiable.cpp engine/Identifiable.hpp engine/Components/Mesh.cpp engine/Components/Mesh.hpp engine/Components/Page/PageController.cpp engine/Components/Page/PageController.hpp engine/Components/Collider.cpp engine/Components/Collider.hpp engine/Components/AnimatedMesh.cpp engine/Components/AnimatedMesh.hpp engine/Components/Translations/Rotate.hpp engine/Components/Translations/Scale.hpp engine/Components/Page/Printable.cpp engine/Components/Page/Printable.hpp engine/Utilities/Cached.hpp engine/Components/Page/Paragraph.cpp engine/Components/Page/Paragraph.hpp engine/Components/Page/Dialog.cpp engine/Components/Page/Dialog.hpp engine/Components/Page/DecisionNode.cpp engine/Components/Page/DecisionNode.hpp engine/Components/Interactive.cpp engine/Components/Interactive.hpp)

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
