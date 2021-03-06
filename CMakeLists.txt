cmake_minimum_required(VERSION 2.8)

#project name
project(blackWolf)

include_directories( ${CMAKE_SOURCE_DIR}/include )
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g")

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)

#find_package( SFML 2 REQUIRED COMPONENTS GRAPHICS WINDOW SYSTEM )
#link_directories(/usr/local/lib/)

#directories
#add_subdirectory(common)
#add_subdirectory(model)
#add_subdirectory(views)
#add_subdirectory(controller)
#add_subdirectory(ext)

set(bw_HDRS
    controller/BoardMaster.hpp
    views/components/PieceSprite.hpp
    common/Position.hpp
    Application.hpp
    common/PseudoMove.hpp
    common/CompleteMove.hpp
    Engine.hpp    
    Resources.hpp
    controller/components/Settings.hpp
    views/MoveList.hpp
    views/Canvas.hpp
    views/Clocks.hpp
    model/GameData.hpp
    BlackWolf.hpp
    views/Status.hpp
    controller/components/ButtonBox.hpp
    controller/components/ColorChoices.hpp
    views/components/Firework.hpp
    Client.hpp
    NetWidgets.hpp
)

set(bw_SRCS
    controller/BoardMaster.cpp
    views/components/PieceSprite.cpp
    common/Position.cpp
    Application.cpp
    common/PseudoMove.cpp
    common/CompleteMove.cpp
    Engine.cpp    
    Resources.cpp
    controller/components/Settings.cpp
    views/MoveList.cpp
    views/Canvas.cpp
    views/Clocks.cpp
    model/GameData.cpp
    BlackWolf.cpp
    views/Status.cpp
    controller/components/ButtonBox.cpp
    controller/components/ColorChoices.cpp
    views/components/Firework.cpp
    Client.cpp
    NetWidgets.cpp
	ext/file_descriptor.cpp
)

set(bw_ALL ${bw_HDRS} ${bw_SRCS})

#Executable
add_library(asLibrary ${bw_ALL})
target_link_libraries(asLibrary /usr/local/lib/libboost_system.so)
target_link_libraries(asLibrary /usr/local/lib/libsfgui.so)
target_link_libraries(asLibrary /usr/local/lib/libthor.so)
target_link_libraries(asLibrary /usr/local/lib/libsfml-graphics.so.2)
target_link_libraries(asLibrary /usr/local/lib/libsfml-window.so.2)
target_link_libraries(asLibrary /usr/local/lib/libsfml-system.so.2)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} asLibrary)

#tests
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

set(test_SRCS
    tests/tests.cpp
)
add_executable(tests ${test_SRCS})
target_link_libraries(tests asLibrary)
target_link_libraries(tests /usr/local/lib/libboost_unit_test_framework.so)
target_link_libraries(tests /usr/local/lib/libboost_system.so)
target_link_libraries(tests /usr/local/lib/libboost_timer.so)
