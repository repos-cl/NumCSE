project(fftreal)
cmake_minimum_required(VERSION 2.8)

add_executable_numcse(main main.cpp)
set_eigen_fft_backend(main "Kiss FFT")
