#pragma once
#include "Point.h"
#include <vector>
#define INF 1000000000

using std::vector;
using std::ostream;
/*
	Розрахунок координат на основі зв'язку
	Додавання зв'язку
*/
class Node {
private:
	int static counter;

	int m_id;
	Point m_coordinate;
	int connections;

	bool is_visited;
	float reach_value;
	Node* previous;

public:
	Node();
	Node(float, float);

	void add_connection();

	int get_id();
	Point* get_point();
	int get_num_of_neighbours();

	bool get_status();
	void status_visited();

	float get_reach_val();
	void set_reach_val(float);

	Node* get_prev_node();
	void set_prev_node(Node*);

	friend ostream& operator << (ostream&, const Node&);
};