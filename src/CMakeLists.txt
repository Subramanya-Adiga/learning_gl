# Main Executable Project
include(FetchContent)

add_executable(gl_learn main.cpp)

if(CMAKE_BUILD_TYPE STREQUAL "Release" AND (NOT CMAKE_CXX_SIMULATE_ID STREQUAL
                                            "MSVC"))
  target_compile_options(gl_learn PRIVATE /GL)
  target_link_options(gl_learn PRIVATE /LTCG)
endif()

target_sources(
  gl_learn
  PRIVATE sdl_imgui_gl.hpp
          sdl_imgui_gl.cpp
          defines.hpp
          file_utils.hpp
          file_utils.cpp
          known-folders.cpp
          known-folders.hpp
          image_utils.cpp
          image_utils.hpp
          shader.hpp
          shader.cpp
          buffer.cpp
          buffer.hpp
          buffer_layout.hpp
          buffer_layout.cpp
          vertex_array.cpp
          vertex_array.hpp
          texture.cpp
          texture.hpp)

find_package(SDL3 CONFIG REQUIRED)
# find_package(glm CONFIG REQUIRED)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG "v1.17.0")
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/icaven/glm.git
  GIT_TAG "dca38025fba63bb9284023e6de55f756b9e37cec")

set(SPDLOG_USE_STD_FORMAT ON)
set(GLM_FORCE_MESSAGES ON)
set(GLM_ENABLE_CXX_20 ON)
set(GLM_ENABLE_FAST_MATH ON)
set(GLM_ENABLE_SIMD_AVX2 ON)

FetchContent_MakeAvailable(spdlog glm)

if(WIN32)
  target_sources(
    gl_learn
    PRIVATE platform/win32/common.cpp
            platform/win32/common.hpp
            platform/win32/win32_known_folders.hpp
            platform/win32/win32_known_folders.cpp
            platform/win32/win32_file_utils.hpp
            platform/win32/win32_file_utils.cpp)

  add_custom_command(
    TARGET gl_learn
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared>
            $<TARGET_FILE_DIR:gl_learn>
    COMMENT "Copying SDL3.dll directory:
      $<TARGET_FILE:SDL3::SDL3-shared> to $<TARGET_FILE_DIR:gl_learn>"
    VERBATIM)
endif()

if(LINUX)
  target_sources(
    gl_learn
    PRIVATE platform/linux/common.cpp platform/linux/common.hpp
            platform/linux/linux_known_folders.cpp
            platform/linux/linux_known_folders.hpp)
endif()

target_compile_definitions(gl_learn PRIVATE IMGUI_IMPL_OPENGL_LOADER_CUSTOM)
target_include_directories(gl_learn PRIVATE "${CMAKE_SOURCE_DIR}/vendor/stb")

target_link_libraries(
  gl_learn
  PRIVATE project_options
          project_dependency
          GLAD
          SDL3::SDL3
          DearImGui
          glm::glm-header-only
          spdlog::spdlog_header_only)

# Copy Resource Directory To Build Directroy
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
set(DESTINATION_DIR "$<TARGET_FILE_DIR:gl_learn>/resources")

add_custom_target(
  copy_data_dir
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${SOURCE_DIR}"
          "${DESTINATION_DIR}"
  COMMENT "Copying data directory: ${SOURCE_DIR} to ${DESTINATION_DIR}")
add_dependencies(gl_learn copy_data_dir)
