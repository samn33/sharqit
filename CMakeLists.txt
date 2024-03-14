cmake_minimum_required(VERSION 3.15...3.27)
project(sharqit_base)
find_package(Python 3.8 COMPONENTS Interpreter Development)

# Detect the installed nanobind package and import it into CMake
execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NB_DIR)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
find_package(nanobind CONFIG REQUIRED)

# c++ source codes
set(LIB_SRC sharqit/cpp/fraction.cpp sharqit/cpp/phase.cpp sharqit/cpp/binary_matrix.cpp
  sharqit/cpp/qgate.cpp sharqit/cpp/optimizer.cpp sharqit/cpp/linear_map.cpp
  sharqit/cpp/qcirc.cpp sharqit/cpp/merge_rotation.cpp sharqit/cpp/propagate_pauli_x.cpp
  sharqit/cpp/zxnode.cpp sharqit/cpp/zxdiagram.cpp sharqit/cpp/simplify.cpp
  sharqit/cpp/extract_qcirc.cpp sharqit/cpp/dagnode.cpp sharqit/cpp/dagedge.cpp
  sharqit/cpp/dagcirc.cpp sharqit/cpp/sharqit.cpp)

set(INC_SRC sharqit/cpp/sharqit.h sharqit/cpp/util.h sharqit/cpp/fraction.h sharqit/cpp/phase.h
  sharqit/cpp/binary_matrix.h sharqit/cpp/linear_map.h sharqit/cpp/qgate.h sharqit/cpp/qcirc.h
  sharqit/cpp/zx.h sharqit/cpp/dag.h sharqit/cpp/optimizer.h)

# c++ library
add_library(shrqt SHARED ${INC_SRC} ${LIB_SRC})
set_target_properties(shrqt PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(shrqt PUBLIC /usr/include/eigen3)
target_compile_options(shrqt PUBLIC -O4 -Wall)
target_compile_features(shrqt PUBLIC cxx_std_17)

# python package
nanobind_add_module(sharqit_base ${INC_SRC} sharqit/cpp/nb_sharqit.cpp)
target_link_libraries(sharqit_base PRIVATE shrqt)
