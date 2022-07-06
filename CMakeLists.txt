cmake_minimum_required(VERSION 3.16)
project(ex6)

set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -std=c++11")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(ex6 main.cpp anomaly_detection_util.h anomaly_detection_util.cpp AnomalyDetector.h HybridAnomalyDetector.h HybridAnomalyDetector.cpp minCircle.h minCircle.cpp SimpleAnomalyDetector.h SimpleAnomalyDetector.cpp timeseries.h timeseries.cpp CLI.cpp CLI.h commands.h Server.cpp Server.h)