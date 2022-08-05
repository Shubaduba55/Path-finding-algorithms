#pragma once
#include "Node.h"
#include <iostream>
/*
	������������ +
	Get Set +
*/
using std::ostream;

class Edge {
private:
	int m_node1;
	int m_node2;
	int m_weight;
public:
	Edge();
	Edge(int, int, int);


	int get_weight();
	void extract_nodes(int&, int&);
	void set_weight(int);
	void set_nodes(int, int);
	bool get_pair_weight(int, int&, float&);
	friend ostream& operator << (ostream&, const Edge&);
};
