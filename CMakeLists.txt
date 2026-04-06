cmake_minimum_required(VERSION 3.28)

project(
  learning_opengl
  VERSION 1.0.0.0
  LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

if(WIN32)
  list(APPEND CMAKE_MODULE_PATH $ENV{SDL3_DIR})
endif()

include(FetchContent)
include(StanderdProjectSettings)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_23)

if(MINGW OR MSVC)
  target_compile_definitions(project_options INTERFACE _CRT_SECURE_NO_WARNINGS
                                                       NOMINMAX UNICODE)
  if(MINGW)
    target_link_libraries(project_options INTERFACE stdc++exp)
  endif()
endif()

set_project_warnings(project_options)

add_library(project_dependency INTERFACE)
target_precompile_headers(
  project_dependency
  INTERFACE
  <vector>
  <string>
  <map>
  <unordered_map>
  <utility>
  <string_view>
  <array>
  <algorithm>
  <memory>
  <filesystem>
  <iostream>
  <tuple>
  <variant>
  <print>
  <cstdint>
  <cstring>
  <functional>)

message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX}")
include(GNUInstallDirs)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKEDIR CMake)
else()
  set(DEF_INSTALL_CMAKEDIR share/cmake/${PROJECT_NAME})
endif()
set(INSTALL_CMAKEDIR
    ${DEF_INSTALL_CMAKEDIR}
    CACHE PATH "Installation directory for CMake files")

foreach(p LIB BIN INCLUDE CMAKE)
  file(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${INSTALL_${p}DIR} _path)
  message(STATUS "Installing ${p} components to ${_path}")
  unset(_path)
endforeach()

add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/GLAD")
add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/imgui")
add_subdirectory("${CMAKE_SOURCE_DIR}/src")

add_custom_target(
  copy_compile_commands ALL
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/compile_commands.json"
    "${CMAKE_SOURCE_DIR}/compile_commands.json"
  DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
  COMMENT "Copying compile_commands.json to source directory")

add_dependencies(copy_compile_commands project_options)

if(BUILD_TESTING)
  FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG "v3.13.0")
  FetchContent_MakeAvailable(catch2)

  add_subdirectory("${CMAKE_SOURCE_DIR}/tests")

endif()
