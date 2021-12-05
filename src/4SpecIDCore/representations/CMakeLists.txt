cmake_minimum_required(VERSION 3.10)

add_library(representations INTERFACE)
add_library(4SpecID::Representations ALIAS representations)

target_include_directories(
    representations 
    INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/representations>
    ${Boost_INCLUDE_DIRS}
)
