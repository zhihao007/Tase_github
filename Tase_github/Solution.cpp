#include"Solution.h"
#include"Configuration.h"

Solution::Solution() {
	_relevance_graph = 0;
	_relevance_graph_penality = 0;

	_node_relevance_min = Configuration::_node_relevance_min;
	_edge_relevance_min = Configuration::_edge_relevance_min;
	_state_relevance_min = Configuration::_state_relevance_min;
	_lamda = Configuration::_lamda;
	_alpha = Configuration::_alpha;
	_node_begin_position = Configuration::_node_begin_position;
	_node_end_position = Configuration::_node_end_position;
}

Solution::Solution(Graph &graph) {
	boost::copy_graph(graph, _graph);
	_relevance_graph = 0;
	_relevance_graph_penality = 0;

	_node_relevance_min = Configuration::_node_relevance_min;
	_edge_relevance_min = Configuration::_edge_relevance_min;
	_state_relevance_min = Configuration::_state_relevance_min;
	_lamda = Configuration::_lamda;
	_alpha = Configuration::_alpha;
	_node_begin_position = Configuration::_node_begin_position;
	_node_end_position = Configuration::_node_end_position;
}

Graph& Solution::get_graph() {
	return _graph;
}

std::vector<float>& Solution::get_g_prime_list() {
	return _g_prime_list;
}

std::vector<std::vector<int>>& Solution::get_sigma_list() {
	return _sigma_list;
}

std::vector<std::vector<int>>& Solution::get_theta_list() {
	return _theta_list;
}

std::vector<std::map<std::pair<int, int>, float>>& Solution::get_A_list() {
	return _A_list;
}

std::vector<Trace_state_table_type>& Solution::get_B_list() {
	return _B_list;
}

std::map<int, float>& Solution::get_node_relevance_map() {
	return _node_relevance_map;
}

std::map<std::pair<int, int>, float>& Solution::get_edge_relevance_map() {
	return _edge_relevance_map;
}

std::map<std::tuple<int, int, std::string>, float>& Solution::get_state_relevance_map() {
	return _state_relevance_map;
}

float Solution::get_relevance_graph() {
	return _relevance_graph;
}

void Solution::print() {
	std::cout << "graph: " << std::endl;
	auto num_vertices = boost::num_vertices(_graph);
	for (int i = 0; i < num_vertices; i++) {
		std::cout << "index:" << i << ";";
		std::cout << "label: " << _graph[i].label << ";";
		std::cout << "position: " << _graph[i].position << std::endl;
	}
	auto epair = boost::edges(_graph);
	for (auto i = epair.first; i != epair.second; i++) {
		std::cout << "edge: " << boost::source(*i, _graph) << "-" << boost::target(*i, _graph) << ";";
		std::cout << "edge state: ";
		for (auto j = _graph[*i].state.begin(); j != _graph[*i].state.end(); j++) {
			std::cout << *j << ",";
		}
		std::cout << std::endl;
	}

	/*
	std::cout << "g_prime_list: " <<" ";
	for (auto k = _g_prime_list.begin(); k != _g_prime_list.end(); k++) {
		std::cout << *k << ",";
	}
	std::cout << std::endl;
	*/
	/*
	std::cout << "sigma_list: " << " ";
	for (auto k = _sigma_list.begin(); k != _sigma_list.end(); k++) {
		auto _sigma = *k;
		for (auto i = _sigma.begin(); i != _sigma.end(); i++) {
			std::cout << *i << " ";
		}
		std::cout << std::endl;
	}
	

	std::cout << "theta: " << " ";
	for (auto k = _theta_list.begin(); k != _theta_list.end(); k++) {
		auto _theta = *k;
		for (auto i = _theta.begin(); i != _theta.end(); i++) {
			std::cout << *i << " ";
		}
		std::cout << std::endl;
	}
	*/
	/*
	std::cout << "A_list: " << std::endl;
	for (auto k = _A_list.begin(); k != _A_list.end(); k++) {
		auto _A = *k;
		for (auto i = _A.begin(); i != _A.end(); i++) {
			std::cout << "(" << i->first.first << "," << i->first.second << ")=" << i->second << ',';
		}
		std::cout << std::endl;
	}
	

	std::cout << "B_list: " << std::endl;
	for (auto k = _B_list.begin(); k != _B_list.end(); k++) {
		auto _B = *k;
		for (auto i = _B.begin(); i != _B.end(); i++) {
			std::cout << "(" << i->first.first << "," << i->first.second << ")=";
			auto states = i->second;
			for (auto j = states.begin(); j != states.end(); j++) {
				std::cout << *j << ",";
			}
		}
		std::cout << std::endl;
	}
	*/
	std::cout << "node relevance list:" << std::endl;
	for (auto i = _node_relevance_map.begin(); i != _node_relevance_map.end(); i++) {
		std::cout << "node: " << i->first << ", relevance: " << i->second << std::endl;
	}
	
	std::cout << "edge relevance list:" << std::endl;
	for (auto i = _edge_relevance_map.begin(); i != _edge_relevance_map.end(); i++) {
		std::cout << "(" << i->first.first << "," << i->first.second << ")=" << i->second << std::endl;
	}

	std::cout << "state relevance list:" << std::endl;
	for (auto i = _state_relevance_map.begin(); i != _state_relevance_map.end(); i++) {
		int node_i, node_j;
		std::string state;
		std::tie(node_i, node_j, state) = i->first;
		std::cout << "(" << node_i << "," << node_j << "," << state << ")=" << i->second << std::endl;
	}
	
	std::cout << "relevance of model:" << _relevance_graph << std::endl;
}

