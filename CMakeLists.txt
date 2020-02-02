cmake_minimum_required(VERSION 3.1.0)

set(PROJECT TerraGen)
project(${PROJECT})

include(ThirdParty.cmake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS gui REQUIRED)
find_package(Qt5 COMPONENTS opengl REQUIRED)


# Whenever this glob's value changes, cmake will rerun and update the build with the
# new/removed files.
file(GLOB_RECURSE shaders CONFIGURE_DEPENDS "Shader/*.glsl")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "src/*.h")

add_executable(${PROJECT}
        ${shaders}
        ${sources}
        ${headers}
        src/window.ui
)

target_include_directories(${PROJECT} PUBLIC
        src/
        ${glm_SOURCE_DIR}
        #${libnoise_SOURCE_DIR}/noiseutils
        ${libnoise_SOURCE_DIR}/src
)


target_link_libraries(${PROJECT}
	Qt5::Widgets
	Qt5::Core
	Qt5::Gui
	OpenGL32
        noise
)
