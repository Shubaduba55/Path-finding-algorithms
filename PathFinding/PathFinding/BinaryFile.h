#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "Graph.h"
using std::string;
using std::fstream;
using std::ios;


class BinaryFile
{

private:
    string name;
    fstream file;
public:
    BinaryFile(string name) : name(name) {};
    
    void write_graph(Graph&);
    Graph read_graph();
};