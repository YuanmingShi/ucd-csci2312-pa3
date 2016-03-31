cmake_minimum_required(VERSION 3.3)
project(clustering)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ffloat-store")

set(SOURCE_FILES main.cpp Point.cpp Point.h Cluster.cpp Cluster.h
ErrorContext.cpp ErrorContext.h ClusteringTests.cpp ClusteringTests.h KMeans.cpp KMeans.h Exceptions.h Exceptions.cpp Point.cpp Exceptions.cpp KMeans.cpp Cluster.cpp)
add_executable(clustering ${SOURCE_FILES})


