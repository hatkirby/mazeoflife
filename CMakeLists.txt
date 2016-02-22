cmake_minimum_required (VERSION 2.6)
project (mazeoflife)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_net REQUIRED)

include_directories(
  ${SDL2_INCLUDE_DIR}
  ${SDL2_TTF_INCLUDE_DIR}
  ${SDL2_NET_INCLUDE_DIRS}
)

add_executable(mazeoflife highscore.cpp hslist.cpp mazeoflife.cpp util.cpp titlestate.cpp gamestate.cpp)
set_property(TARGET mazeoflife PROPERTY CXX_STANDARD 11)
set_property(TARGET mazeoflife PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(mazeoflife ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_NET_LIBRARIES})
