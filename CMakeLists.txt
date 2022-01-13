cmake_minimum_required(VERSION 3.21)
project(Pea_project_3)

set(CMAKE_CXX_STANDARD 20)

add_executable(Pea_project_3 src/main.cpp src/Graph/Graph.hpp src/Graph/Graph.cpp src/GeneticAlgoritm/GeneticAlgoritm.cpp src/GeneticAlgoritm/GeneticAlgoritm.hpp)

file(COPY sample_data/tsp17.txt sample_data/tsp4.txt
        sample_data/tsp6_1.txt sample_data/tsp6_2.txt sample_data/tsp10.txt sample_data/tsp12.txt
        sample_data/tsp13.txt sample_data/tsp14.txt sample_data/ftv55.txt sample_data/ftv170.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
