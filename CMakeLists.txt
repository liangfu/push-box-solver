CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(pushbox2)

IF(UNIX)
  SET(CMAKE_CXX_FLAGS "-pipe -g -Wall -W -O0")
ENDIF(UNIX)

INCLUDE_DIRECTORIES(.)
ADD_DEFINITIONS(`fltk-config --cxxflags`)
ADD_EXECUTABLE(pushbox2 
  main.cpp 
  pmwidget.cpp pmwidget.hpp
  pmsolver.cpp pmsolver.hpp 
  utilities.hpp 
  stopwatch.h 
  config.hpp)

IF(UNIX)
  TARGET_LINK_LIBRARIES(pushbox2 fltk Xext Xft fontconfig Xinerama pthread dl m X11)
ENDIF(UNIX)

