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
find_package(glm 0.9.9.9 REQUIRED)

add_subdirectory(libs/glad)

#TODO: WIN32 only for release
add_executable(game_of_life main.cpp) #win32 for console window hide

target_link_libraries(${PROJECT_NAME} glfw glad glm::glm)