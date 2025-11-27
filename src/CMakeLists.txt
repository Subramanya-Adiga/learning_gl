# Main Executable Project

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

if(WIN32)
  target_sources(
    gl_learn
    PRIVATE platform/platform_win32.cpp platform/platform_win32.hpp
            platform/win32_known_folders.hpp platform/win32_known_folders.cpp
            platform/win32_file_utils.hpp platform/win32_file_utils.cpp)
endif()

target_include_directories(gl_learn PRIVATE "${CMAKE_SOURCE_DIR}/vendor/stb")

find_package(SDL3 CONFIG REQUIRED)

target_link_libraries(gl_learn PRIVATE project_options project_dependency GLAD
                                       SDL3::SDL3)
