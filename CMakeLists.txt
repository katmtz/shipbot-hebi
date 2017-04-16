cmake_minimum_required (VERSION 2.6)
project (hebi_cpp_test)

# ================= import library and add dependencies

SET (CMAKE_CXX_STANDARD 11)
SET (CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_subdirectory (hebi)
include_directories (${HEBI_INCLUDE_DIRECTORIES})
link_directories (${HEBI_LINK_DIRECTORIES})
# Packaging sources into object library allows reuse if you have multiple targets
# add_library(hebi_cpp_files OBJECT ${HEBI_SOURCES})
add_library(hebi_cpp_files OBJECT 
  hebi/src/mac_address.cpp
  hebi/src/command.cpp
  hebi/src/feedback.cpp
  hebi/src/info.cpp
  hebi/src/group_command.cpp
  hebi/src/group_feedback.cpp
  hebi/src/group_info.cpp
  hebi/src/module.cpp
  hebi/src/group.cpp
  hebi/src/lookup.cpp
)


# ================= build examples

# Build all examples:
SET(TEST_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/armcontrol/armcontrol.cpp
)

add_custom_target(arm) # metatarget for all examples
foreach (TEST ${TEST_SOURCES})
  get_filename_component(EX_NAME ${TEST} NAME_WE)
  add_executable(${EX_NAME} ${TEST} $<TARGET_OBJECTS:hebi_cpp_files>)
  add_dependencies(arm ${EX_NAME}) # add to metatarget
  if (WIN32) 
  target_link_libraries( ${EX_NAME} hebi kernel32 ) # kernel32 for sleep commands.
  else()
  target_link_libraries( ${EX_NAME} hebi m pthread )
  endif()
  # Add ultra-conservative warnings.
  if ( CMAKE_COMPILER_IS_GNUCC )
    set_property( TARGET ${EX_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS -Wall )
  endif ( CMAKE_COMPILER_IS_GNUCC )
  # For Windows, we copy the .dll file into the binary directory so that we don't have to set the PATH variable.
  if (WIN32)
  add_custom_command( TARGET ${EX_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/lib/hebi.dll"
    $<TARGET_FILE_DIR:${EX_NAME}>)
  endif()
endforeach (TEST ${TEST_SOURCES})
