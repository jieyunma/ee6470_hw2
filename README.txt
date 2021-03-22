File Structure:
  |-- hw2_report.pdf
  |-- lena_std_short.bmp
  |-- guassian_sc
  |   |-- CMakeLists.txt
  |   |-- main.cpp
  |   |-- Testbench.h
  |   |-- Testbench.cpp
  |   |-- GaussFilter.h
  |   |-- GaussFilter.cpp
  |   |-- filter_def.h
 
How to run? (use lena_std_short.bmp as input image)
  1. change directory to gaussian_sc, type "cd gaussian_sc"
  2. build new directory, type "mkdir build".
  3. change directory to build, type "cd build"
  4. type "cmake ..".
  5. type "make run".
  6. you will get output image "out.bmp".







