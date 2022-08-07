#include "BinaryFile.h"

void BinaryFile::write_graph(Graph& graph)
{
    file.open(name, ios::binary | ios::out);
    //Error: file is not found

    graph.write(file);
    file.close();
}

Graph BinaryFile::read_graph()
{
    file.open(name, ios::binary | ios::in);

    //Error: file is not found, file is empty

    Graph graph;
    graph.read(file);
    file.close();
    return graph;
}