bool Solution::is_replayed(int event_index, int node_index, Trace_type& trace, int trace_begin_index, int trace_end_index, int graph_begin_index, int graph_end_index) {
	if ((event_index != trace_begin_index) && (event_index != trace_end_index) && (node_index != graph_begin_index) && (node_index != graph_end_index)) {
		return _graph[node_index].label == trace[event_index]["label"];
	}
	else {
		if ((event_index == trace_begin_index) && (node_index == graph_begin_index)) {
			return true;
		}
		else if ((event_index == trace_end_index) && (node_index == graph_end_index)) {
			return true;
		}
		else {
			return false;
		}
	}

}

int Solution::get_position(int node_index, int graph_begin_index, int graph_end_index) {
	if (node_index == graph_begin_index) {
		return _node_begin_position;
	}
	else if (node_index == graph_end_index) {
		return _node_end_position;
	}
	else {
		return _graph[node_index].position;
	}
}

std::set<std::string> Solution::get_states_trace(int i, int j, Trace_type& trace) {
	std::set<std::string> states;
	for (int k = i; k < j; k++) {
		states.insert(trace[k]["state"]);
	}
	return states;
}

float Solution::score_node(int event_index, int node_index, Trace_type& trace) {
	float score = 0;
	if (_graph[node_index].label == trace[event_index]["label"]) {
		score = 1;
	}
	return score;
	
}

float Solution::score_edge(int event_i_index, int event_j_index, int node_i_index, int node_j_index, Trace_state_table_type& trace_state_table) {
	float term1;
	float term2;
	if (boost::edge(node_i_index, node_j_index, _graph).second == true) {
		term1 = 1.0;
		auto& vec1 = trace_state_table[{event_i_index, event_j_index}];
		std::set<std::string>& set2 = _graph[boost::edge(node_i_index, node_j_index, _graph).first].state;
		int count = 0;
		for (auto state1 : vec1) {
			if (set2.count(state1)) {
				count++;
			}
		}
		term2 = (float)count / vec1.size();
	}
	else {
		term1 = 0;
		term2 = 0;
	}

	return (1 - _lamda) * term1 + _lamda * term2;
	
}

