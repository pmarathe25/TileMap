cmake_minimum_required(VERSION 3.10)

project(Tensor3)
# Enable OpenMP
add_openmp()
# Add install target for interface.
add_install_header(include/interfaces/Tensor3)
# Add test executable
add_executable(test0 test/test.cpp ${interface_file})
target_link_libraries(test0 -flto)
enable_testing()
add_test(NAME test COMMAND test0)
