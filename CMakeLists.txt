cmake_minimum_required(VERSION 3.10)
project(tictactoe)

set(CMAKE_CXX_STANDARD 14)

# SFML include/library directories
set(SFML_INCLUDE_DIRECTORY "/usr/include" CACHE PATH "SFML include directory")
set(SFML_LIB_DIRECTORY "/usr/lib" CACHE PATH "SFML libraries directory")

# SFML libraries configuration
if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(SFML_LIBS sfml-system-d sfml-window-d sfml-graphics-d)
else()
  if (WIN32)
    set(SFML_LIBS sfml-main sfml-system sfml-window sfml-graphics)
  else()
    set(SFML_LIBS sfml-system sfml-window sfml-graphics)
  endif()
endif()

# Find sources, add executable, link libraries
include_directories(${SFML_INCLUDE_DIRECTORY} ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src)
file(GLOB SOURCES src/*.cpp src/Gui/*.cpp src/Utils/*.cpp src/Game/*.cpp)
link_directories(${SFML_LIB_DIRECTORY})
if(WIN32 AND CMAKE_BUILD_TYPE MATCHES Release)
  add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
else()
  add_executable(${PROJECT_NAME} ${SOURCES})
endif()

target_link_libraries(${PROJECT_NAME} ${SFML_LIBS})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fonts/calibri.ttf
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/fonts/)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
