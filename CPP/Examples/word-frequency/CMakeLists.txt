cmake_minimum_required(VERSION 3.20)
project(word_frequency LANGUAGES CXX)

add_executable(word_frequency main.cpp)
target_compile_features(word_frequency PRIVATE cxx_std_20)

if(MSVC)
  target_compile_options(word_frequency PRIVATE /W4 /permissive-)
else()
  target_compile_options(word_frequency PRIVATE -Wall -Wextra -Wpedantic -Wconversion)
endif()

