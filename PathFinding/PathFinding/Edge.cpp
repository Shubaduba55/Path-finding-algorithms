#include "Edge.h"


Edge::Edge() : m_node1(-1), m_node2(-1), m_weight(0) {}
Edge::Edge(int node1, int node2, int wght) : 
    m_node1(node1), 
    m_node2(node2), 
    m_weight(wght) {}


int Edge::get_weight(){return m_weight;}

void Edge::extract_nodes(int& node1, int& node2){
    node1 = m_node1;
    node2 = m_node2;
    return;
}
void Edge::set_weight(int wght){ m_weight = wght;}

void Edge::set_nodes(int node1, int node2) {
    m_node1 = node1;
    m_node2 = node2;
}


bool Edge::get_pair_weight(int node_id1, int& node_id2, float& wght)
{
    /*
    Function that checks, if the given node_id1 belongs to our edge. If so, returns true, id of the second node and it's weight.
    In other case return false.
    */
    if (m_node1 == node_id1) {
        node_id2 = m_node2;
        wght = m_weight;
        return true;
    }
    else if (m_node2 == node_id1) {
        node_id2 = m_node1;
        wght = m_weight;
        return true;
    }
    else {
        return false;
    }
    
}

ostream& operator<<(ostream& out, const Edge& edge)
{
    out << "(" << edge.m_node1 << "<->" << edge.m_node2 << ") - w: " << edge.m_weight;
    return out;
}