void Solution::replayability(Trace_type& trace, Trace_state_table_type& trace_state_table) {
	int graph_size = (int)boost::num_vertices(_graph);
	int trace_size = (int)trace.size();
	int trace_begin_index = -1;
	int trace_end_index = trace_size;
	int graph_begin_index = -1;
	int graph_end_index = graph_size;
	std::vector<int> sigma;
	std::vector<int> theta;
	std::map<std::pair<int, int>, float> A;
	Trace_state_table_type B;
	std::map<std::pair<int, int>, float> g;
	g[{trace_begin_index, graph_begin_index}] = 0;
	std::map<std::pair<int, int>, std::pair<int, int>> s;
	std::vector<int> x_prime;
	x_prime.push_back(trace_begin_index);
	std::map<int, std::vector<int>> y_prime;
	y_prime[trace_begin_index] = { graph_begin_index };
	float negative_infinity_value = -999;
	int negative_infinity_index = -999;
	//core
	for (int i = 0; i <= trace_end_index; i++) {
		bool flag = false;
		std::vector<int> y_prime_prime;
		for (int j = 0; j <= graph_end_index; j++) {
			if (is_replayed(i, j, trace, trace_begin_index, trace_end_index, graph_begin_index, graph_end_index) == false) {
				g[{i, j}] = negative_infinity_value;
				continue;
			}
			float temp = negative_infinity_value;
			float value_max = negative_infinity_value;
			int u_max = negative_infinity_index;
			int v_max = negative_infinity_index;
			for (auto u : x_prime) {
				for (auto v : y_prime[u]) {
					if (get_position(j, graph_begin_index, graph_end_index) > get_position(v, graph_begin_index, graph_end_index)) {
						if ((i == trace_end_index) && (j == graph_end_index)) {
							temp = g[{u, v}];
						}
						else if ((u == trace_begin_index) && (v == graph_begin_index)) {
							temp = g[{u, v}] + score_node(i, j, trace);
						}
						else {
							temp = g[{u, v}] + score_node(i, j, trace) + _alpha * score_edge(u, i, v, j, trace_state_table);
						}
						if (value_max < temp) {
							value_max = temp;
							u_max = u;
							v_max = v;
						}
						if (value_max == temp) { //if two solutions equivalentes, save the one at the lowest position
							if (get_position(v_max, graph_begin_index, graph_end_index) > get_position(v, graph_begin_index, graph_end_index)) {
								value_max = temp;
								u_max = u;
								v_max = v;
							}
						}
					}
				}
			}
			g[{i, j}] = value_max;
			s[{i, j}] = std::make_pair(u_max, v_max);
			if (g[{i, j}] != negative_infinity_value) {
				y_prime_prime.push_back(j);
				flag = true;
			}
		}
		if (flag == true) {
			x_prime.push_back(i);
			y_prime[i] = y_prime_prime;
		}
	}
	//retrieve
	int p, q;
	std::tie(p, q) = s[{trace_end_index, graph_end_index}];
	float g_prime = g[{trace_end_index, graph_end_index}];
	while ((p != trace_begin_index) && (q != graph_begin_index)) {
		sigma.insert(sigma.begin(), p);
		theta.insert(theta.begin(), q);
		std::tie(p, q) = s[{p, q}];
	}
	for (int i = 0; i < (int)theta.size() - 1; i++) {
		A[{theta[i], theta[i + 1]}] = score_edge(sigma[i], sigma[i + 1], theta[i], theta[i + 1], trace_state_table);
		std::vector<std::string> vec1 = trace_state_table[{sigma[i], sigma[i + 1]}];
		std::set<std::string> set2;
		if (boost::edge(theta[i], theta[i + 1], _graph).second == true) {
			set2 = _graph[boost::edge(theta[i], theta[i + 1], _graph).first].state;
		}
		std::vector<std::string> intersect;
		for (auto state1 : vec1) {
			if (set2.count(state1)) {
				intersect.push_back(state1);
			}
		}
		B[{theta[i], theta[i + 1]}] = intersect;
	}

	_g_prime_list.push_back(g_prime);
	_sigma_list.push_back(sigma);
	_theta_list.push_back(theta);
	_A_list.push_back(A);
	_B_list.push_back(B);

	return;
}


