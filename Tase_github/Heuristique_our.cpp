#include"Heuristique_our.h"
#include"Configuration.h"
#include <chrono>

Heuristique_our::Heuristique_our(Data& data) {
	_fname = Configuration::_fname;
	_iteration_max = Configuration::_iteration_max;
	_no_improvement_iteration_max = Configuration::_no_improvement_iteration_max;
	_p_max = Configuration::_p_max;
	_data = data;
	_duration_time = Configuration::_duration_time;

	_best_solution = std::make_shared<Solution>();
}


bool Heuristique_our::initialisation() {
	initialise_random_generator();
	return true;
}

void Heuristique_our::print() {
	std::cout << "parameters:" << std::endl;
	std::cout << "iteration max = " << _iteration_max << "; no improvement iteration max = " << _no_improvement_iteration_max;
	std::cout << "; p_max = " << _p_max << "; node relevance = " << Configuration::_node_relevance_min
		<< "; edge relevance = " << Configuration::_edge_relevance_min << "; state relevance = " << Configuration::_state_relevance_min << std::endl;
}

Data& Heuristique_our::get_data() {
	return _data;
}

void Heuristique_our::initialise_random_generator() {
	_distribution_init = std::uniform_real_distribution<double>(0, 1);
	std::random_device dev;
	_rng.seed(dev());
	std::cout << "seed = " << _rng << "\n";
}



bool Heuristique_our::node_existe(Graph& graph, std::string label, int position) {
	auto num_vertices = boost::num_vertices(graph);
	for (int i = 0; i < num_vertices; i++) {
		if ((graph[i].label == label) && (graph[i].position == position)) {
			return true;
		}
	}
	return false;
}

int Heuristique_our::node_find(Graph& graph, std::string label, int position) {
	auto num_vertices = boost::num_vertices(graph);
	for (int i = 0; i < num_vertices; i++) {
		if ((graph[i].label == label) && (graph[i].position == position)) {
			return i;
		}
	}
	return -1;
}



void Heuristique_our::go() {
	//attention: if node_score contains only non-zero values and no node is successfully inserted, the algo will not regenerate a new initial solution.
	//Since our initial (current) solution is not empty, the matrice node_score contains always some zero values (0 when a node already existes)
	if (initialisation() == false) {
		std::cout << "initialisation failed...\n";
		return;
	}
	int counter = 0;
	auto start = std::chrono::high_resolution_clock::now();
	std::shared_ptr<Solution> initial_solution = random_solution();
	std::shared_ptr<Solution> current_solution = std::make_shared<Solution>();
	while (initial_solution == NULL) {
		initial_solution = random_solution();
	}
	_best_solution = initial_solution;
	current_solution = initial_solution;
	for (int i = 0; i < _iteration_max; i++) {
		std::cout << "iteration = " << i << "; initial relevance = " << initial_solution->get_relevance_graph()
			<< "; current relevance = " << current_solution->get_relevance_graph()
			<< "; best relevance = " << _best_solution->get_relevance_graph() << "\n";
		if (counter >= _no_improvement_iteration_max) {
			break;
		}
		counter++;
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
		if ((float)duration.count() / 1000 > _duration_time) {
			break;
		}
		std::vector<std::tuple<std::string, int, int>> node_score;
		cal_node_score(current_solution, node_score);
		std::sort(node_score.begin(), node_score.end(), sortByVal);
		//std::cout << node_score << std::endl;
		for (int j = 0; j < (int)node_score.size(); j++) {
			std::string label = std::get<0>(node_score[j]);
			int position = std::get<1>(node_score[j]);
			int value = std::get<2>(node_score[j]);
			if (value <= 0) {
				initial_solution = random_solution();
				while (initial_solution == NULL) {
					initial_solution = random_solution();
				}
				current_solution = initial_solution;
				if (current_solution->get_relevance_graph() > _best_solution->get_relevance_graph()) {
					_best_solution = current_solution;
					counter = 0;
				}
				break;
			}
			Graph graph(current_solution->get_graph());
			insert_node_graph(graph, label, position, _data.get_state_set());
			std::shared_ptr<Solution> neighbor_solution = std::make_shared<Solution>(graph);
			neighbor_solution->evaluation(_data);
			if (neighbor_solution->feasibility() == true) {
				if (neighbor_solution->get_relevance_graph() > _best_solution->get_relevance_graph()) {
					_best_solution = neighbor_solution;
					counter = 0;
				}
				if (neighbor_solution->get_relevance_graph() > current_solution->get_relevance_graph()) {
					current_solution = neighbor_solution;
					break;
				}
			}
			else {
				std::shared_ptr<Solution> repaired_solution = std::make_shared<Solution>();
				if (repair_solution_progressive_nonfixed_size(neighbor_solution, repaired_solution) == true)
				{
					if (repaired_solution->get_relevance_graph() > _best_solution->get_relevance_graph()) {
						_best_solution = repaired_solution;
						counter = 0;
					}
					if (repaired_solution->get_relevance_graph() > current_solution->get_relevance_graph()) {
						current_solution = repaired_solution;
						break;
					}
				}
			}
		}
		
	}
	_best_solution->print();
}



