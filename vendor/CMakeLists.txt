add_subdirectory(glad/cmake)
glad_add_library(glad STATIC API gl:core=4.6)

set(GLM_QUIET OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(glm)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
set_target_properties(glfw PROPERTIES FOLDER vendor)

set(FASTNOISE2_NOISETOOL OFF CACHE BOOL "" FORCE)
add_subdirectory(FastNoise2)

# project(Imgui)
# find_package(OpenGL REQUIRED)
# file(GLOB_RECURSE imguiFiles CONFIGURE_DEPENDS "imgui/*.cpp" "imgui/*.h")
# add_library(Imgui STATIC ${imguiFiles})
# target_include_directories(Imgui PUBLIC "imgui/" "imgui/backends")
# set(linker "")
# if (WIN32)
#     set (linker "-limm32")
# elseif (UNIX)
#     set (linker "")
# endif()
#
# target_link_libraries(Imgui PRIVATE ${linker}
#     PUBLIC
#     glfw
#     OpenGL::GL
# )
