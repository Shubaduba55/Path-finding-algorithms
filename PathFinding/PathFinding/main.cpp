#include "Graph.h"
#include "BinaryFile.h"
int Node::counter = 0;



int main() {
	Graph graph;
	//graph.create_graph(4);
	BinaryFile file("FindingErrors.txt");
	//file.write_graph(graph);
	graph = file.read_graph();
	
	//graph.find_path(0, 15);
	graph.visualize();
	
	return 0; 
}





/*
	Graph graph;
	graph.add_node(0, 0);
	graph.add_node(1, 1);
	graph.add_node(2, 2);
	graph.add_node(3, 3);
	graph.add_node(4, 4);
	graph.add_node(5, 5);
	graph.add_node(6, 6);
	graph.add_node(6, 6);
	

	graph.add_edge(0, 1, 5);
	graph.add_edge(0, 2, 4);
	graph.add_edge(1, 2, 4);
	graph.add_edge(1, 4, 7);
	graph.add_edge(2, 4, 3);
	graph.add_edge(2, 5, 2);
	graph.add_edge(3, 4, 1);
	graph.add_edge(3, 6, 1);
	graph.add_edge(4, 5, 10);
	graph.add_edge(5, 6, 8);
*/

/*Graph graph;
	graph.add_node(0, 0);//0
	graph.add_node(1, 1);//1
	graph.add_node(1, -1);//2
	graph.add_node(3, 3);//3
	graph.add_node(3, -3);//4
	graph.add_node(5, 0);//5
	graph.add_node(-100, -1);//6
	graph.add_node(-100, 1);//7


	graph.add_edge(0, 1, 7);
	graph.add_edge(0, 2, 2);
	graph.add_edge(1, 2, 3);
	graph.add_edge(1, 3, 4);
	graph.add_edge(2, 4, 8);
	graph.add_edge(3, 4, 1);
	graph.add_edge(3, 5, 10);
	graph.add_edge(4, 5, 3);

	graph.add_edge(6, 0, 1);
	graph.add_edge(7, 0, 1);
	graph.add_edge(6, 7, 1);

	graph.print();
	graph.find_path(0, 5);;*/