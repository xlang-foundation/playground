cmake_minimum_required(VERSION 3.0)
project(Example101)

include_directories("../xlang/sdk")

FILE(GLOB XLANG_SOURCES "../xlang/sdk/*.cpp")
# Add the source files
add_executable(${PROJECT_NAME} WIN32 
	main.cpp 
	${XLANG_SOURCES})

# Specify Windows libraries if needed
target_link_libraries(${PROJECT_NAME} -lgdi32)
