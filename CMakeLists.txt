cmake_minimum_required(VERSION 2.8)
project(PipelineTest)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(PipelineTest ${SOURCES})
target_link_libraries(PipelineTest ${OpenCV_LIBS})

file(COPY testImages DESTINATION ${CMAKE_BINARY_DIR})