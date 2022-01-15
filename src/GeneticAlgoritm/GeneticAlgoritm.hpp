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
    int calculatePath(vector<int> &path);
    void orderedCrossover(vector<int> &parent1, vector<int> &parent2) const;
    void partiallyCrossover(vector<int> &parent1, vector<int> &parent2) const;
    static bool isInPath(int element, int begin, int end, vector<int> &path);
    int apply(bool crossing);
    static vector<int> insert(vector<int> &permutation, int first, int second);
    vector<vector<int>> makePopulation() const;
    void selection(vector<int> fitness, vector<vector<int>> &population) const;
};


#endif //PEA_PROJECT_3_GENETICALGORITM_HPP
