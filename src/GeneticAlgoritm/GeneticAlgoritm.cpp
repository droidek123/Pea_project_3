#include "GeneticAlgoritm.hpp"
#include <random>
#include <ctime>

using namespace std;

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
    } while (begin == end || begin > end);

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
    vector<int> desc1(size), desc2(size);
    vector<int>::iterator itr1, itr2;
    int begin, end;

    do {
        begin = rand() % size;
        end = rand() % size;
    } while (begin == end || begin > end);

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

vector<vector<int>> GeneticAlgorithm::makePopulation() {
    vector<int> permutation(size);
    vector<vector<int>> population(populationSize);

    for (int i = 0; i < size; i++) permutation[i] = i;

    for (int i = 0; i < populationSize; i++) {
        shuffle(permutation.begin(), permutation.end(),mt19937(random_device()()));
        population[i] = permutation;
    }
    return population;
}

void GeneticAlgorithm::selection(vector<int> fitness, vector<vector<int>> &population){
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

int GeneticAlgorithm::apply(bool crossing) {
    vector<vector<int>> population(populationSize), nextPopulation(populationSize);
    vector<int> fitness(populationSize), permutation(size);
    int index, result, p1, p2;
    clock_t start;

    population = makePopulation();

    start = std::clock();

    // Kolejne iteracje algorytmu
    while (((std::clock() - start) / (CLOCKS_PER_SEC)) < stop) {
        // Ocena jakości osobników
        for (size_t idx = 0; auto &itr : population) {
            fitness[idx] = calculatePath(itr);
            idx++;
        }

        // Tworzenie nowej populacji na drodze selekcji
        selection(fitness,population);

        // Krzyżowanie osobników
        for (int j = 0; j < (int) (crossRate * (float) populationSize); j += 2) {
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

//            swap(population.at(j)[p1], population.at(j)[p2]);
            insert(population[j], p1, p2);
        }
    }

    result = *(min_element(fitness.begin(), fitness.end()));
    return result;
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