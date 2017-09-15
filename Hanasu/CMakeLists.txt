CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(Gawariet)

SET(CMAKE_CXX_FLAGS_PROFILING "-O0 -g -pg")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -g")
SET(CMAKE_CXX_FLAGS_CCOV "-O2 -g --coverage")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread")

SET(CMAKE_BUILD_TYPE debug)

# Set compiler warning flags
# ADD_DEFINITIONS("-Werror") # Make all warnings into errors.
ADD_DEFINITIONS("-Wall") # Generate all warnings
ADD_DEFINITIONS("-Wextra") # Generate even more extra warnings

SET(SERVER "hanasu-server")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

INCLUDE_DIRECTORIES(/home/oskar/Programowanie/Hanasu/HanasuCommon/include/)
ADD_SUBDIRECTORY(server)
