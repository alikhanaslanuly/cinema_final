cmake_minimum_required(VERSION 3.20)
project(cinema3d VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# glad (vendored), glfw / glm / imgui / stb (downloaded once into external/_deps)
add_subdirectory(external)

# ---- core (owner: Alikhan) ---------------------------------------------------
file(GLOB CORE_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/core/*.cpp)

# ---- modules of teammates ----------------------------------------------------
# For every module directory: if src/<module> already contains .cpp files they are
# used. Otherwise a tiny placeholder from dev_stubs/<module> is compiled so the
# whole app still builds and runs while the teammate is not ready.
set(MODULE_SOURCES "")
set(USING_STUBS OFF)
foreach(m scene picking game ui)
  file(GLOB_RECURSE real CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/${m}/*.cpp)
  if(real)
    list(APPEND MODULE_SOURCES ${real})
    message(STATUS "module '${m}': using src/${m}")
  else()
    file(GLOB_RECURSE stub CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/dev_stubs/${m}/*.cpp)
    list(APPEND MODULE_SOURCES ${stub})
    set(USING_STUBS ON)
    message(STATUS "module '${m}': src/${m} is empty -> using placeholder from dev_stubs/${m}")
  endif()
endforeach()

add_executable(cinema3d src/main.cpp ${CORE_SOURCES} ${MODULE_SOURCES})
target_include_directories(cinema3d PRIVATE ${CMAKE_SOURCE_DIR}/src)
if(USING_STUBS)
  # after src/, so real headers always win over placeholders
  target_include_directories(cinema3d PRIVATE ${CMAKE_SOURCE_DIR}/dev_stubs)
endif()
target_compile_definitions(cinema3d PRIVATE GLFW_INCLUDE_NONE CINEMA_ROOT_DIR="${CMAKE_SOURCE_DIR}")
target_link_libraries(cinema3d PRIVATE glad glfw glm::glm imgui stb)

if(MSVC)
  target_compile_options(cinema3d PRIVATE /utf-8 /W3)
  set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT cinema3d)
  set_target_properties(cinema3d PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
else()
  target_compile_options(cinema3d PRIVATE -Wall -Wextra)
endif()
