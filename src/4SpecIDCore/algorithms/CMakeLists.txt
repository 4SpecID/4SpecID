cmake_minimum_required(VERSION 3.10)

add_library(algorithms INTERFACE)
add_library(4SpecID::Algorithms ALIAS algorithms)

target_include_directories(
    algorithms
    INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/representations>
)

add_subdirectory(auditor)