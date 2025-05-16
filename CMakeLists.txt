cmake_minimum_required(VERSION 4.0.1)
project(trabalho_cg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_language(C)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

include(FetchContent)

# GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE) 
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)  
FetchContent_MakeAvailable(glfw)

# GLAD
add_library(glad src/glad/glad.c)
target_include_directories(glad PUBLIC include)

# GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# Assimp
FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG v5.3.1
)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(assimp)

# FreeType
FetchContent_Declare(
  freetype
  GIT_REPOSITORY https://github.com/freetype/freetype.git
  GIT_TAG VER-2-13-3
)
set(FT_WITH_ZLIB OFF CACHE BOOL "" FORCE)
set(FT_WITH_PNG OFF CACHE BOOL "" FORCE)
set(FT_WITH_BZIP2 OFF CACHE BOOL "" FORCE)
set(FT_WITH_HARFBUZZ OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(freetype)

# Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Includes
target_include_directories(${PROJECT_NAME}
  PRIVATE include
)

# Link libs
target_link_libraries(${PROJECT_NAME}
  PRIVATE glfw
  PRIVATE glm
  PRIVATE assimp
  PRIVATE glad
  PRIVATE freetype
)

# Warnings
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

