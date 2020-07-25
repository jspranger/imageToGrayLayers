cmake_minimum_required(VERSION 3.6)

set(PROJECT_NAME "imageToGrayLayers")

project(${PROJECT_NAME} VERSION "1.0.0")

find_package(OpenCV REQUIRED)

set(
    HDR_FILES ${PROJECT_SOURCE_DIR}/src/filters.h
              ${PROJECT_SOURCE_DIR}/src/io.h
              ${PROJECT_SOURCE_DIR}/src/utils.h
)
set(
    SRC_FILES ${PROJECT_SOURCE_DIR}/src/filters.cpp
              ${PROJECT_SOURCE_DIR}/src/main.cpp
              ${PROJECT_SOURCE_DIR}/src/io.cpp
              ${PROJECT_SOURCE_DIR}/src/utils.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HDR_FILES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
                    CXX_STANDARD 17
                    CXX_EXTENSIONS OFF
)
set_property(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}
)
