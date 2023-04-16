#pragma once
#ifndef _Solution_h_included
#define _Solution_h_included

#include"Graph.h"
#include"Data.h"
#include<limits>
class Solution {
private:
	Graph _graph;
	std::vector<float> _g_prime_list;
	std::vector<std::vector<int>> _sigma_list;
	std::vector<std::vector<int>> _theta_list;
	std::vector<std::map<std::pair<int, int>, float>> _A_list;
	std::vector<Trace_state_table_type> _B_list;

	std::map<int, float> _node_relevance_map;
	std::map<std::pair<int, int>, float> _edge_relevance_map;
	std::map<std::tuple<int, int, std::string>, float> _state_relevance_map;
	float _relevance_graph;
	float _relevance_graph_penality;

	float _node_relevance_min;
	float _edge_relevance_min;
	float _state_relevance_min;
	float _lamda;
	float _alpha;
	int _node_begin_position;
	int _node_end_position;


public:
	Solution();
	Solution(Graph &graph);
	Graph& get_graph();
	std::vector<float>& get_g_prime_list();
	std::vector<std::vector<int>>& get_sigma_list();
	std::vector<std::vector<int>>& get_theta_list();
	std::vector<std::map<std::pair<int, int>, float>>& get_A_list();
	std::vector<Trace_state_table_type>& get_B_list();
	std::map<int, float>& get_node_relevance_map();
	std::map<std::pair<int, int>, float>& get_edge_relevance_map();
	std::map<std::tuple<int, int, std::string>, float>& get_state_relevance_map();
	float get_relevance_graph();
	void print();
	bool is_replayed(int event_index, int node_index, Trace_type& trace, int trace_begin_index, int trace_end_index, int graph_begin_index, int graph_end_index);
	int get_position(int node_index, int graph_begin_index, int graph_end_index);
	std::set<std::string> get_states_trace(int i, int j, Trace_type& trace);
	float score_node(int event_index, int node_index, Trace_type& trace);
	float score_edge(int event_i_index, int event_j_index, int node_i_index, int node_j_index, Trace_state_table_type& trace_state_table);
	void replayability(Trace_type& trace, Trace_state_table_type& trace_state_table);
	void replay_all_traces(Data& data);
	void relevance_node();
	void relevance_edge();
	void relevance_state();
	void relevance_graph();
	void evaluation(Data& data);
	bool feasibility();
	void clean();
	void copy(Solution& solution);
	float get_node_relevance_min();
	float get_edge_relevance_min();
	float get_state_relevance_min();
	std::pair<int,bool> get_node_invalide_min();
	std::tuple<int, int, bool> get_edge_invalide_min();
	std::tuple<int, int, std::string, bool> get_edge_state_invalide_min();
	void evaluation_penality(Data& data);
	float get_relevance_graph_penality();
	void set_relevance_graph_penality(float relevance_graph_penality);
};

#endif // !_Solution_h_included
