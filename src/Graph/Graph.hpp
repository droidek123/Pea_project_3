//
// Created by Andrzej on 22.11.2021.
//

#pragma once

#ifndef PEA_PROJECT_2_GRAPH_HPP
#define PEA_PROJECT_2_GRAPH_HPP

#include <string>
#include <vector>

using namespace std;

class Graph {
public:
    vector<vector<int>> matrix{};
    int number_of_vertices;

    Graph();

    virtual ~Graph();

    void loadData(const string &name_file);

    void display() const;

    void generateGraph(int size);

};


#endif //PEA_PROJECT_2_GRAPH_HPP
