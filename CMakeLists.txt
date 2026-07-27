cmake_minimum_required(VERSION	3.20)

project(MunitaEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(Engine)
add_subdirectory(MunitaEditor)
add_subdirectory(Runtime)

target_include_directories(
    MunitaEngine
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty 
)

target_include_directories(
    MunitaEngine
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets
)