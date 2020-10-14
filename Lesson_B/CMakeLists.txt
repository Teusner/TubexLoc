# ==================================================================
#  tubex-lib / Lesson-B - cmake configuration file
# ==================================================================

  cmake_minimum_required(VERSION 3.0.2)
  project(Lesson-A LANGUAGES CXX)

# Adding IBEX

  # In case you installed IBEX in a local directory, you need 
  # to specify its path with the CMAKE_PREFIX_PATH option.
  # set(CMAKE_PREFIX_PATH "~/ibex-lib/build_install")

  find_package(IBEX REQUIRED)
  ibex_init_common() # IBEX should have installed this function
  message(STATUS "Found IBEX version ${IBEX_VERSION}")

# Adding Tubex

  # In case you installed Tubex in a local directory, you need 
  # to specify its path with the CMAKE_PREFIX_PATH option.
  # set(CMAKE_PREFIX_PATH "~/tubex-lib/build_install")

  find_package(TUBEX REQUIRED)
  message(STATUS "Found Tubex version ${TUBEX_VERSION}")

# Compilation

  file( GLOB APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )
  foreach( sourcefile ${APP_SOURCES} )
      get_filename_component( filename ${sourcefile} NAME_WE )
      add_executable( ${filename} ${sourcefile} )
      target_compile_options( ${filename} PUBLIC ${TUBEX_CXX_FLAGS})
      target_include_directories( ${filename} SYSTEM PUBLIC ${TUBEX_INCLUDE_DIRS})
      target_link_libraries( ${filename} PUBLIC ${TUBEX_LIBRARIES} Ibex::ibex ${TUBEX_LIBRARIES})
  endforeach( sourcefile ${APP_SOURCES} )
