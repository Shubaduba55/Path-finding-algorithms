#include "Graph.h"

Graph::Graph(vector<Node> nodes, vector<Edge> edges): m_nodes(nodes), m_edges(edges){}

void Graph::add_node(float x, float y)
{	
	for (Node& elem : m_nodes) {
		if (*elem.get_point() == Point(x, y)) return;
	}
	Node tmp(x, y);
	m_nodes.push_back(tmp);
	return;
}

void Graph::add_edge(int id1, int id2, int weight)
{
	int extr_id1, extr_id2;
	for (Edge& edge : m_edges) {
		edge.extract_nodes(extr_id1, extr_id2);
		if ((id1 == extr_id1 && id2 == extr_id2) || (id2 == extr_id1 && id1 == extr_id2)) return;
	}
	Node& node1 = this->get_node(id1);
	Node& node2 = this->get_node(id2);
	m_edges.push_back(Edge(id1, id2, weight));
	node1.add_connection();
	node2.add_connection();
	return;
}

void Graph::delete_node(int id)
{
	if (id > m_nodes.size()) {

		return;
	}
	else {
		vector<Node> new_nodes;
		for (int i = 0; i < m_nodes.size(); i++)
		{	
			if(m_nodes[i].get_id()!=id)	new_nodes.push_back(m_nodes[i]);
		}
		m_nodes = new_nodes;
	}
}

void Graph::print()
{
	cout << "Nodes: \n";
	for (const Node& elem : m_nodes) {
		std::cout << elem << std::endl;
	}

	cout << "\n\nEdges: \n";

	for (const Edge& elem : m_edges) {
		cout << elem << std::endl;
	}
	return;
}

Node& Graph::get_node(int id)
{
	for (int i = 0; i < m_nodes.size(); i++) {
		if (m_nodes[i].get_id() == id) return m_nodes[i];
	}
}

Node* Graph::find_min_node()
{
	float value = INF;
	Node* tmp = nullptr;
	for (Node& elem : m_nodes) {
		if (!elem.get_status() && value > elem.get_reach_val()) {
			tmp = &elem;
			value = elem.get_reach_val();
		}
	}

	//Error: tmp == nullptr then it is not reachable
	return tmp;
}

void Graph::run_through_neighbours(Node* node, Node* end_n, function<float(Node*, Node*)> heuristic)
{
	/*Function that goes through all nodes connected to given node*/

	int neighbours_to_visit = node->get_num_of_neighbours(); //Numbers of neighbours of the node we need to visit
	int current_node_id = node->get_id();
	int neighbour_id;
	float edge_wght;
	for (int i = 0; neighbours_to_visit; i++)
	{
		/*m_edges[i].extract_nodes(node_id1, node_id2);
		float edge_weight = m_edges[i].get_weight();


		if (current_node_id == node_id1) {
			neighbours_to_visit--;
			Node& neighbour = this->get_node(node_id2);
			float new_reach_val = node->get_reach_val() + edge_weight;
			if (neighbour.get_reach_val() > new_reach_val) {
				neighbour.set_reach_val(new_reach_val);
			}
		}*/
		if (m_edges[i].get_pair_weight(current_node_id, neighbour_id, edge_wght)) {
			neighbours_to_visit--;

			Node& neighbour = this->get_node(neighbour_id);
			float new_reach_val = node->get_reach_val() + edge_wght + heuristic(&neighbour, end_n);

			if (neighbour.get_reach_val() > new_reach_val) {
				neighbour.set_reach_val(new_reach_val);
				neighbour.set_prev_node(node);
			}
		};
	}
	return;
}

void Graph::find_path(int start, int end)
{
	int num;
	cout << "\n\nChoose the way to find path: \n1.Dijkstra;\n2.A*(euclidian heuristic)\n3.A*(manhattan heuristic)\n\nEnter number(1, 2 or 3): ";
	std::cin >> num;

	function<float(Node*, Node*)> heuristic;
	switch (num) {
	case 1:
		heuristic = [](Node*, Node*)->float { return 0; };
		break;
	case 2:
		heuristic = euclid_heuristic;
		break;
	case 3:
		heuristic = manhattan_heuristic;
		break;
	}


	auto strt = high_resolution_clock::now();

	//Get adresses of start and end node
	Node& start_n = this->get_node(start);
	Node& end_n = this->get_node(end);

	//Set reach value of the starting node from INF to 0
	start_n.set_reach_val(0);
	Node* min_rv_node; // Node with minimal reach value
	vector<Node> visited;

	//Run loop until end node is visited
	int count_of_visited_nodes = 0;
	while (!end_n.get_status()) {
		count_of_visited_nodes++;
		min_rv_node = this->find_min_node(); // find unvisited node with the least reach value
		run_through_neighbours(min_rv_node, &end_n, heuristic); // go through this node's neighbour nodes
		min_rv_node->status_visited(); //change status of this node from unvisited to visited
		//visited.push_back(*min_rv_node);
	}

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - strt);


	//Print all nodes from end to start
	Node* tmp = &end_n;
	while (tmp != nullptr) {
		cout << *tmp << std::endl;
		tmp = tmp->get_prev_node();
	}
	cout << "\nNumber of visited nodes: " << count_of_visited_nodes << "\t Time:" << duration.count() << " ms" << std::endl;

	/*for (Node& elem : visited) {
		cout << elem << std::endl;
	}*/
	return;
}

void Graph::create_graph(int size)
{
	
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			this->add_node(i, j);
		}
	}
	int size_square = size * size;
	for (int i = 0; i < size_square; i++) {

		int dir1 = i + 1;//right
		int dir2 = i + (size + 1);//up-right
		int dir3 = i + size;//up
		int dir4 = i - (size - 1);//down-right

		if((i + 1)%size != 0 && dir1 < size_square) this->add_edge(i, dir1, 1);//OK
		if ((i+1)%size != 0 && dir2 < size_square) this->add_edge(i, dir2, 1.4142);//OK
		if (dir3 >= 0 && dir3 < size_square) this->add_edge(i, dir3, 1);//OK
		if (dir4 >= 0 && (i + 1) % size != 0)this->add_edge(i, dir4, 1.4142);//OK
	}
}

float euclid_heuristic(Node* node1, Node* node2) {
	Point* a = node1->get_point();
	Point* b = node2->get_point();
	return calc_distance(a, b);
}

float manhattan_heuristic(Node* node1, Node* node2) {
	Point* a = node1->get_point();
	Point* b = node2->get_point();
	return fabs(a->get_x() - b->get_x()) + fabs(a->get_y() + b->get_y());
}

/*if (num == 1) run_through_neighbours(min_rv_node);
		else if (num == 2) run_through_neighbours(min_rv_node, &end_n, euclid_heuristic);
		else if (num == 3) run_through_neighbours(min_rv_node, &end_n, manhattan_heuristic);*/