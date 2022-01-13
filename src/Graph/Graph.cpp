//
// Created by Andrzej on 22.11.2021.
//

#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include "Graph.hpp"

using namespace std;

Graph::Graph() {
    this->number_of_vertices = 0;
}

Graph::~Graph() = default;

void Graph::loadData(const string &name_file) {
    this->matrix.clear();
    string val;
    fstream file_in;


    file_in.open(name_file + ".txt");
    if (file_in.is_open()) {
        file_in >> this->number_of_vertices;
        this->matrix.resize(this->number_of_vertices);

        if (file_in.fail())
            cout << "File error - READ SIZE" << endl;
        else
            for (int i = 0; i < this->number_of_vertices; i++) {
                for (int j = 0; j < this->number_of_vertices; j++) {
                    file_in >> val;
                    if (file_in.fail()) {
                        cout << "File error - READ DATA" << endl;
                        break;
                    } else
                        this->matrix[i].push_back(stoi(val));
                }
            }
        file_in.close();
    } else
        cout << "File error - OPEN" << endl;
}

void Graph::display() const {
    for (auto &m: this->matrix) {
        for (auto &mm: m) {
            cout << mm;
        }
        cout << endl;
    }
}


void Graph::generateGraph(int size) {

    if(size <= 1){
        cout<<"To little nodes were given." << endl;
        return;
    }
    this->matrix.clear();
    this->number_of_vertices = size;
    this->matrix.resize(size);
    for(int i = 0; i < size;i++){
        this->matrix[i].resize(size);
    }

    srand (time(nullptr));
    for (int i = 0; i < this->number_of_vertices; i ++){
        for (int j = 0; j < this->number_of_vertices; j++){
            if(i == j)
                this->matrix[i][j] = 0;
            else
                this->matrix[i][j] = rand()%100+1;
        }
    }
}