cmake_minimum_required(VERSION 3.10)
project(graphics VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EMP_ROOT ../..)
set(RAYLIB_SRC_DIR ${EMP_ROOT}/third-party/raylib)
set(RAYLIB_BUILD_DIR raylib_build)

set(CMAKE_BUILD_TYPE Release)

add_subdirectory(${RAYLIB_SRC_DIR} ${RAYLIB_BUILD_DIR})

add_executable(app main.cpp)
target_include_directories(app 
  PRIVATE ${RAYLIB_BUILD_DIR}/raylib/include
  PRIVATE ${EMP_ROOT}/include
  )
target_link_libraries(app 
  PRIVATE raylib
  )

