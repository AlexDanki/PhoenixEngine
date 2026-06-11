cmake_minimum_required(VERSION	3.20)

project(PhoenixEngine)

add_subdirectory(Engine)
add_subdirectory(Sandbox)

target_include_directories(
    PhoenixEngine
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty 
)

target_include_directories(
    PhoenixEngine
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets
)