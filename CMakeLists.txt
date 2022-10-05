cmake_minimum_required(VERSION 3.23)
project(game_of_life)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++") #link std lib

#--------------------------------------------------------------------
# Hide the console window in visual studio projects
#--------------------------------------------------------------------
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

find_package(glfw3 3.3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIR})

add_subdirectory(libs/glad)

add_executable(game_of_life WIN32 main.cpp) #win32 for console window hide

target_link_libraries(${PROJECT_NAME} glfw glad)