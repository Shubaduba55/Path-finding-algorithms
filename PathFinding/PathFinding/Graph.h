#pragma once
#include "Node.h"
#include "Edge.h"
#include <vector>
#include <functional>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>



using std::vector;
using std::cout;
using std::function;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::ostream;
using std::istream;
using std::string;
using std::to_string;

using namespace std::chrono;


/*
	Візуалізація
	Додавання та видалення вузлів +
	Алгоритми пошуку шляху
*/
class Graph {
private:
	vector<Node> m_nodes;
	vector<Edge> m_edges;
	bool is_resetted;
	
	int rows, clms;

	Node* find_min_node();
	void run_through_neighbours(Node*, Node*, function<float(Node*, Node*)>);
public:
	Graph();
	Graph(vector<Node>, vector<Edge>);
	~Graph() {};

	int get_rows();
	int get_columns();


	bool does_node_exist(float, float);
	bool does_node_exist(int);
	bool does_edge_exist(int, int);

	void add_node(float, float, bool = true);
	void add_edge(int, int, float);
	void delete_node(int);
	void delete_edge(int, int);
	void delete_graph_info();

	void print();
	Node& get_node(int);

	string find_path(int, int, int);
	void reset_graph();
	void create_graph(int, int, int = 0);
	void visualize();

	void write(ostream&);
	void read(istream&);
};

float euclid_heuristic(Node*, Node*);

float manhattan_heuristic(Node*, Node*);