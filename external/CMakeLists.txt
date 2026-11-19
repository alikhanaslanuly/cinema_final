include(FetchContent)

# CMake 4.x refuses very old cmake_minimum_required in third-party projects (glm)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# Dependencies are downloaded once at configure time into external/_deps (git-ignored).
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external/_deps CACHE PATH "" FORCE)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

FetchContent_Declare(glfw  GIT_REPOSITORY https://github.com/glfw/glfw.git        GIT_TAG 3.4     GIT_SHALLOW TRUE)
FetchContent_Declare(glm   GIT_REPOSITORY https://github.com/g-truc/glm.git       GIT_TAG 1.0.1   GIT_SHALLOW TRUE)
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git    GIT_TAG v1.91.5 GIT_SHALLOW TRUE)
FetchContent_Declare(stb   GIT_REPOSITORY https://github.com/nothings/stb.git     GIT_TAG master  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(glfw glm imgui stb)

# glad: minimal GL 3.3 core loader, vendored in external/glad
add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
target_compile_definitions(glad PUBLIC GLFW_INCLUDE_NONE)

# Dear ImGui + GLFW/OpenGL3 backends
add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_compile_definitions(imgui PUBLIC GLFW_INCLUDE_NONE)
target_link_libraries(imgui PUBLIC glfw)

# stb (header-only)
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
