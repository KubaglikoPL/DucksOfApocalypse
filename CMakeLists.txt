cmake_minimum_required(VERSION 3.12)
project(GAME CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
include_directories(include)
include_directories(3rd_party)

if(WIN32)
	link_directories(win32_deps)
	include_directories(win32_deps)
endif(WIN32)

file(GLOB src
        "src/*.cpp"
		"src/engine/*.cpp"
		"src/engine/pc/*.cpp"
		"src/CXXoverC/*.cpp"
		"include/*.h"
		"3rd_party/*cpp"
		"3rd_party/*.h")
		
add_executable(GAME ${src})
target_link_libraries(GAME glfw3)
if(WIN32)
	target_link_libraries(GAME opengl32)
else()
	target_link_libraries(GAME GL)
endif(WIN32)
