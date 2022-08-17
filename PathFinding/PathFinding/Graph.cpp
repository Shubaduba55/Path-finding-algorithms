#include "Graph.h"

Graph::Graph() : is_resetted(true) {}

Graph::Graph(vector<Node> nodes, vector<Edge> edges): m_nodes(nodes), m_edges(edges), is_resetted(true){}

int Graph::get_rows(){ return rows;}

int Graph::get_columns(){ return clms;}



bool Graph::does_node_exist(float x, float y)
{
	for (Node& elem : m_nodes) {
		if (*elem.get_point() == Point(x, y)) return true;
	}
	return false;
}
bool Graph::does_node_exist(int id)
{
	for (Node& elem : m_nodes) {
		if (elem.get_id() == id) return true;
	}
	return false;
}
bool Graph::does_edge_exist(int id1, int id2)
{
	int extr_id1, extr_id2;
	for (Edge& edge : m_edges) {
		edge.extract_nodes(extr_id1, extr_id2);
		if ((id1 == extr_id1 && id2 == extr_id2) || (id2 == extr_id1 && id1 == extr_id2)) return true;
	}
	return false;
}

void Graph::add_node(float x, float y, bool is_walkable)
{	
	//Messages: node already exists
	if (does_node_exist(x, y)) { cout << "Node (" << x << ";" << y << ") already exists"; return; }

	Node tmp(x, y, is_walkable);
	m_nodes.push_back(tmp);
	return;
}

void Graph::add_edge(int id1, int id2, float weight)
{
	//Message: edge already exists
	if (!does_node_exist(id1) || !does_node_exist(id2)) throw "Trying to connect not existing nodes";
	if (does_edge_exist(id1, id2)) { cout << "Edge (" << id1 << ";" << id2 << ") already exists"; return; }
	if (weight <= 0 || weight > 100) { throw "Edge weight should be [0; 100]"; }
	if (id1 == id2) throw "Edge should connect different nodes, not the same:" + to_string(id1);

	Node& node1 = this->get_node(id1);
	Node& node2 = this->get_node(id2);
	m_edges.push_back(Edge(id1, id2, weight));
	node1.add_connection();
	node2.add_connection();
	return;
}

void Graph::delete_node(int id)
{
	//Message: node doesn't exist
	if(!does_node_exist(id)) { cout << "Node " << id << " doesn't exist"; return; }


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
	//Error: edge doesn't belong
	if (!does_edge_exist(id1, id2)) { cout << "Edge (" << id1 << ";" << id2 << ") doesn't exist"; return; }


	vector<Edge> new_edges;
	int tmp_id1, tmp_id2;
	
	for (Edge& edge : m_edges) {
		edge.extract_nodes(tmp_id1, tmp_id2);
		if ((id1 != tmp_id1 || id2 != tmp_id2) && (id1 != tmp_id2 || id2 != tmp_id1)) new_edges.push_back(edge);
		else { 
			get_node(id1).delete_connection();
			get_node(id2).delete_connection();
		}
	}
	m_edges = new_edges;
	
	return;
}

