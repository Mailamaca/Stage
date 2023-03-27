MESSAGE( STATUS "Configuring libstage" )

# for config.h
include_directories(${PROJECT_BINARY_DIR})

set( stageSrcs
	src/block.cc
	src/blockgroup.cc
	src/camera.cc
	src/color.cc
	src/file_manager.cc
	src/gl.cc
	src/logentry.cc
	src/model.cc
	src/model_actuator.cc
	src/model_blinkenlight.cc
	src/model_blobfinder.cc
	src/model_bumper.cc
	src/model_callbacks.cc
	src/model_camera.cc
	src/model_draw.cc
	src/model_fiducial.cc
	src/model_gripper.cc
	src/model_lightindicator.cc
	src/model_position.cc
	src/model_ranger.cc
	src/option.cc
	src/powerpack.cc
	src/region.cc
	src/stage.cc
	src/texture_manager.cc
	src/typetable.cc
	src/world.cc
	src/worldfile.cc
	src/canvas.cc
	src/options_dlg.cc
	src/vis_strip.cc
	src/worldgui.cc
	src/ancestor.cc
)

#	model_getset.cc
#	model_load.cc

add_library(stage SHARED ${stageSrcs})
set_target_properties(stage PROPERTIES VERSION ${VERSION})
set_target_properties(stage PROPERTIES PUBLIC_HEADER include/stage.hh)
target_include_directories(stage PRIVATE include)

# if fltk-config didn't bring along the OpenGL dependencies (eg. on
# Debian/Ubuntu), add them explicity
IF (NOT(${FLTK_LDFLAGS} MATCHES "-lGL"))
  target_link_libraries( stage ${OPENGL_LIBRARIES})
ENDIF (NOT(${FLTK_LDFLAGS} MATCHES "-lGL"))


target_link_libraries( stage
    ${LTDL_LIB}
    ${JPEG_LIBRARIES}
    ${PNG_LIBRARIES}
    ${FLTK_LIBRARIES}
)

set( stagebinarySrcs src/main.cc )
# set_source_files_properties( ${stagebinarySrcs} PROPERTIES COMPILE_FLAGS "${FLTK_CFLAGS}" )


add_executable( stagebinary ${stagebinarySrcs} )
set_target_properties( stagebinary PROPERTIES OUTPUT_NAME stage )

target_link_libraries( stagebinary stage pthread)

install(TARGETS stage
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)

INSTALL(TARGETS stagebinary
	RUNTIME DESTINATION bin
)

