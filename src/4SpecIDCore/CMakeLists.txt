cmake_minimum_required(VERSION 3.10)
project(iSpecID LANGUAGES CXX VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(VERBOSE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "-DCMAKE_CXX_STANDARD=17 -Wall -Wextra -Wstrict-aliasing -pedantic -Wunreachable-code -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-include-dirs -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo  -Wstrict-overflow=5 -Wswitch-default -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option ")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -Wabi -g -fno-omit-frame-pointer -fno-inline-functions -fno-optimize-sibling-calls")

find_package(Boost REQUIRED COMPONENTS thread regex REQUIRED)
find_package(CURL REQUIRED)

add_subdirectory(representations)
add_subdirectory(algorithms)
add_subdirectory(network)
add_subdirectory(utilities)
add_executable( 
    4specid 
    main.cpp
)

target_include_directories(
    4specid
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(4specid 
    4SpecID::Utilities
    4SpecID::Representations
    4SpecID::Network
    4SpecID::Auditor
)