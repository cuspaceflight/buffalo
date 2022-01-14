
cmake_minimum_required( VERSION 3.9.1 )

project(state_estimation_int)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(src_files main.c state_estimation.c config.c fc.c)

add_executable(main ${src_files})
set_target_properties(main PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(main m)
