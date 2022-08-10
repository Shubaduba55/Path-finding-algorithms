#include "Graph.h"

Graph::Graph() : is_resetted(true) {}

Graph::Graph(vector<Node> nodes, vector<Edge> edges): m_nodes(nodes), m_edges(edges), is_resetted(true){}

void Graph::add_node(float x, float y, bool is_walkable)
{	
	for (Node& elem : m_nodes) {
		if (*elem.get_point() == Point(x, y)) return;
	}
	Node tmp(x, y, is_walkable);
	m_nodes.push_back(tmp);
	return;
}

void Graph::add_edge(int id1, int id2, float weight)
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
	//Error: node doesn't exist
	
	vector<Node> new_nodes;
	int tmp_id; float wght;
	bool node_belongs = false;
	vector<int> neighbours_id;

	Node& node_to_delete = get_node(id);
	int connections = node_to_delete.get_num_of_neighbours();
	

	for (int i = 0; connections; i++) {
		if (m_edges[i].get_pair_weight(id, tmp_id, wght)) {
			neighbours_id.push_back(tmp_id);
			connections--;
		}
	}
	for (int neighbour_id : neighbours_id) delete_edge(id, neighbour_id);
	for (Node& node : m_nodes) {if (node.get_id() != id) new_nodes.push_back(node);}
	m_nodes = new_nodes;
	return;
}

void Graph::delete_edge(int id1, int id2)
{
	vector<Edge> new_edges;
	int tmp_id1, tmp_id2;
	bool edge_belongs = false;
	for (Edge& edge : m_edges) {
		edge.extract_nodes(tmp_id1, tmp_id2);
		if ((id1 != tmp_id1 || id2 != tmp_id2) && (id1 != tmp_id2 || id2 != tmp_id1)) new_edges.push_back(edge);
		else { 
			edge_belongs = true; 
			get_node(id1).delete_connection();
			get_node(id2).delete_connection();
		}
	}
	m_edges = new_edges;
	//Error: edge doesn't belong
	return;
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
		if (!elem.is_node_visited() && elem.is_node_walkable() && value > (elem.get_reach_val() + elem.get_heuristic_val())) 
		{
			tmp = &elem;
			value = elem.get_reach_val() + elem.get_heuristic_val();
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
			if (neighbour.is_node_walkable()) {
				float new_reach_val = node->get_reach_val() + edge_wght;
				//float new_heuristic_val = heuristic(&neighbour, end_n);

				if (neighbour.get_reach_val() > new_reach_val) {
					neighbour.set_reach_val(new_reach_val);
					neighbour.set_heuristic_val(heuristic(&neighbour, end_n));
					neighbour.set_prev_node(node); 
				}
			}
		};
	}
	return;
}

void Graph::find_path(int start, int end)
{
	this->reset_graph();
	int num;
	cout << "\n\nChoose the way to find path: \n1.Dijkstra;\n2.A*(euclidian heuristic)\n3.A*(manhattan heuristic)\n\nEnter number(1, 2 or 3): ";
	std::cin >> num;

	function<float(Node*, Node*)> heuristic;
	switch (num)
	{
	case 1:	heuristic = [](Node*, Node*)->float { return 0; }; break;
	case 2:	heuristic = euclid_heuristic; break;
	case 3:	heuristic = manhattan_heuristic; break;
	}


	auto strt = high_resolution_clock::now();

	//Get adresses of start and end node
	Node& start_n = this->get_node(start);
	Node& end_n = this->get_node(end);
	if (!(start_n.is_node_walkable()) || !(end_n.is_node_walkable())) {cout << "\n\nError: start and end nodes are not walkable"; return;}

	//Set reach value of the starting node from INF to 0
	start_n.set_reach_val(0);
	Node* min_rv_node; // Node with minimal reach value
	

	//Run loop until end node is visited
	int count_of_visited_nodes = 0;
	while (!end_n.is_node_visited()) {
		count_of_visited_nodes++;
		min_rv_node = this->find_min_node(); // find unvisited node with the least reach value
		run_through_neighbours(min_rv_node, &end_n, heuristic); // go through this node's neighbour nodes
		min_rv_node->set_status_visited(); //change status of this node from unvisited to visited
	}

	is_resetted = false;
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - strt);


	//Print all nodes from end to start
	Node* tmp = &end_n;
	while (tmp != nullptr) {
		tmp->set_status_path();
		cout << *tmp << std::endl;
		tmp = tmp->get_prev_node();
	}
	cout << "\nNumber of visited nodes: " << count_of_visited_nodes << "\t Time:" << duration.count() << " ms" << 
		"\t Path value: "<< end_n.get_reach_val() << std::endl;

	return;
}

void Graph::reset_graph()
{
	if (is_resetted) return;
	else { 
		for (Node& node : m_nodes) node.reset(); 
		is_resetted = true;
	}
	
	return;
}

void Graph::create_graph(int size)
{
	srand(time(NULL));
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			bool is_walkable = rand() % 3;
			this->add_node(j, i, is_walkable);
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

void Graph::visualize()
{
	int size = m_nodes.size();
	int sqr = sqrt(m_nodes.size());
	string field = "", row = "", sign;

	for (int i = 0; i < size; i++)
	{	
		if (m_nodes[i].is_node_path()) sign = '*';
		else if (m_nodes[i].is_node_visited()) sign = '$';
		else {
			if (m_nodes[i].is_node_walkable()) sign = '+';
			else sign = '#';
		};

		row = row + sign + ' ';
		if ((i + 1) % sqr == 0) { 
			field = row + '\n' + field;
			row = ""; 
		}
		
	}

	ofstream visualize("Visualize.txt", ios::trunc);

	visualize << field;
	visualize.close();
	return;
}

void Graph::write(ostream& save_graph)
{

	//Error: empty m_nodes
	size_t size_n = m_nodes.size();
	save_graph.write((char*)&size_n, sizeof(size_n));
	save_graph.write((char*)&m_nodes[0], size_n * sizeof(Node));
	
	//Error: empty m_edges
	size_t size_e = m_edges.size();
	save_graph.write((char*)&size_e, sizeof(size_e));
	save_graph.write((char*)&m_edges[0], size_e * sizeof(Edge));
	
	
	return;
}

void Graph::read(istream& read_graph)
{
	size_t size_n;
	read_graph.read((char*)&size_n, sizeof(size_n));

	vector<Node> nodes(size_n);
	read_graph.read((char*)&nodes[0], size_n * sizeof(Node));
	m_nodes = nodes;
	
	size_t size_e;
	read_graph.read((char*)&size_e, sizeof(size_e));

	vector<Edge> edges(size_e);
	read_graph.read((char*)&edges[0], size_e * sizeof(Edge));
	m_edges = edges;

	return;
}



float euclid_heuristic(Node* node1, Node* node2) {
	Point* a = node1->get_point();
	Point* b = node2->get_point();
	return calc_distance(a, b);
}

float manhattan_heuristic(Node* node1, Node* node2) {
	Point* a = node1->get_point();
	Point* b = node2->get_point();
	return fabs(a->get_x() - b->get_x()) + fabs(a->get_y() - b->get_y());
}

