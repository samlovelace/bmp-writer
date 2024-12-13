cmake_minimum_required(VERSION 3.8)

project(bmp-writer)

set(DIR ${CMAKE_CURRENT_LIST_DIR})

set(SOURCES 
    ${DIR}/src/main.cpp
    ${DIR}/src/BmpImage.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

include_directories(${PROJECT_NAME}
    ${DIR}/include    
)
