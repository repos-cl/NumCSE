include(CMake/Macros/list_replace.cmake)
include(CMake/Macros/get_target_name_numcse.cmake)

# note: install rpath is disabled for now since this is only required for
#  relocatable binaries

macro (add_executable_numcse _name)
  # cmake targets need to be unique
  # as such we prepend a prefix from the source path
  # all of this is done by get_executable_name_numcse
  get_target_name_numcse(${_name} target_name)
  #  reassemble argv for add_executable macro with new target name
  set(argv_parsed ${ARGV})
  LIST_REPLACE(argv_parsed 0 "${target_name}")

	# create directory that will contain the binary
	set(TARGET_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

  # retrieve relative path of runtime output directory (e.g the directory to which the target is compiled to)
	string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" TARGET_RUNTIME_OUTPUT_DIRECTORY ${TARGET_RUNTIME_OUTPUT_DIRECTORY})

  # get relative path to binary dir (used as an rpath)
  #string(REPLACE "${CMAKE_BINARY_DIR}" "" RELATIVE_PATH_TO_BINARY_DIR "${TARGET_RUNTIME_OUTPUT_DIRECTORY}")
  #string(REGEX REPLACE "/[^/]+" "../" RELATIVE_PATH_TO_BINARY_DIR "${RELATIVE_PATH_TO_BINARY_DIR}")

  # invoke built-in add_executable
  add_executable(${argv_parsed})

  # linke with libraries
  if (TARGET ${target_name})
    # link with mathgl and figure class
  	set_target_properties(${target_name} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${TARGET_RUNTIME_OUTPUT_DIRECTORY}"
      OUTPUT_NAME "${_name}"
      #INSTALL_RPATH "@executable_path/${RELATIVE_PATH_TO_BINARY_DIR}/mathgl_install/lib"
    )

    #target_link_libraries(${target_name} Figure ${MATHGL2_LIBRARIES})
  	#add_dependencies(${target_name} Eigen)
  	#add_dependencies(${target_name} MathGL)

    # link with python3
    target_link_libraries(${target_name} ${Python3_LIBRARIES})
  endif()
endmacro()