void Heuristique_our::complete_graph(Graph& graph, std::set<std::string>& state_set) {
	int num_vertices = (int)boost::num_vertices(graph);
	for (int i = 0; i < num_vertices; i++) {
		for (int j = 0; j < num_vertices; j++) {
			if (graph[i].position < graph[j].position) {
				auto edge = boost::edge(i, j, graph);
				if (edge.second == true) {
					for (auto iter = state_set.begin(); iter != state_set.end(); iter++) {
						graph[edge.first].state.insert(*iter);
					}
				}
				else {
					boost::add_edge(i, j, { state_set }, graph);
				}
			}
		}
	}
}

void Heuristique_our::insert_node_graph(Graph& graph, std::string label, int position, std::set<std::string>& state_set) {
	int vertice = boost::add_vertex({ label,position }, graph);
	int num_vertices = (int)boost::num_vertices(graph);
	for (int i = 0; i < num_vertices; i++) {
		if (i == vertice) {
			continue;
		}
		if (graph[i].position < graph[vertice].position) {
			boost::add_edge(i, vertice, { state_set }, graph);
		}
		if (graph[i].position > graph[vertice].position) {
			boost::add_edge(vertice, i, { state_set }, graph);
		}
	}
}


float Heuristique_our::best_solution() {
	return _best_solution->get_relevance_graph();
}

std::string Heuristique_our::get_instance_name() {
	return _fname;
}

bool Heuristique_our::repair_solution_progressive_nonfixed_size(std::shared_ptr<Solution> old_solution, std::shared_ptr<Solution> new_solution) {
	std::set<int> nodes;
	std::set<std::pair<int, int>> edges;
	std::map<std::pair<int, int>, std::set<std::string>> edge_states;
	for (auto iter = old_solution->get_node_relevance_map().begin(); iter != old_solution->get_node_relevance_map().end(); iter++) {
		if ((iter->second) > 0) {
			nodes.insert(iter->first);
		}
	}
	for (auto iter = old_solution->get_edge_relevance_map().begin(); iter != old_solution->get_edge_relevance_map().end(); iter++) {
		if ((iter->second) > 0) {
			edges.insert(iter->first);
		}
	}
	for (auto iter = old_solution->get_state_relevance_map().begin(); iter != old_solution->get_state_relevance_map().end(); iter++) {
		if ((iter->second) > 0) {
			int node_i, node_j;
			std::string state;
			std::tie(node_i, node_j, state) = iter->first;
			edge_states[{node_i, node_j}].insert(state);
		}
	}
	std::map<int, int> node_old_to_new_map;
	std::map<int, int> node_new_to_old_map;
	for (auto node : nodes) {
		int new_index = (int)boost::add_vertex({ old_solution->get_graph()[node].label,old_solution->get_graph()[node].position }, new_solution->get_graph());
		node_old_to_new_map[node] = new_index;
		node_new_to_old_map[new_index] = node;
	}
	for (auto edge : edges) {
		int node_i, node_j;
		std::tie(node_i, node_j) = edge;
		if (edge_states.find(edge) != edge_states.end() && nodes.find(node_i) != nodes.end() && nodes.find(node_j) != nodes.end()) {
			int new_node_i = node_old_to_new_map[node_i];
			int new_node_j = node_old_to_new_map[node_j];
			boost::add_edge(new_node_i, new_node_j, { edge_states[edge] }, new_solution->get_graph());
		}
	}
	new_solution->evaluation(_data);
	while (new_solution->feasibility() == false) {
		int source, target;
		std::string state;
		bool flag;
		std::tie(source, target, state, flag) = new_solution->get_edge_state_invalide_min();
		if (flag == true) {
			auto edge = boost::edge(source, target, new_solution->get_graph());
			new_solution->get_graph()[edge.first].state.erase(state);
			if (new_solution->get_graph()[edge.first].state.size() == 0) {
				boost::remove_edge(source, target, new_solution->get_graph());
			}
			std::shared_ptr<Solution> repaired_solution = std::make_shared<Solution>(new_solution->get_graph());
			repaired_solution->evaluation(_data);
			new_solution->copy(*repaired_solution);
			continue;
		}
		std::tie(source, target, flag) = new_solution->get_edge_invalide_min();
		if (flag == true) {
			auto edge = boost::edge(source, target, new_solution->get_graph());
			boost::remove_edge(source, target, new_solution->get_graph());
			std::shared_ptr<Solution> repaired_solution = std::make_shared<Solution>(new_solution->get_graph());
			repaired_solution->evaluation(_data);
			new_solution->copy(*repaired_solution);
			continue;
		}
		int node;
		std::tie(node, flag) = new_solution->get_node_invalide_min();
		if (flag == true) {
			boost::clear_vertex(node, new_solution->get_graph());
			boost::remove_vertex(node, new_solution->get_graph());
			std::shared_ptr<Solution> repaired_solution = std::make_shared<Solution>(new_solution->get_graph());
			repaired_solution->evaluation(_data);
			new_solution->copy(*repaired_solution);
			continue;
		}
		return false;
	}
	return true;
}

