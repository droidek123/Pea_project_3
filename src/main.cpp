#include <iostream>
#include "Graph/Graph.hpp"
#include "GeneticAlgoritm/GeneticAlgoritm.hpp"

using namespace std;

void displayMainMenu();

int main() {
    bool quit = false;
    char option;
    int stop_time = 60;
    int population = 1000;
    float crossesRate = 0.8;
    float mutationsRate = 0.01;
    Crossing crossing = PMX;
    Mutation mutation = SWAP;
    string name;
    Graph graph;
    do {
        displayMainMenu();
        cin >> option;

        cout << endl;
        switch (option) {
            case '0':
                quit = true;
                break;
            case '1':
                cout << "Plese enter name of the file (without .txt!): ";
                cin >> name;
                graph.loadData(name);
                break;
            case '2':
                do {
                    cout << "Enter the time [s]> ";
                    cin >> stop_time;
                } while (!stop_time);
                break;
            case '3': {
                cout << "Enter the population";
                cin >> population;
                break;
            }
            case '4':
                cout << "Eneter the crossRate" << endl;
                cin >> crossesRate;
                break;
            case '5':
                cout << "Eneter the mutationsRate" << endl;
                cin >> mutationsRate;
                break;
            case '6': {
                int temp;
                std::cout << "Please choose a crossing operation:\n"
                             "0 - PMX\n"
                             "1 - OX\n";
                cin >> temp;
                if (temp == 0) {
                    crossing = PMX;
                    cout << "Crossing operation: PMX" << endl;
                } else {
                    crossing = OX;
                    cout << "Crossing operation: OX" << endl;
                }
                break;
            }
            case '7': {
                int temp;
                std::cout << "Please choose a mutation operation:\n"
                             "0 - SWAP\n"
                             "1 - INSERT\n";
                cin >> temp;
                if (temp == 0) {
                    mutation = SWAP;
                    cout << "Crossing operation: SWAP" << endl;
                } else {
                    mutation = INSERT;
                    cout << "Crossing operation: INSERT" << endl;
                }
                break;
            }
            case '8': {
                GeneticAlgorithm geneticAlgorithm(graph, stop_time, population, crossesRate, mutationsRate);
                geneticAlgorithm.solve(crossing, mutation);
                break;
            }
            default:
                cout << "Wrong option chosen.\n";
        }

    } while (!quit);
    return 0;
}

void displayMainMenu() {
    cout << "================== MENU ==================" << endl;
    cout << "\t   1. Load from file" << endl;
    cout << "\t   2. Enter the stop criterion" << endl;
    cout << "\t   3. Enter population" << endl;
    cout << "\t   4. Enter cross rate" << endl;
    cout << "\t   5. Enter mutation rate" << endl;
    cout << "\t   6. Choose a crossing operation" << endl;
    cout << "\t   7. Choose a mutation operation" << endl;
    cout << "\t   8. Solve" << endl;
    cout << "\t   0. Exit" << endl << endl;
    cout << "Chosen option:  ";
}