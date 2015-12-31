cmake_minimum_required (VERSION 2.8.0 FATAL_ERROR)
project (Lyst CXX)

# Version numbers
set (Lyst_VERSION_MAJOR 0)
set (Lyst_VERSION_MINOR 1)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3")

# Adding extra libraries for displays and things
set (EXTRA_LIBS ${EXTRA_LIBS} 
    pthread
)

add_executable (Lyst
    #        src/lystSolver.cpp
    src/lyst.cpp
    src/main.cpp
)
target_link_libraries (Lyst ${EXTRA_LIBS})