void Graph::delete_graph()
{
	m_nodes.clear(); m_edges.clear(); Node::reset_counter();
	rows = 0;
	clms = 0;
	is_resetted = true;
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
	//Error: there is no such node
	if(!does_node_exist(id)) throw "Trying to get not existing node via id:" + to_string(id);

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

	//Error: tmp == nullptr then end is not reachable
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
		if (m_edges[i].get_pair_weight(current_node_id, neighbour_id, edge_wght)) {
			neighbours_to_visit--;
			Node& neighbour = this->get_node(neighbour_id);
			if (neighbour.is_node_walkable()) {
				float new_reach_val = node->get_reach_val() + edge_wght;
				
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

void Graph::find_path(int start, int end, int algorithm_option)
{
	this->reset_graph();
	//int num;
	//cout << "\n\nChoose the way to find path: \n1.Dijkstra;\n2.A*(euclidian heuristic)\n3.A*(manhattan heuristic)\n\nEnter number(1, 2 or 3): ";
	//std::cin >> num;

	function<float(Node*, Node*)> heuristic;
	switch (algorithm_option)
	{
	case 1:	heuristic = [](Node*, Node*)->float { return 0; }; break;
	case 2:	heuristic = euclid_heuristic; break;
	case 3:	heuristic = manhattan_heuristic; break;
	}


	auto strt = high_resolution_clock::now();

	//Get adresses of start and end node
	Node& start_n = this->get_node(start);
	Node& end_n = this->get_node(end);

	//Error: chosen nodes are not walkable
	if (!(start_n.is_node_walkable()) || !(end_n.is_node_walkable())) {cout << "\n\nError: start and end nodes are not walkable"; return;}

	//Set reach value of the starting node from INF to 0
	start_n.set_reach_val(0);
	Node* min_rv_node; // Node with minimal reach value
	

	//Run loop until end node is visited
	int count_of_visited_nodes = 0; bool is_en_reachable = true;
	while (!end_n.is_node_visited() && is_en_reachable) {

		min_rv_node = this->find_min_node(); // find unvisited node with the least reach value

		if (min_rv_node) {
			count_of_visited_nodes++;
			run_through_neighbours(min_rv_node, &end_n, heuristic); // go through this node's neighbour nodes
			min_rv_node->set_status_visited(); //change status of this node from unvisited to visited
		}
		else {
			is_en_reachable = false; cout << "End node is not reachable\n";
		}
	}

	is_resetted = false;
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - strt);
	//Error: there is no path between start and end

	//Print all nodes from end to start
	Node* tmp = &end_n;
	while (tmp != nullptr && is_en_reachable) {
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

void Graph::create_graph(int rows, int clms, int obstacles)
{
	this->delete_graph();

	this->clms = clms; this->rows = rows;

	srand(time(NULL));
	int chance_to_be_obstacle;
	if (obstacles) chance_to_be_obstacle = (rows * clms) / obstacles + (rows * clms) % obstacles;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < clms; j++)
		{
			/*
			bool is_walkable = true;
			if (obstacles) {
				is_walkable = rand() % chance_to_be_obstacle;
				if(!is_walkable) obstacles--;
			}
			*/
			this->add_node(j, i);
		}
	}

	for (int i = 0; obstacles; i++)
	{
		bool is_walkable = rand() % chance_to_be_obstacle;
		if (!is_walkable && m_nodes[i].is_node_walkable()) {
			obstacles--;
			m_nodes[i].change_walkable_status();
		}
		if (i + 1 == m_nodes.size()) i = 0;
	}


	int size = rows * clms;
	for (int i = 0; i < size; i++) {

		/*
		int dir1 = i + 1;//right
		int dir2 = i + (size + 1);//up-right
		int dir3 = i + size;//up
		int dir4 = i - (size - 1);//down-right

		*/

		int dir1 = i + 1;//right
		int dir2 = i + (clms + 1);//down-right
		int dir3 = i + clms;//down
		int dir4 = i - (clms - 1);//up-right


		if ((i + 1) % clms != 0 && dir1 < size) this->add_edge(i, dir1, 1);//OK
		//if ((i + 1) % clms != 0 && dir2 < size) this->add_edge(i, dir2, 1.4142);//OK
		if (dir3 >= 0 && dir3 < size) this->add_edge(i, dir3, 1);//OK
		//if (dir4 >= 0 && (i + 1) % clms != 0)this->add_edge(i, dir4, 1.4142);//OK
	}
}

void Graph::visualize()
{
	int size = m_nodes.size();
	int clms = this->clms;
	string field = "", row = "", sign;

	for (int i = 0; i < size; i++)
	{
		if (m_nodes[i].is_node_path()) sign = '*';
		else if (m_nodes[i].is_node_visited()) sign = '$';
		else {
			if (m_nodes[i].is_node_walkable()) sign = ' ';
			else sign = '#';
		};

		row = row + sign + ' ';
		if ((i + 1) % clms == 0) {
			field =  field + row + '\n';
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
	if (!m_nodes.size()) throw "writing empty graph";
	save_graph.write((char*)&rows, sizeof(rows));
	save_graph.write((char*)&clms, sizeof(clms));

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
	read_graph.read((char*)&rows, sizeof(rows));
	read_graph.read((char*)&clms, sizeof(clms));

	//Error:invalid data
	size_t size_n;
	read_graph.read((char*)&size_n, sizeof(size_n)); if (size_n != (rows * clms) || size_n == 0) throw "invalid data";

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

