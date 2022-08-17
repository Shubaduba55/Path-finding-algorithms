#include "BinaryFile.h"

void BinaryFile::write_graph(Graph graph)
{
    file.open(name, ios::binary | ios::out);
    //Error: file is not found
    if (!file) throw "file couldn't open file";


    graph.reset_graph();
    graph.write(file);
    file.close();
}

Graph BinaryFile::read_graph()
{
    file.open(name, ios::binary | ios::in);
    //Error: file is not found, file is empty
    if (!file) throw "file couldn't open file";


    Graph graph;
    //try {
    graph.read(file);
    //}
    //catch (const char* text) { throw text; }
    file.close();
    return graph;
}
