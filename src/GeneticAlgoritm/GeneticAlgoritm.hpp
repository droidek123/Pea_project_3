//
// Created by Andrzej on 13.01.2022.
//

#ifndef PEA_PROJECT_3_GENETICALGORITM_HPP
#define PEA_PROJECT_3_GENETICALGORITM_HPP


#include "../Graph/Graph.hpp"
#include <vector>

class GeneticAlgorithm
{
private:

    int stop;
    int populationSize;
    float crossRate;
    float mutationRate;
    vector<vector<int>> matrix;
    int size;

public:

    GeneticAlgorithm(const Graph& graph, int stop, int population, float crossRate, float mutationRate);
    int calculatePath(std::vector<int> &path);
    void orderedCrossover(std::vector<int> &parent1, std::vector<int> &parent2) const;
    void partiallyCrossover(std::vector<int> &parent1, std::vector<int> &parent2) const;
    static bool isInPath(int element, int begin, int end, std::vector<int> &path);
    int apply(bool crossing);
};


#endif //PEA_PROJECT_3_GENETICALGORITM_HPP
