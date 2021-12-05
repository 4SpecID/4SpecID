cmake_minimum_required(VERSION 3.10)

add_library(auditor SHARED
    src/auditor.cpp
)
add_library(4SpecID::Auditor ALIAS auditor)
target_link_libraries(auditor
    PUBLIC
    4SpecID::Network
    4SpecID::Representations
    4SpecID::Algorithms
    4SpecID::Utilities
)
target_include_directories(
    auditor
    PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/auditor>
    ${Boost_INCLUDE_DIRS}
)