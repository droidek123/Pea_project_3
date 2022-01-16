#include "GeneticAlgoritm.hpp"
#include <random>
#include <ctime>
#include <iostream>

using namespace std;

GeneticAlgorithm::GeneticAlgorithm(const Graph &graph, int stop, int population, float crossRate, float mutationRate) {
    matrix = graph.matrix;
    size = graph.number_of_vertices;
    this->stop = stop;
    populationSize = population;
    if (crossRate == 0)
        this->crossRate = 0.8;
    else
        this->crossRate = crossRate;
    if (mutationRate == 0)
        this->mutationRate = 0.01;
    else
        this->mutationRate = mutationRate;
}

int GeneticAlgorithm::calculatePath(vector<int> &path) {
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

    begin = rand() % size;
    do {
        end = rand() % size;
    } while (begin == end);

    if (begin > end)
        swap(begin, end);

    for (int i = begin; i <= end; i++) {
        desc1.at(i) = parent1.at(i);
        desc2.at(i) = parent2.at(i);
        map1[parent1.at(i)] = parent2.at(i);
        map2[parent2.at(i)] = parent1.at(i);
    }

    for (int i = 0; i < size; i++) {
        if (desc1.at(i) == -1) {

            if (*find(desc1.begin() + begin, desc1.begin() + end, parent2.at(i)) != parent2.at(i))
                desc1.at(i) = parent2.at(i);
            else {
                temp = parent2.at(i);
                do {
                    temp = map1[temp];
                } while (*find(desc1.begin() + begin, desc1.begin() + end, temp) == temp);

                desc1.at(i) = temp;
            }
        }
        if (desc2.at(i) == -1) {
            if (*find(desc2.begin() + begin, desc2.begin() + end, parent1.at(i)) != parent1.at(i))
                desc2.at(i) = parent1.at(i);
            else {
                temp = parent1.at(i);
                do {
                    temp = map2[temp];
                } while (*find(desc2.begin() + begin, desc2.begin() + end, temp) == temp);

                desc2.at(i) = temp;
            }
        }
    }
    parent1 = desc1;
    parent2 = desc2;
}

void GeneticAlgorithm::orderedCrossover(vector<int> &parent1, vector<int> &parent2) const {
    vector<int> temp1(size), temp2(size);

    int begin, end;

    temp1 = parent1;
    temp2 = parent2;

    begin = rand() % size;
    do {
        end = rand() % size;
    } while (begin == end);

    if (begin > end)
        swap(begin, end);


    int p1 = 0;
    int p2 = 0;
    for (int i = 0; i < size; i++) {

        if (p1 == begin) p1 = end + 1;
        if (p2 == begin) p2 = end + 1;

        if (i >= begin && i <= end) {
            parent1[i] = temp2[i];
            parent2[i] = temp1[i];
        }

        if (*find(temp2.begin() + begin, temp2.begin() + end, temp1[i]) != temp1[i]) {
            parent1[p1] = temp1[i];
            p1++;
        }
        if (*find(temp1.begin() + begin, temp1.begin() + end, temp2[i]) != temp2[i]) {
            parent2[p2] = temp2[i];
            p2++;
        }
    }
}

vector<vector<int>> GeneticAlgorithm::makePopulation() const {
    vector<int> permutation(size);
    vector<vector<int>> population(populationSize);

    for (int i = 0; i < size; i++) permutation[i] = i;

    for (int i = 0; i < populationSize; i++) {
        shuffle(permutation.begin(), permutation.end(), mt19937(random_device()()));
        population[i] = permutation;
    }
    return population;
}

void GeneticAlgorithm::selection(vector<int> fitness, vector<vector<int>> &population) const {
    int tournamentSize = 5;
    int index;
    vector<vector<int>> nextPopulation(populationSize);
    vector<int> permutation(size);
    for (int j = 0; j < populationSize; j++) {
        int result = INT32_MAX;

        // Wybór najlepszego osobnika z turnieju
        for (int k = 0; k < tournamentSize; k++) {
            index = rand() % populationSize;

            if (fitness[index] < result) {
                result = fitness[index];
                permutation = population[index];
            }
        }
        nextPopulation[j] = permutation;
    }

    // Wymiana pokoleń
    population.swap(nextPopulation);
}

void GeneticAlgorithm::apply(Crossing crossing, Mutation mutation) {
    vector<vector<int>> population(populationSize), nextPopulation(populationSize);
    vector<int> fitness(populationSize), permutation(size);
    int result, p1, p2, temp;
    clock_t start;

    population = makePopulation();


    start = std::clock();

    // Kolejne iteracje algorytmu
    while ((std::clock() - start) / (CLOCKS_PER_SEC) < stop) {
        // Ocena jakości osobników
        for (size_t idx = 0; auto &itr: population) {
            temp = calculatePath(itr);
            fitness[idx] = temp;
            if(temp < best) {
                best = temp;
                cout << to_string(best) + " czas: " + to_string((double)(clock() - start) / (CLOCKS_PER_SEC))  << endl;
            }
            idx++;
        }

        // Tworzenie nowej populacji na drodze selekcji
        selection(fitness, population);

        // Krzyżowanie osobników
        for (int j = 0; j < (int) (crossRate * (float) populationSize); j++) {
            p1 = rand() % size;
            do {
                p2 = rand() % size;
            } while (p1 == p2);
            if (crossing)
                orderedCrossover(population.at(p1), population.at(p2));
            else
                partiallyCrossover(population.at(p1), population.at(p2));
        }

        // Mutacje osobników
        for (int j = 0; j < (int) (mutationRate * (float) populationSize); j++) {
            p1 = rand() % size;
            do {
                p2 = rand() % size;
            } while (p1 == p2);

            if (mutation)
                insert(population[j], p1, p2);
            else
                swap(population.at(j)[p1], population.at(j)[p2]);
        }
    }

    result = *(min_element(fitness.begin(), fitness.end()));
    if(result < best)
        best = result;
    cout << to_string(best) + " czas: " + to_string((double)(clock() - start) / (CLOCKS_PER_SEC)) << endl;
}

vector<int> GeneticAlgorithm::insert(vector<int> &permutation, int first, int second) {
    if (second < first) {
        int tmp = permutation[first];
        for (int i = first; i > second; i--) {
            permutation[i] = permutation[i - 1];
        }
        permutation[second] = tmp;
        return permutation;
    } else {
        int tmp = permutation[first];
        for (int i = first; i < second; i++) {
            permutation[i] = permutation[i + 1];
        }
        permutation[second] = tmp;
        return permutation;
    }
}