cmake_minimum_required(VERSION 3.5)
project(typewriter_gaem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-long-long")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif ()

add_executable(typewriter_gaem main.cpp engine/Components/Page/TextEffect.cpp engine/Components/Page/TextEffect.hpp engine/System.cpp engine/System.hpp engine/AudioSystem.cpp engine/AudioSystem.hpp engine/Engine.hpp engine/Utilities/General.hpp engine/Components/Transition.hpp engine/Utilities/Tuples.hpp engine/Entity.cpp engine/Entity.hpp engine/Game.cpp engine/Game.hpp engine/EventBus/EventBus.hpp engine/EventBus/Listeners.hpp engine/EventBus/QueuedListeners.hpp engine/Components/Component.cpp engine/Components/Component.hpp engine/Components/Translations/Translate.hpp engine/Components/Translations/TranslateVertical.hpp engine/Components/Translations/TranslateHorizontal.hpp engine/Identifiable.cpp engine/Identifiable.hpp engine/Components/Mesh.cpp engine/Components/Mesh.hpp engine/Components/Page/PageController.cpp engine/Components/Page/PageController.hpp engine/Components/Collider.cpp engine/Components/Collider.hpp engine/Components/AnimatedMesh.cpp engine/Components/AnimatedMesh.hpp engine/Components/Translations/Rotate.hpp engine/Components/Translations/Scale.hpp engine/Components/Page/Printable.cpp engine/Components/Page/Printable.hpp engine/Utilities/Cached.hpp engine/Components/Page/Paragraph.cpp engine/Components/Page/Paragraph.hpp engine/Components/Page/Dialog.cpp engine/Components/Page/Dialog.hpp engine/Components/Page/DecisionNode.cpp engine/Components/Page/DecisionNode.hpp engine/Utilities/General.cpp engine/Utilities/NonCopyable.h engine/Utilities/NonMovable.h engine/EventBus/SynchronousListeners.hpp engine/Scene.cpp engine/Scene.hpp engine/Components/KeyboardInput.cpp engine/Components/KeyboardInput.hpp engine/Components/MouseInput.cpp engine/Components/MouseInput.hpp engine/Constants.hpp engine/Utilities/Logger.cpp engine/Utilities/Logger.hpp engine/Components/Visual/Parallax.cpp engine/Components/Visual/Parallax.hpp engine/Components/PlayerController.cpp engine/Components/PlayerController.hpp)

# Debug flags
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(typewriter_gaem PUBLIC DEBUG_BOUNDS=1)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(typewriter_gaem PUBLIC DEBUG_BOUNDS=0)
endif()

target_compile_definitions(typewriter_gaem PRIVATE BOOST_ALLOW_DEPRECATED_HEADERS)

# Tweeny for tweening out of the box
add_subdirectory(vendor/tweeny)
target_link_libraries(typewriter_gaem tweeny)

# SFML
find_package(SFML REQUIRED COMPONENTS system window graphics network audio)
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
endif ()

# Boost
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS log log_setup thread filesystem system)
if (BOOST_FOUND)
    message(STATUS "---------------------")
    message(STATUS "Boost_FOUND: ${Boost_FOUND}")
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "---------------------")

    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(typewriter_gaem ${Boost_LIBRARIES})

    # multithreaded boost linking on MacOS fuckery fix
    if(APPLE)
        target_link_libraries(typewriter_gaem ${Boost_LIBRARY_DIRS}/libboost_log-mt.dylib)
    endif()
endif ()
