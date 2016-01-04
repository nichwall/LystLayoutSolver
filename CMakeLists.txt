cmake_minimum_required (VERSION 2.8.0 FATAL_ERROR)
project (Lyst CXX)

# Version numbers
set (Lyst_VERSION_MAJOR 0)
set (Lyst_VERSION_MINOR 1)

configure_file (
    "${PROJECT_SOURCE_DIR}/src/lystConfig.h.in"
    "${PROJECT_BINARY_DIR}/src/lystConfig.h"
)
# Compiling options
option (USE_STRING_BLOCK "Set ON for std::string vectors, OFF for uint16_t vectors" ON)
option (STORE_COMP "Set ON to store vectors periodically. USE_STRING_BLOCK must be ON" OFF)

# If we can avoid using C++14, do it.
if (STORE_COMP)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3")
else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
endif ()

# Adding extra libraries for displays and things
set (EXTRA_LIBS ${EXTRA_LIBS} 
    pthread
)

add_executable (Lyst
    src/lyst.cpp
    src/main.cpp
)
target_link_libraries (Lyst ${EXTRA_LIBS})
