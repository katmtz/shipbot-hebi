cmake_minimum_required (VERSION 2.6)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
project (hebi_cpp_api)

#file(GLOB HEBI_SOURCEST
#  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
#  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

#set(HEBI_SOURCES $(HEBI_SOURCEST) PARENT_SCOPE)

set (HEBI_INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/src                   # CPP wrapper classes
  ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE) # Underlying C headers
set (HEBI_LINK_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/lib PARENT_SCOPE)