void Solution::replay_all_traces(Data& data) {
	for (int i = 0; i < data.get_eventlog().size(); i++) {
		replayability(data.get_eventlog()[i], data.get_trace_state_table_list()[i]);
	}
}

void Solution::relevance_node() {
	for (int i = 0; i < boost::num_vertices(_graph); i++) {
		_node_relevance_map[i] = 0;
	}
	for (auto theta:_theta_list) {
		for (auto node:theta) {
			_node_relevance_map[node] = _node_relevance_map[node] + 1;
		}
	}
}

void Solution::relevance_edge() {
	auto epair = boost::edges(_graph);
	for (auto iter = epair.first; iter != epair.second; iter++) {
		int node_i = (int)boost::source(*iter, _graph);
		int node_j = (int)boost::target(*iter, _graph);
		_edge_relevance_map[{node_i, node_j}] = 0;
	}
	for (auto A : _A_list) {
		for (auto iter = A.begin(); iter != A.end(); iter++) {
			if (_edge_relevance_map.find(iter->first) != _edge_relevance_map.end()) {
				_edge_relevance_map[iter->first] = _edge_relevance_map[iter->first] + A[iter->first];
			}
		}
	}
}

void Solution::relevance_state() {
	auto epair = boost::edges(_graph);
	for (auto i = epair.first; i != epair.second; i++) {
		int node_i = (int)boost::source(*i, _graph);
		int node_j = (int)boost::target(*i, _graph);
		for (auto j = _graph[*i].state.begin(); j != _graph[*i].state.end(); j++) {
			auto key = std::make_tuple(node_i, node_j, *j);
			_state_relevance_map[key] = 0;
		}
	}
	for (int i = 0; i < _B_list.size();i++) {
		auto B = _B_list[i];
		auto A = _A_list[i];
		for (auto iter = B.begin(); iter != B.end(); iter++) {
			int node_i = iter->first.first;
			int node_j = iter->first.second;
			auto states = iter->second;
			if (states.size() == 0) {
				continue;
			}
			float average_relevance = A[iter->first] / states.size();
			for (auto state : states) {
				auto key = std::make_tuple(node_i, node_j, state);
				_state_relevance_map[key] = _state_relevance_map[key] + average_relevance;
			}
		}
	}
}

void Solution::relevance_graph() {
	float relevance_node = 0;
	for (auto iter = _node_relevance_map.begin(); iter != _node_relevance_map.end(); iter++) {
		relevance_node = relevance_node + iter->second;
	}
	float relevance_edge = 0;
	for (auto iter = _edge_relevance_map.begin(); iter != _edge_relevance_map.end(); iter++) {
		relevance_edge = relevance_edge + iter->second;
	}
	_relevance_graph =  relevance_node + _alpha * relevance_edge;
}

void Solution::evaluation(Data& data) {
	replay_all_traces(data);
	relevance_node();
	relevance_edge();
	relevance_state();
	relevance_graph();
}

bool Solution::feasibility() {
	for (auto iter = _state_relevance_map.begin(); iter != _state_relevance_map.end(); iter++) {
		if ((iter->second) < _state_relevance_min) {
			return false;
		}
	}
	for (auto iter = _edge_relevance_map.begin(); iter != _edge_relevance_map.end(); iter++) {
		if ((iter->second) < _edge_relevance_min) {
			return false;
		}
	}
	for (auto iter = _node_relevance_map.begin(); iter != _node_relevance_map.end(); iter++) {
		if ((iter->second) < _node_relevance_min) {
			return false;
		}
	}
	if (boost::num_vertices(_graph) == 0) {
		return false;
	}
	return true;
}


