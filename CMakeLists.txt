#######################################################
## Nessy CMake build script

PROJECT("DigitalKeystone")

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMakeModules/")

# Overide with cmake -DCMAKE_BUILD_TYPE=Debug {dir}
IF( NOT CMAKE_BUILD_TYPE )
   MESSAGE("Build type not set (defaults to release)")
   MESSAGE("-DCMAKE_BUILD_TYPE=Debug for debug")
   SET(CMAKE_BUILD_TYPE Release)
ENDIF()

# Platform configuration vars
INCLUDE(SetPlatformVars)

#######################################################
## Sources

SET(
  SOURCES
  main.cpp
  keystone.cg
)

#######################################################
## Find required dependencies

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(FREEGLUT REQUIRED)
FIND_PACKAGE(Pangolin REQUIRED)
FIND_PACKAGE(Cg REQUIRED)
FIND_PACKAGE(Boost 1.40 COMPONENTS thread REQUIRED)

LINK_DIRECTORIES(
  ${Boost_LIBRARY_DIRS}
)

INCLUDE_DIRECTORIES(
  ${OPENGL_INCLUDE_DIR}
  ${FREEGLUT_INCLUDE_DIR}
  ${Pangolin_INCLUDE_DIR}
  ${CG_INCLUDE_PATH}
  ${Boost_INCLUDE_DIRS}
)

LINK_LIBRARIES(
  ${OPENGL_LIBRARIES}
  ${FREEGLUT_LIBRARY}
  ${Pangolin_LIBRARY}
  ${CG_LIBRARY}
  ${CG_GL_LIBRARY}
  ${Boost_LIBRARIES}
)

ADD_EXECUTABLE( digitalkeystone ${SOURCES})
