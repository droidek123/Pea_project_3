//
// Created by Andrzej on 13.01.2022.
//

#include "GeneticAlgoritm.hpp"
#include <random>
#include <ctime>

GeneticAlgorithm::GeneticAlgorithm(const Graph& graph, int stop, int population, float crossRate, float mutationRate) {
    matrix = graph.matrix;
    size = graph.number_of_vertices;
    this->stop = stop;
    populationSize = population;
    if(crossRate == 0)
        this->crossRate = 0.8;
    else
        this->crossRate = crossRate;
    if(mutationRate == 0)
        this->mutationRate = 0.01;
    else
        this->mutationRate = mutationRate;
}

int GeneticAlgorithm::calculatePath(std::vector<int> &path) {
    int result = 0;

    for (int i = 0; i < size - 1; i++)
        result += matrix[path[i]][path[i + 1]];

    result += matrix[path[size - 1]][path[0]];

    return result;
}

void GeneticAlgorithm::partiallyCrossover(std::vector<int> &parent1, std::vector<int> &parent2) const {
    std::vector<int> desc1(size, -1), desc2(size, -1);
    std::vector<int> map1(size, -1), map2(size, -1);
    int begin, end, temp;

    do {
        begin = rand() % size;
        end = rand() % size;
    } while ((0 >= (end - begin)) || !begin || !(end - (size - 1)));

    for (int i = begin; i <= end; i++) {
        desc1[i] = parent1[i];
        desc2[i] = parent2[i];
        map1[parent1[i]] = parent2[i];
        map2[parent2[i]] = parent1[i];
    }

    for (int i = 0; i < size; i++) {
        if (desc1[i] == -1) {
            if (!isInPath(parent2[i], begin, end, desc1))
                desc1[i] = parent2[i];
            else {
                temp = parent2[i];

                do {
                    temp = map1[temp];
                } while (isInPath(temp, begin, end, desc1));

                desc1[i] = temp;
            }
        }
    }

    for (int i = 0; i < size; i++) {
        if (desc2[i] == -1) {
            if (!isInPath(parent1[i], begin, end, desc2))
                desc2[i] = parent1[i];
            else {
                temp = parent1[i];

                do {
                    temp = map2[temp];
                } while (isInPath(temp, begin, end, desc2));

                desc2[i] = temp;
            }
        }
    }

    parent1.clear();
    parent2.clear();
    parent1 = desc1;
    parent2 = desc2;
}

void GeneticAlgorithm::orderedCrossover(std::vector<int> &parent1, std::vector<int> &parent2) const {
    std::vector<int> desc1(size), desc2(size);
    std::vector<int>::iterator itr1, itr2;
    int begin, end;

    do {
        begin = rand() % size;
        end = rand() % size;
    } while ((0 >= (end - begin)) || !begin || !(end - (size - 1)));

    for (int i = begin; i <= end; i++) {
        desc1[i] = parent1[i];
        desc2[i] = parent2[i];
    }

    itr1 = desc1.begin() + end + 1;
    itr2 = parent2.begin() + end + 1;

    while (itr1 != desc1.begin() + begin) {
        if (!(isInPath(*itr2, begin, end, desc1))) {
            *itr1 = *itr2;

            if (itr1 == desc1.end() - 1)
                itr1 = desc1.begin();
            else
                itr1++;

            if (itr2 == parent2.end() - 1)
                itr2 = parent2.begin();
            else
                itr2++;
        } else {
            if (itr2 == parent2.end() - 1)
                itr2 = parent2.begin();
            else
                itr2++;
        }
    }

    itr1 = desc2.begin() + end + 1;
    itr2 = parent1.begin() + end + 1;

    while (itr1 != desc2.begin() + begin) {
        if (!(isInPath(*itr2, begin, end, desc2))) {
            *itr1 = *itr2;

            if (itr1 == desc2.end() - 1)
                itr1 = desc2.begin();
            else
                itr1++;

            if (itr2 == parent1.end() - 1)
                itr2 = parent1.begin();
            else
                itr2++;
        } else {
            if (itr2 == parent1.end() - 1)
                itr2 = parent1.begin();
            else
                itr2++;
        }
    }

    parent1.clear();
    parent2.clear();
    parent1 = desc1;
    parent2 = desc2;
}

bool GeneticAlgorithm::isInPath(int element, int begin, int end, std::vector<int> &path) {
    for (int i = begin; i <= end; i++) {
        if (element == path[i])
            return true;
    }
    return false;
}

int GeneticAlgorithm::apply(bool crossing) {
    std::vector<std::vector<int>> population, nextPopulation;
    std::vector<int> fitness, permutation;
    int tournamentSize = 5;
    int index, result, p1, p2;
    std::clock_t start;

    for (int i = 0; i < size; i++)
        permutation.push_back(i);

    for (int i = 0; i < populationSize; i++) {
        shuffle(permutation.begin(), permutation.end(),mt19937(random_device()()));
        population.push_back(permutation);
    }

    start = std::clock();

    // Kolejne iteracje algorytmu
    while (((std::clock() - start) / (CLOCKS_PER_SEC)) < stop) {
        fitness.clear();

        // Ocena jakości osobników
        for (auto &itr : population)
            fitness.push_back(calculatePath(itr));

        // Tworzenie nowej populacji na drodze selekcji
        for (int j = 0; j < populationSize; j++) {
            result = INT32_MAX;

            // Wybór najlepszego osobnika z turnieju
            for (int k = 0; k < tournamentSize; k++) {
                index = rand() % populationSize;

                if (fitness[index] < result) {
                    result = fitness[index];
                    permutation.clear();
                    permutation = population[index];
                }
            }
            nextPopulation.push_back(permutation);
        }

        // Wymiana pokoleń
        for (auto &itr : population)
            itr.clear();

        population.clear();
        population = nextPopulation;

        for (auto &itr : nextPopulation)
            itr.clear();

        nextPopulation.clear();

        // Krzyżowanie osobników
        for (int j = 0; j < (int) (crossRate * (float) populationSize); j += 2) {
            p1 = rand() % populationSize;
            do {
                p2 = rand() % populationSize;
            } while (p1 == p2);

            if (crossing)
                orderedCrossover(population.at(j), population.at(j + 1));
            else
                partiallyCrossover(population.at(j), population.at(j + 1));
        }

        // Mutacje osobników
        for (int j = 0; j < (int) (mutationRate * (float) populationSize); j++) {
            p1 = rand() % size;
            do {
                p2 = rand() % size;
            } while (p1 == p2);

            swap(population.at(j)[p1], population.at(j)[p2]);
        }
    }

    result = *(min_element(fitness.begin(), fitness.end()));
    return result;
}