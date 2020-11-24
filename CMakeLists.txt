cmake_minimum_required(VERSION 3.5)

project(tut2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# finds OpenGL, GLU and X11
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)
set(GL_LIBRARY GL GLU X11)

# PCL
find_package(PCL 1.3 REQUIRED COMPONENTS common io features visualization)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

#IMGUI
find_package(imgui CONFIG)


add_executable(tut2 main.cpp)
target_link_libraries(tut2 imgui ${PCL_LIBRARIES} glfw ${GL_LIBRARY} m)
