cmake_minimum_required(VERSION 3.28)
project(GXEngine)

set(CMAKE_CXX_STANDARD 17)

file(GLOB copyResources "./assets" "./thirdParty/libs/assimp/assimp-vc143-mtd.lib")
file(COPY ${copyResources} DESTINATION ${CMAKE_BINARY_DIR})

include_directories(
        SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include)

link_directories(
        SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/libs/glfw
        SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/libs/assimp
)

if(MSVC)
    add_compile_options("/source-charset:utf-8")
endif()

add_subdirectory(gx)

add_executable(GXEngine main.cpp)
#add_executable(triangle examples/triangle.cpp)

target_link_libraries(GXEngine gx_lib glfw3.lib assimp-vc143-mtd.lib)
#target_link_libraries(triangle gx_lib glfw3.lib assimp-vc143-mtd.lib)