std::shared_ptr<Solution> Heuristique_our::random_solution() {
	Graph graph;
	for (int i = 0; i < _p_max; i++) {
		for (auto label : _data.get_event_set()) {
			if (_distribution_init(_rng) > 0.5) {
				boost::add_vertex({ label,i }, graph);
			}
		}
	}
	std::shared_ptr<Solution> solution = std::make_shared<Solution>(graph);
	complete_graph(solution->get_graph(), _data.get_state_set());
	solution->evaluation(_data);
	if (solution->feasibility() == true) {
		return solution;
	}
	else {
		std::shared_ptr<Solution> repaired_solution = std::make_shared<Solution>();
		if (repair_solution_progressive_nonfixed_size(solution, repaired_solution) == true)
		{
			return repaired_solution;
		}
		else {
			return NULL;
		}
	}
}

void Heuristique_our::cal_node_score(std::shared_ptr<Solution> solution, std::vector<std::tuple<std::string, int, int>>& node_score) {
	auto event_log = _data.get_eventlog();
	auto graph = solution->get_graph();
	for (auto label : _data.get_event_set()) {
		for (int position = 0; position < _p_max; position++) {
			if (node_existe(graph, label, position) == true) {
				node_score.push_back(std::make_tuple(label, position, 0));
			}
			else {
				int score = 0;
				for (int i = 0; i < (int)event_log.size(); i++) {
					auto trace = event_log[i];
					auto nodes_replayed = solution->get_theta_list()[i];
					auto events_replayed = solution->get_sigma_list()[i];
					int start = -1;
					int end = (int)trace.size();
					bool flag = false;
					for (int j = 0; j < nodes_replayed.size();j++) {
						auto node = nodes_replayed[j];
						if (graph[node].position < position) {
							start = events_replayed[j];
						}
						if (graph[node].position > position) {
							if (end > events_replayed[j]) {
								end = events_replayed[j];
							}
						}
						if (graph[node].position == position) {
							flag = true;
						}
					}
					if (flag == false) {
						for (int j = start + 1; j < end; j++) {
							if (trace[j]["label"] == label) {
								score = score + 1;
								break;
							}
						}
					}
				}
				node_score.push_back(std::make_tuple(label, position, score));
			}
		}
	}
}

void Heuristique_our::test() {
	if (initialisation() == false) {
		std::cout << "initialisation failed...\n";
		return;
	}

	Graph graph;
	std::vector<std::pair<int, std::string>> nodes{ {1,"RCP"}, {2,"RCP"}, {3,"TTT"}, {4,"RCP"}, {6,"RCP"}, {0,"OD"}, {10,"lastc"}, {7,"RCP"}, {1,"Chir"}, {8,"RCP"}, {5,"TTT"}, {4,"Chir"},{9,"RCP"}};
	std::vector<std::tuple<int, int, std::set<std::string>>> arcs{ {0,1,{"np"}},{0,11,{"np"}},{1,2,{"np"}},{1,11,{"np"}},{2,3,{"np"}},{3,10,{"np"}},{4,7,{"np"}},{5,0,{"np"}},{5,8,{"np"}},{7,9,{"np","pro"}},{8,2,{"np"}},{9,12,{"pro"}},
		{10,4,{"np"} },{10,12,{"np"}}, {11,10,{"np"}}, {12,6,{"np","pro"}}};
	for (auto node : nodes) {
		//attributes
		int i = node.first;
		std::string labels = node.second;
		boost::add_vertex({ labels, i }, graph);
	}
	for (auto arc : arcs) {
		int source = std::get<0>(arc);
		int destination = std::get<1>(arc);
		auto states = std::get<2>(arc);
		boost::add_edge(source, destination, { states }, graph);
	}
	std::shared_ptr<Solution> solution = std::make_shared<Solution>(graph);
	solution->evaluation(_data);
	solution->print();
}