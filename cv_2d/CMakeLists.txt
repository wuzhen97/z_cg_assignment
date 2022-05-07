cmake_minimum_required(VERSION 3.0)
project(CV_2D)

IF(NOT CMAKE_BUILD_TYPE)         
    set(CMAKE_BUILD_TYPE Release)
ENDIF()
message("Build type: " ${CMAKE_BUILD_TYPE})  
#set(BUILD_USE_64BITS on)

#set(OpenCV_DIR "E:/opencv-4.5.4/build")
find_package(OpenCV REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
)

# build libs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
add_library(${PROJECT_NAME} SHARED
    src/noice.cpp
    src/filter.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS}
)
    
# build examples
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
add_executable(main src/main.cpp)
target_link_libraries(main ${PROJECT_NAME})