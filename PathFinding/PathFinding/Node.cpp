#include "Node.h"

Node::Node() :
	m_id(counter++), connections(0), 
	is_visited(false), reach_value(INF), 
	previous(nullptr) {}

Node::Node(float x, float y) : 
	m_id(counter++), m_coordinate(Point(x, y)), 
	connections(0), is_visited(false), 
	reach_value(INF), previous(nullptr) {}

int Node::get_id(){	return m_id; }
 
void Node::add_connection()
{
	connections += 1;
}

Point* Node::get_point()
{
	return &m_coordinate;
}

int Node::get_num_of_neighbours()
{
	return connections;
}

bool Node::get_status()
{
	return is_visited;
}

void Node::status_visited()
{
	is_visited = true;
}

float Node::get_reach_val()
{
	return reach_value;
}

void Node::set_reach_val(float new_reach_val)
{
	reach_value = new_reach_val;
}

Node* Node::get_prev_node()
{
	return previous;
}

void Node::set_prev_node(Node* node)
{
	previous = node;
}

ostream& operator<<(ostream& out, const Node& node)
{
	out << "ID: " << node.m_id << " " << node.m_coordinate << " number of neighbours: " << node.connections;
	return out;
}
