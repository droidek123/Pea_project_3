#include <iostream>
#include "Graph/Graph.hpp"
#include "GeneticAlgoritm/GeneticAlgoritm.hpp"

using namespace std;

void displayMainMenu();

int main() {
    bool quit = false;
    char option;
    int time = 0;
    int population = 0;
    float crossesFactor = 0.0;
    float mutationsFactor = 0.0;
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
                    cin >> time;
                } while (!time);
                break;
            case '3': {
                cout << "Enter the population";
                cin >> population;
                break;
            }
            case '4':
//                if (diversification == 0)cout << "Diversification on" << endl;
//                else cout << "Diversification off" << endl;
//                diversification = !diversification;
                cout << "Eneter the crossFactor" << endl;
                cin >> crossesFactor;
                break;
            case '5':
//                std::cout << "Please choose a neighborhood:\n"
//                             "0 - swap\n"
//                             "1 - insert\n";
//                cin >> neighborhood;
                cout << "Eneter the mutationsFactor" << endl;
                cin >> mutationsFactor;
                break;
            case '6':{
                cout << "zobaczymy moze to zrobie" << endl;
                break;
            }
            case '7': {
                GeneticAlgorithm geneticAlgorithm(graph,time,population,crossesFactor,mutationsFactor);
                std::cout<< geneticAlgorithm.apply(true)<<std::endl;
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
    cout << "\t   3. Population" << endl;
    cout << "\t   4. Krzyzowanie" << endl;
    cout << "\t   5. Mutacje" << endl;
    cout << "\t   6. Zmiana metody krzyzowania" << endl;
    cout << "\t   0. Exit" << endl << endl;
    cout << "Chosen option:  ";
}