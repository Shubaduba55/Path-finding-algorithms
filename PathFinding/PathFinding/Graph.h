#pragma once
#include "Node.h"
#include "Edge.h"
#include <vector>
#include <functional>
#include <chrono>
using std::vector;
using std::cout;
using std::function;
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

	Node* find_min_node();
	//void run_through_neighbours(Node*, Node* = nullptr, function<float(Node*, Node*)> = [](Node* = nullptr, Node* = nullptr)->float {return 0; });
	void run_through_neighbours(Node*, Node*, function<float(Node*, Node*)>);
public:
	Graph() {};
	Graph(vector<Node>, vector<Edge>);
	~Graph() {};


	void add_node(float, float);
	void add_edge(int, int, int);
	void delete_node(int);
	void print();
	Node& get_node(int);

	void find_path(int, int);
	void create_graph(int);
};

float euclid_heuristic(Node*, Node*);


float manhattan_heuristic(Node*, Node*);