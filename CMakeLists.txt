cmake_minimum_required(VERSION 3.6)
project(simplex)

#set(CMAKE_CONFIGURATION_TYPES "cmake-build-debug;release")
set(CMAKE_CXX_STANDARD 11)

set( Boost_USE_MULTITHREADED ON )
set( Boost_USE_STATIC_LIBS   ON )
set( Boost_DETAILED_FAILURE_MSG   ON )
#set( Boost_COMPILER "-vc140" )

set(Boost_DEBUG ON)

FIND_PACKAGE( Boost COMPONENTS program_options iostreams filesystem system REQUIRED )
include_directories(${Boost_INCLUDE_DIRS})

link_libraries(ws2_32 wsock32 ${Boost_LIBRARIES})

#message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
#message(STATUS "Boost_IOSTREAMS: ${Boost_IOSTREAMS}")
#message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
#message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")

set(SOURCE_FILES
        main.cpp
        streamer.cpp
        streamer.h
        udp_server.cpp
        udp_server.h)

add_executable(simplex ${SOURCE_FILES})