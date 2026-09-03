cmake_minimum_required(VERSION 4.1)

project(
  S4SAR
  VERSION 0.1.0
  DESCRIPTION "Simple Stripmap Signal Simulator for Synthetic Aperture Radar"
  LANGUAGES CXX
)

set(S4SAR_VERSION_SUFFIX "rc1")
set(
  S4SAR_FULL_VERSION
  "${PROJECT_VERSION}-${S4SAR_VERSION_SUFFIX}"
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
  s4sar 
  apps/s4sar.cpp
)

 target_include_directories(
   s4sar  
   PRIVATE
     ${CMAKE_CURRENT_SOURCE_DIR}/include
 )
