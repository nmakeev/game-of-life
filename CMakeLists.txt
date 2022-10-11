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

IF(CMAKE_BUILD_TYPE MATCHES Release)
    set(PROJECT_WIN32 "WIN32")
ENDIF()

add_subdirectory(libs/glad)

SET(SRC_LIST src/main.cpp
        src/graphics/shader.cpp
        src/graphics/shader.h
        src/graphics/resource_manager.cpp
        src/graphics/resource_manager.h
        src/graphics/renderer.cpp
        src/graphics/renderer.h src/application.cpp src/application.h src/graphics/camera.cpp src/graphics/camera.h src/graphics/screen_settings.cpp src/graphics/screen_settings.h)

add_executable(${PROJECT_NAME} ${PROJECT_WIN32} ${SRC_LIST}) #win32 for console window hide

target_link_libraries(${PROJECT_NAME} glfw glad glm::glm)

file(COPY res DESTINATION ${CMAKE_BINARY_DIR})

#cpack

include(InstallRequiredSystemLibraries)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/res/
        DESTINATION res
        COMPONENT ResourcesCursors)

include(CPack)