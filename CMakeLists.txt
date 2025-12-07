cmake_minimum_required(VERSION 3.16)
project(seating-chart)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wno-braced-scalar-init")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PGO_MODE "" CACHE STRING "PGO mode: GEN or USE")

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  message(STATUS "Enabling ThinLTO")
  add_compile_options(-flto=thin)

  if (APPLE)
    add_link_options(-flto=thin)
  else()
    add_link_options(-flto=thin -fuse-ld=lld)

    # Force llvm-ar/ranlib for static libraries on Linux
    find_program(LLVM_AR NAMES llvm-ar)
    find_program(LLVM_RANLIB NAMES llvm-ranlib)

    if (LLVM_AR AND LLVM_RANLIB)
      set(CMAKE_AR "${LLVM_AR}" CACHE FILEPATH "" FORCE)
      set(CMAKE_RANLIB "${LLVM_RANLIB}" CACHE FILEPATH "" FORCE)
    endif()
  endif()
endif()

if(PGO_MODE STREQUAL "GEN")
  message(STATUS "Configuring for PGO Generation")
  add_compile_options(-fprofile-instr-generate)
  add_link_options(-fprofile-instr-generate)
elseif(PGO_MODE STREQUAL "USE")
  message(STATUS "Configuring for PGO Use")
  # Make sure to point to the correct location
  set(PGO_PROFILE_PATH "${CMAKE_SOURCE_DIR}/profiles/default.profdata")

  if(EXISTS ${PGO_PROFILE_PATH})
    add_compile_options(-fprofile-instr-use=${PGO_PROFILE_PATH})
    # Link options aren't strictly required for USE, but good for LTO
    add_link_options(-fprofile-instr-use=${PGO_PROFILE_PATH})
  else()
    message(FATAL_ERROR "PGO profile data not found at ${PGO_PROFILE_PATH}")
  endif()
endif()

add_executable(seating-chart src/main.cpp src/seating_chart.cpp)
