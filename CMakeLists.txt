cmake_minimum_required (VERSION 2.6.0)  # upgrade `cmake` to find newer version of boost

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")  # it is fine to build w/ clang++

# Head and source
include_directories(include)
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

# python3
set(PYTHON_LIBRARY "/Library/Frameworks/Python.framework/Versions/3.6/lib/libpython3.6m.dylib")
set(PYTHON_INCLUDE_DIR "${PYTHON_INCLUDE_DIR} /Library/Frameworks/Python.framework/Versions/3.6/include/python3.6m")

# opencv
set(OpenCV_DIR "/usr/local/opt/opencv@2/share/OpenCV")  # installed w/ homebrew

# Find necessary packages
find_package(PythonLibs 3.6 REQUIRED)  # adapted for py3
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(Boost COMPONENTS python37 REQUIRED)  # `COMPONENTS` w/ version
include_directories(${Boost_INCLUDE_DIR})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

# Define the wrapper library
add_library(Augment SHARED ${DIR_SRCS})
target_link_libraries(Augment ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${OpenCV_LIBRARIES})

# Don't prepend wrapper library name with lib
set_target_properties(Augment PROPERTIES PREFIX "")
