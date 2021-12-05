cmake_minimum_required(VERSION 3.10)

add_library(utilities 
INTERFACE
)
add_library(4SpecID::Utilities ALIAS utilities)
set_target_properties(utilities PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(
    utilities
    INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/utilities>
)

