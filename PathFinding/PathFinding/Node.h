#pragma once
#include "Point.h"
#include <exception>
#define INF 1000000000


using std::ostream;

class Node {
private:
	int static counter;

	int m_id;
	Point m_coordinate; 
	int connections;

	bool is_walkable;

	bool is_visited;
	float reach_value;
	float heuristic_value;
	Node* previous;

	bool is_path = false;

public:
	Node();
	Node(float, float, bool = true);

	void add_connection();
	void delete_connection();

	int get_id();
	Point* get_point();
	int get_num_of_neighbours();

	bool is_node_walkable();
	void change_walkable_status();

	bool is_node_visited();
	void set_status_visited();

	float get_reach_val();
	void set_reach_val(float);

	float get_heuristic_val();
	void set_heuristic_val(float);

	Node* get_prev_node();
	void set_prev_node(Node*);

	bool is_node_path();
	void set_status_path();

	void reset();
	static void reset_counter();
	friend ostream& operator << (ostream&, const Node&);
};