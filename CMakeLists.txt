cmake_minimum_required( VERSION 3.12 FATAL_ERROR )
PROJECT(stage)

SET( V_MAJOR 4 )
SET( V_MINOR 3 )
SET( V_BUGFIX 0 )

SET( VERSION ${V_MAJOR}.${V_MINOR}.${V_BUGFIX} )
SET( APIVERSION ${V_MAJOR}.${V_MINOR} )

# minimum version of Player to build the plugin
SET( MIN_PLAYER 2.1.0 )

OPTION (BUILD_PLAYER_PLUGIN "Build Player plugin" ON)
OPTION (BUILD_LSPTEST "Build Player plugin tests" OFF)
OPTION (CPACK_CFG "[release building] generate CPack configuration files" ON)

# todo - this doesn't work yet. Run Stage headless with -g.
# OPTION (BUILD_GUI "Build FLTK-based GUI. If OFF, build a gui-less Stage useful e.g. for headless compute clusters." ON )

IF (CMAKE_MAJOR_VERSION EQUAL 2 AND NOT CMAKE_MINOR_VERSION LESS 6)
	cmake_policy( SET CMP0003 NEW )
	cmake_policy( SET CMP0005 OLD )
ENDIF (CMAKE_MAJOR_VERSION EQUAL 2 AND NOT CMAKE_MINOR_VERSION LESS 6)

MESSAGE (STATUS "${PROJECT_NAME} version ${VERSION}")

# Set the default build type
IF (NOT CMAKE_BUILD_TYPE)
  SET (CMAKE_BUILD_TYPE "release" CACHE STRING
    "Choose the type of build, options are: release (default) debug profile" FORCE)
ENDIF (NOT CMAKE_BUILD_TYPE)
STRING(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)

MESSAGE( STATUS "Build type ${CMAKE_BUILD_TYPE}" )


# Create the config.h file
# config.h belongs with the source (and not in CMAKE_CURRENT_BINARY_DIR as in Brian's original version)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
					${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

message( STATUS "Checking for libtool" )
find_path( LTDL_INCLUDE_DIR ltdl.h DOC "Libtool include dir" )
find_library( LTDL_LIB ltdl DOC "Libtool lib" )


MESSAGE( STATUS "Checking for required libraries..." )

find_package(ament_cmake)

find_package( JPEG REQUIRED )

find_package( PNG REQUIRED )

# deal with new missing X11 on OS X 10.8 Mountain Lion
# todo: fix this properly
# SET( PNG_LIBRARIES /opt/X11/lib/libpng.dylib )
# SET( PNG_INCLUDE_DIR /opt/X11/include )

set (FLTK_SKIP_FLUID TRUE)
find_package( FLTK REQUIRED )
find_package( OpenGL REQUIRED )

IF( NOT OPENGL_GLU_FOUND )
  MESSAGE( FATAL_ERROR "OpenGL GLU not found, aborting" )
ENDIF( NOT OPENGL_GLU_FOUND )

SET( INDENT "  * " )
MESSAGE( STATUS ${INDENT} "JPEG_INCLUDE_DIR = ${JPEG_INCLUDE_DIR}" )
MESSAGE( STATUS ${INDENT} "JPEG_LIBRARIES = ${JPEG_LIBRARIES}" )
MESSAGE( STATUS ${INDENT} "PNG_INCLUDE_DIR = ${PNG_INCLUDE_DIR}" )
MESSAGE( STATUS ${INDENT} "PNG_LIBRARIES = ${PNG_LIBRARIES}" )
MESSAGE( STATUS ${INDENT} "FLTK_LIBRARIES=${FLTK_LIBRARIES}" )
MESSAGE( STATUS ${INDENT} "FLTK_INCLUDE_DIR=${FLTK_INCLUDE_DIR}" )
MESSAGE( STATUS ${INDENT} "OpenGL GLU found at ${OPENGL_INCLUDE_DIR}" )
MESSAGE( STATUS "    OPENGL_INCLUDE_DIR = ${OPENGL_INCLUDE_DIR}")
MESSAGE( STATUS "    OPENGL_glu_LIBRARY = ${OPENGL_glu_LIBRARY}")


MESSAGE( STATUS "Installation path CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}" )

# all targets need these include directories
include_directories(
    .
    libstage/include
    replace
    ${FLTK_INCLUDE_DIR}
    ${PNG_INCLUDE_DIR}
    ${JPEG_INCLUDE_DIR}
    ${CMAKE_INCLUDE_PATH}
    ${OPENGL_INCLUDE_DIR}
    ${LTDL_INCLUDE_DIR}
)


# work through these subdirs
ADD_SUBDIRECTORY(libstage)
ADD_SUBDIRECTORY(assets)

IF(${ament_cmake_FOUND})
  # If ament is found compile stage as ros package.
  ament_export_libraries(stage)
  ament_export_include_directories(include)
  ament_package()
ENDIF (${ament_cmake_FOUND})
