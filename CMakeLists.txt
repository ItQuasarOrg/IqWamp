cmake_minimum_required (VERSION 3.1.0 FATAL_ERROR)

set (PROJECT IqWamp)
set (VERSION "0.1.0")

if (UNIX)
    string (TOLOWER ${PROJECT} TARGET)
else ()
    set (TARGET ${PROJECT})
endif ()

#option (IqWamp_DEBUG_MODE_ENABLED "Enable IqWamp debug mode" OFF)

set (IqWamp_FOUND ON CACHE BOOL "True if headers and requested libraries were found")
set (IqWamp_VERSION ${VERSION} CACHE STRING "IqWamp version")
set (IqWamp_LIBRARIES ${TARGET} CACHE STRING "IqWamp component libraries to be linked")

project (${PROJECT})

#Qt definitions
set (CMAKE_AUTOMOC ON)

find_package (Qt5WebSockets REQUIRED)

#Find sources
file (GLOB_RECURSE SOURCES src/*.cpp)
source_group ("Source Files" FILES ${SOURCES})
file (GLOB_RECURSE HEADERS src/*.h)
source_group ("Header Files" FILES ${HEADERS})

#Find include dirs
set (IqWamp_INCLUDE_DIRS "")
foreach (HEADER_FILE ${HEADERS})
    get_filename_component(DIR ${HEADER_FILE} PATH)
    list (APPEND IqWamp_INCLUDE_DIRS ${DIR})
endforeach()
list(REMOVE_DUPLICATES IqWamp_INCLUDE_DIRS)

include_directories(${IqWamp_INCLUDE_DIRS})
set (IqWamp_INCLUDE_DIRS ${IqWamp_INCLUDE_DIRS} CACHE STRING "IqWamp include directories")

#Set difinitions
add_definitions (-DIQWAMP_LIBRARY)
add_definitions (-DLIB_VERSION="${VERSION}")

#Set Windows res files compiler
set (VERSIONINFO_RES_FILE "")
if(MINGW)
    file (GLOB VERSIONINFO_RES_FILE . src/versioninfo.rc)
    SET (CMAKE_RC_COMPILE_OBJECT
            "<CMAKE_RC_COMPILER> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif(MINGW)

add_library (${TARGET} SHARED ${SOURCES} ${VERSIONINFO_RES_FILE})

target_link_libraries (${TARGET} Qt5::WebSockets)

set_target_properties (${TARGET} PROPERTIES
                       VERSION ${VERSION})

#Set C++11
if (MINGW)
    add_definitions(-std=c++11)
else ()
    target_compile_features (${TARGET} PRIVATE cxx_range_for)
endif ()

#Set all warnings enabled
if (MSVC)
    add_definitions (/W4)
elseif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -pedantic)
else ()
    message ("Unknown compiler")
endif ()

#Enable debug mode
if (IqWamp_DEBUG_MODE)
    add_definitions (-DIQWAMP_DEBUG_MODE)
endif ()