void Solution::clean() {
	_graph.clear();
	_g_prime_list.clear();
	_sigma_list.clear();
	_theta_list.clear();
	_A_list.clear();
	_B_list.clear();

	_node_relevance_map.clear();
	_edge_relevance_map.clear();
	_state_relevance_map.clear();
	_relevance_graph = 0;
	_relevance_graph_penality = 0;
}

void Solution::copy(Solution& solution) {
	clean();
	boost::copy_graph(solution.get_graph(), _graph);
	_g_prime_list = solution.get_g_prime_list();
	_sigma_list = solution.get_sigma_list();
	_theta_list = solution.get_theta_list();
	_A_list = solution.get_A_list();
	_B_list = solution.get_B_list();
	_node_relevance_map = solution.get_node_relevance_map();
	_edge_relevance_map = solution.get_edge_relevance_map();
	_state_relevance_map = solution.get_state_relevance_map();
	_relevance_graph = solution.get_relevance_graph();
	_relevance_graph_penality = solution.get_relevance_graph_penality();
}

float Solution::get_node_relevance_min() {
	return _node_relevance_min;
}

float Solution::get_edge_relevance_min() {
	return _edge_relevance_min;
}

float Solution::get_state_relevance_min() {
	return _state_relevance_min;
}

std::pair<int, bool> Solution::get_node_invalide_min() {
	bool flag = false;
	std::pair<int, float> worst_node = { -1, std::numeric_limits<float>::max() };
	for (auto element : _node_relevance_map) {
		if (element.second < _node_relevance_min) {
			if (element.second < worst_node.second) {
				worst_node = element;
			}
			flag = true;
		}
	}
	return { worst_node.first,flag };
}

std::tuple<int, int, bool> Solution::get_edge_invalide_min() {
	bool flag = false;
	std::pair<std::pair<int, int>, float> worst_edge = { { -1,-1},std::numeric_limits<float>::max() };
	for (auto element : _edge_relevance_map) {
		if (element.second < _edge_relevance_min) {
			if (element.second < worst_edge.second) {
				worst_edge = element;
			}
			flag = true;
		}
	}
	return { worst_edge.first.first,worst_edge.first.second, flag };
}

std::tuple<int, int, std::string, bool> Solution::get_edge_state_invalide_min() {
	bool flag = false;
	std::pair<std::tuple<int, int, std::string>, float> worst_edge_state = { {-1,-1,""} , std::numeric_limits<float>::max() };
	for (auto element : _state_relevance_map) {
		if (element.second < _state_relevance_min) {
			if (element.second < worst_edge_state.second) {
				worst_edge_state = element;
			}
			flag = true;
		}
	}
	int source, target;
	std::string state;
	std::tie(source, target, state) = worst_edge_state.first;
	return { source, target, state, flag };
}

void Solution::evaluation_penality(Data& data) {
	evaluation(data);
	_relevance_graph_penality = _relevance_graph;
	for (auto node_relevance : _node_relevance_map) {
		if (node_relevance.second < _node_relevance_min) {
			_relevance_graph_penality = _relevance_graph_penality - node_relevance.second;//(_node_relevance_min - node_relevance.second);
		}
	}
	for (auto edge_relevance : _edge_relevance_map) {
		if (edge_relevance.second < _edge_relevance_min && edge_relevance.second != 0) {
			_relevance_graph_penality = _relevance_graph_penality - edge_relevance.second; (_edge_relevance_min - edge_relevance.second);
		}
	}
	for (auto state_relavance : _state_relevance_map) {
		if (state_relavance.second < _state_relevance_min && state_relavance.second != 0) {
			_relevance_graph_penality = _relevance_graph_penality - state_relavance.second;//(_state_relevance_min - state_relavance.second);
		}
	}
}

float Solution::get_relevance_graph_penality() {
	return _relevance_graph_penality;
}

void Solution::set_relevance_graph_penality(float relevance_graph_penality) {
	_relevance_graph_penality = relevance_graph_penality;
}