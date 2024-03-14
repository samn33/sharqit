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
set(LIB_SRC fraction.cpp phase.cpp binary_matrix.cpp
  qgate.cpp optimizer.cpp linear_map.cpp
  qcirc.cpp merge_rotation.cpp propagate_pauli_x.cpp
  zxnode.cpp zxdiagram.cpp simplify.cpp
  extract_qcirc.cpp dagnode.cpp dagedge.cpp
  dagcirc.cpp)

set(INC_SRC sharqit.h util.h fraction.h phase.h
  binary_matrix.h linear_map.h qgate.h qcirc.h
  zx.h dag.h optimizer.h)

# library
add_library(shrqt SHARED ${INC_SRC} ${LIB_SRC})
set_target_properties(shrqt PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(shrqt PUBLIC /usr/include/eigen3)
target_compile_options(shrqt PUBLIC -O4 -Wall)
target_compile_features(shrqt PUBLIC cxx_std_17)

# command
add_executable(sharqit sharqit.cpp)
target_include_directories(sharqit PRIVATE /usr/include/eigen3)
target_compile_options(sharqit PUBLIC -O4 -Wall)
target_compile_features(sharqit PUBLIC cxx_std_17)
target_link_libraries(sharqit shrqt)

# install
install(TARGETS shrqt DESTINATION $ENV{HOME}/lib)
install(FILES ${INC_SRC} DESTINATION $ENV{HOME}/include/sharqit)
install(TARGETS sharqit DESTINATION $ENV{HOME}/bin)
