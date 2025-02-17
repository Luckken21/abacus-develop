remove_definitions(-D__MPI)
remove_definitions(-D__EXX)
remove_definitions(-D__CUDA)
remove_definitions(-D__UT_USE_CUDA)
remove_definitions(-D__UT_USE_ROCM)
remove_definitions(-D__ROCM)
remove_definitions(-D__DEEPKS)
remove_definitions(-D_OPENMP)
remove_definitions(-DUSE_PAW)

install(DIRECTORY support DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

AddTest(
  TARGET Elecstate_Op_UTs
  LIBS ${math_libs} psi base device
  SOURCES ../kernels/test/elecstate_op_test.cpp
)

AddTest(
  TARGET elecstate_occupy
  LIBS ${math_libs} base device
  SOURCES elecstate_occupy_test.cpp ../occupy.cpp
)

AddTest(
  TARGET elecstate_magnetism
  LIBS ${math_libs} base device
  SOURCES elecstate_magnetism_test.cpp ../magnetism.cpp
)

AddTest(
  TARGET elecstate_fp_energy
  LIBS ${math_libs} base device
  SOURCES elecstate_fp_energy_test.cpp ../fp_energy.cpp
)

AddTest(
  TARGET elecstate_print
  LIBS ${math_libs} base device
  SOURCES elecstate_print_test.cpp ../elecstate_print.cpp ../occupy.cpp
)

AddTest(
  TARGET elecstate_base
  LIBS ${math_libs} base device
  SOURCES elecstate_base_test.cpp ../elecstate.cpp ../occupy.cpp ../../module_psi/psi.cpp
)

AddTest(
  TARGET elecstate_pw
  LIBS ${math_libs} planewave_serial base device
  SOURCES elecstate_pw_test.cpp
    ../elecstate_pw.cpp
    ../elecstate.cpp
    ../occupy.cpp
    ../../module_psi/psi.cpp
    ../../module_psi/kernels/psi_memory_op.cpp
)

AddTest(
  TARGET elecstate_energy
  LIBS ${math_libs} base device
  SOURCES elecstate_energy_test.cpp ../elecstate_energy.cpp ../fp_energy.cpp
)

AddTest(
  TARGET potentials_base
  SOURCES potentials_base_test.cpp
)

AddTest(
  TARGET potentials_new
  LIBS ${math_libs} base device planewave_serial
  SOURCES potential_new_test.cpp ../potentials/potential_new.cpp
)

AddTest(
  TARGET charge_test
  LIBS ${math_libs} planewave_serial base device cell_info
  SOURCES charge_test.cpp ../module_charge/charge.cpp
    ../../module_io/output.cpp
)

AddTest(
  TARGET charge_mixing
  LIBS  base ${math_libs}  psi  device planewave_serial cell_info
  SOURCES charge_mixing_test.cpp  
  ../module_charge/charge_mixing.cpp
  ../../module_io/output.cpp
)

AddTest(
  TARGET charge_extra
  LIBS ${math_libs} base device cell_info
  SOURCES charge_extra_test.cpp ../module_charge/charge_extra.cpp ../../module_io/read_cube.cpp ../../module_io/write_cube.cpp
  ../../module_io/output.cpp
)
