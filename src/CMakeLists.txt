cmake_minimum_required(VERSION 3.10)
project(iSpecID LANGUAGES CXX VERSION 1.0)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -Wall -Wextra  -Wstrict-aliasing -pedantic -Werror -Wunreachable-code -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo  -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -Wabi -g -fno-omit-frame-pointer -fno-inline-functions -fno-optimize-sibling-calls")
set(VERBOSE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(CURL_DIR)
	list(APPEND CMAKE_PREFIX_PATH ${CURL_DIR})
endif()

if(BOOST_DIR)
	list(APPEND CMAKE_PREFIX_PATH ${BOOST_DIR})
endif()

list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/curl/")

option(BIN "Create BIN binary" ON)
option(MPI "Create MPI binary" ON)
if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0)
  set(CMAKE_CXX17_STANDARD_COMPILE_OPTION "-std=c++17")
  set(CMAKE_CXX17_EXTENSION_COMPILE_OPTION "-std=gnu++17")
elseif (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.1)
  set(CMAKE_CXX17_STANDARD_COMPILE_OPTION "-std=c++1z")
  set(CMAKE_CXX17_EXTENSION_COMPILE_OPTION "-std=gnu++1z")
endif()

set(CURL_PATH "/usr/local/opt/curl/")

find_library(
        CURL_LIBRARIES
        NAMES
            curl
        PATHS 
            ${CURL_PATH}/lib)

find_package(Boost REQUIRED COMPONENTS thread regex REQUIRED)

if(Boost_FOUND)
    message(STATUS "Found Boost version: ${Boost_VERSION}")
    message(STATUS "Using Boost include dir(s): ${Boost_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Could not find Boost")
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/iSpecIDCore/include
    ${PROJECT_SOURCE_DIR}/iSpecIDCore/src
    ${Boost_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
)
file(GLOB core
    "${PROJECT_SOURCE_DIR}/iSpecIDCore/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/iSpecIDCore/include/*.h"
    )

if(BIN)
    if(COMMAND cmake_policy)
        cmake_policy(SET CMP0003 NEW)
    endif(COMMAND cmake_policy)
    add_executable( isid "${PROJECT_SOURCE_DIR}/main.cpp" ${core}  )
    target_link_libraries(isid -lpthread)
    target_link_libraries(isid ${CURL_LIBRARIES})
    target_link_libraries(isid ${Boost_LIBRARIES})
endif()