cmake_minimum_required(VERSION 3.10.0)
project(sharqit)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_INSTALL_PREFIX $ENV{HOME})

set(LIB_SRC src/fraction.cpp src/phase.cpp src/binary_matrix.cpp src/qgate.cpp
  src/optimizer.cpp src/linear_map.cpp
  src/qcirc/qcirc.cpp src/qcirc/merge_rotation.cpp src/qcirc/propagate_pauli_x.cpp
  src/zx/zxnode.cpp src/zx/zxdiagram.cpp src/zx/simplify.cpp src/zx/extract_qcirc.cpp
  src/dag/dagnode.cpp src/dag/dagedge.cpp src/dag/dagcirc.cpp)

set(INCLUDE_SRC src/sharqit.h src/util.h src/fraction.h src/phase.h
  src/binary_matrix.h src/linear_map.h src/qgate.h src/qcirc.h src/zx.h
  src/dag.h src/optimizer.h)

add_library(shrqt SHARED ${LIB_SRC})
target_include_directories(shrqt PRIVATE src /usr/include/eigen3)
target_compile_options(shrqt PUBLIC -O4 -Wall)
target_compile_features(shrqt PUBLIC cxx_std_17)

add_executable(sharqit src/sharqit.cpp)
target_include_directories(sharqit PRIVATE src /usr/include/eigen3)
target_compile_options(sharqit PUBLIC -O4 -Wall)
target_compile_features(sharqit PUBLIC cxx_std_17)
target_link_libraries(sharqit shrqt)

install(TARGETS sharqit DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS shrqt DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES ${INCLUDE_SRC} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/sharqit)
