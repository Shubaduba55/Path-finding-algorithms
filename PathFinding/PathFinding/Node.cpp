#include "Node.h"

Node::Node() :
	m_id(counter++), connections(0), is_walkable(true),
	is_visited(false), reach_value(INF), heuristic_value(0),
	previous(nullptr) {}

Node::Node(float x, float y, bool is_walkable): 
	m_id(counter++), m_coordinate(Point(x, y)), 
	connections(0), is_walkable(is_walkable), is_visited(false),
	reach_value(INF), heuristic_value(0), previous(nullptr) {}

int Node::get_id(){	return m_id; }
 
void Node::add_connection(){ connections++; }

Point* Node::get_point(){ return &m_coordinate; }

int Node::get_num_of_neighbours(){ return connections; }

bool Node::is_node_walkable(){ return is_walkable; }

void Node::change_walkable_status() { is_walkable = !is_walkable; }

bool Node::is_node_visited(){ return is_visited; }

void Node::set_status_visited(){ is_visited = true;}

float Node::get_reach_val(){ return reach_value; }

void Node::set_reach_val(float new_reach_val){	reach_value = new_reach_val;}

float Node::get_heuristic_val() { return heuristic_value; }

void Node::set_heuristic_val(float new_h_val) { heuristic_value = new_h_val; }

Node* Node::get_prev_node(){ return previous; }

void Node::set_prev_node(Node* node){ previous = node;}

bool Node::is_node_path(){ return is_path; }

void Node::set_status_path(){ is_path = true; }

ostream& operator<<(ostream& out, const Node& node)
{
	out << "ID: " << node.m_id << " " << node.m_coordinate << " number of neighbours: " << node.connections;
	out << " walkable status:" << node.is_walkable;
	return out;
}
