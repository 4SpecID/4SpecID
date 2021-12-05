cmake_minimum_required(VERSION 3.10)
add_library(network SHARED
    src/network.cpp
)
add_library(4SpecID::Network ALIAS network)
target_link_libraries(network
    PRIVATE
    ${CURL_LIBRARIES}
)
target_include_directories(
    network
    PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/network>
)

target_include_directories(
    network
    PUBLIC 
    ${CURL_INCLUDE_DIRS}
)