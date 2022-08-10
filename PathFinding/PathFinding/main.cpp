#include "Graph.h"
#include "BinaryFile.h"
#include <iterator>
int Node::counter = 0;
using std::iterator;


int main() {
	Graph graph;
	BinaryFile file("2500.txt");


	graph = file.read_graph();
	//graph.delete_node(8);
	
	//graph.delete_node(2449);
	graph.find_path(0, 2499);

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

/*	graph.add_node(5, 15);//0
	graph.add_node(3, 12);//1
	graph.add_node(8, 11);//2

	graph.add_node(3, 9);//3
	graph.add_node(4, 7);//4
	graph.add_node(8, 8);//5

	graph.add_node(9, 15);//6
	graph.add_node(15, 2);//7
	graph.add_node(14, 15);//8

	graph.add_node(17, 13);//9
	graph.add_node(15, 10);//10
	graph.add_node(19, 10);//11
	graph.add_node(17, 7);//12

	graph.add_edge(0, 1, 7);
	graph.add_edge(0, 2, 2);
	graph.add_edge(0, 8, 3);

	graph.add_edge(1, 2, 3);
	graph.add_edge(1, 3, 4);

	graph.add_edge(2, 3, 4);
	graph.add_edge(2, 5, 1);

	graph.add_edge(3, 4, 5);

	graph.add_edge(4, 5, 3);
	
	graph.add_edge(5, 6, 2);
	
	graph.add_edge(6, 7, 2);

	graph.add_edge(7, 12, 5);

	graph.add_edge(8, 9, 2);

	graph.add_edge(9, 10, 4);
	graph.add_edge(9, 11, 4);

	graph.add_edge(10, 11, 6);
	graph.add_edge(10, 12, 4);

	graph.add_edge(11, 12, 4);*/