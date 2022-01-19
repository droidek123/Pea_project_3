#include "GeneticAlgoritm.hpp"
#include <random>
#include <ctime>
#include <iostream>

using namespace std;

/**
 * Konstuktor
 * @param graph graf na którym bedziemy wykonywac agorytm
 * @param stop czas trwania algorytmu
 * @param population populacja
 * @param crossRate wspolczynnik krzyzowania
 * @param mutationRate  wspolczynnik mutacji
 */
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


/**
 * metoda odpowiedzilana za oblicznie dlugosci sciezki
 * @param path sciezka
 * @return zwraca dlugosc sciezki
 */
int GeneticAlgorithm::calculatePath(vector<int> &path) {
    int result = 0;

    for (int i = 0; i < size - 1; i++)
        result += matrix[path[i]][path[i + 1]];

    result += matrix[path[size - 1]][path[0]];

    return result;
}


/**
 * metoda wykonujaca krzyzowanie typu pmx
 * @param parent1 pierwszy rodzic
 * @param parent2 drugi rodzic
 */
void GeneticAlgorithm::partiallyCrossover(vector<int> &parent1, vector<int> &parent2) const {
    vector<int> desc1(size, -1), desc2(size, -1);
    vector<int> map1(size, -1), map2(size, -1);

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


/**
 * metoda wykonujaca krzyzowanie typu ox
 * @param parent1 pierwszy rodzic
 * @param parent2 drugi rodzic
 */
void GeneticAlgorithm::orderCrossover(vector<int> &parent1, vector<int> &parent2) const {
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


/**
 * medoda generujaca populacje poczatkowa
 * @return zwraca populacje poczatkowa
 */
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

/**
 * metoda wykonujaca selekcje turniejowa
 * @param fitness wektor z warosciami siciezek dla kazdej permutacji w populacji
 * @param population  populacja
 */
void GeneticAlgorithm::selection(vector<int> fitness, vector<vector<int>> &population) const {
    int tournamentSize = 5, index;
    vector<vector<int>> nextPopulation(populationSize);
    vector<int> permutation(size);
    for (int j = 0; j < populationSize; j++) {
        int result = INT32_MAX;

        for (int k = 0; k < tournamentSize; k++) {
            index = rand() % populationSize;
            if (fitness[index] < result) {
                result = fitness[index];
                permutation = population[index];
            }
        }
        nextPopulation[j] = permutation;
    }

    population.swap(nextPopulation);
}

void GeneticAlgorithm::solve(Crossing crossing, Mutation mutation) {
    vector<vector<int>> population(populationSize), nextPopulation(populationSize);
    vector<int> fitness(populationSize), permutation(size);
    int result, first, second, temp;
    clock_t start;

    population = makePopulation();

    start = std::clock();

    while ((std::clock() - start) / (CLOCKS_PER_SEC) < stop) {
        // Ocena populacji
        for (size_t idx = 0; auto &itr: population) {
            temp = calculatePath(itr);
            fitness[idx] = temp;
            if (temp < best) {
                best = temp;
            }
            idx++;
        }

        // Stworzenie nowej populacji za pomca selkcji turniejowej
        selection(fitness, population);

        // Krzyżowanie osobników
        for (int j = 0; j < (int) (crossRate * (float) populationSize); j++) {
            first = rand() % populationSize;
            do {
                second = rand() % populationSize;
            } while (first == second);
            if (crossing)
                orderCrossover(population.at(first), population.at(second));
            else
                partiallyCrossover(population.at(first), population.at(second));
        }

        // Mutacja osobników
        for (int j = 0; j < (int) (mutationRate * (float) populationSize); j++) {
            first = rand() % size;
            do {
                second = rand() % size;
            } while (first == second);

            if (mutation) insert(population[j], first, second);
            else swap(population.at(j)[first], population.at(j)[second]);
        }
    }

    result = *(min_element(fitness.begin(), fitness.end()));
    if (result < best)
        best = result;
    cout << best << endl;

}


/**
 * metoda wykonujaca mutacje typu insert
 * @param permutation permutacja z populacji
 * @param first ta wartosc bedzie wstawiana w miejsce wskazywane przez second
 * @param second ta wartosc wskazuje gdzie bedzie wstawiene first
 * @return zwraca zmutowana permtacje
 */
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