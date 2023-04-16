#pragma once
#ifndef _Data_h_included
#define _Data_h_included
#include<iostream>
#include <fstream>
#include <sstream>
#include<vector>
#include<map>
#include<string>
#include<variant>
#include<set>
#include <algorithm>
#include"Printer.h"
typedef std::vector<std::map<std::string, std::string>> Trace_type;
typedef std::vector<Trace_type> Eventlog_type;
typedef std::map<std::pair<int, int>, std::vector<std::string>> Trace_state_table_type;
typedef std::tuple<std::string, std::string, std::string> Transition_state_type;
class Data {
private:
	std::vector<Trace_type> _eventlog;
	std::vector<Trace_state_table_type> _trace_state_table_list;
	std::set<std::string> _event_set;
	std::set<std::tuple<std::string, int>> _event_position_set;
	std::set<std::string> _state_set;
	std::map<Transition_state_type, int> _transition_table;
	std::map<std::string, int> _event_occurence_table;
	int _mean_length;
	int _max_length;
	float _optimal_solution;
	int _p_max;
	float _node_relevance_min;
	float _edge_relevance_min;
	float _state_relevance_min;
public:
	Data();
	Data(Eventlog_type& eventlog);
	bool initialisation(std::string fname);
	bool read_file(std::string fname);
	void analyse_eventlog();
	void print();
	Eventlog_type& get_eventlog();
	std::vector<Trace_state_table_type>& get_trace_state_table_list();
	std::map<Transition_state_type, int>& get_transition_table();
	std::set<std::string>& get_state_set();
	int get_mean_length();
	std::map<std::string, int>& get_event_occurence_table();
	std::set<std::string>& get_event_set();
	float get_optimal_solution();
	int get_p_max();
	float get_node_relevance_min();
	float get_edge_relevance_min();
	float get_state_relevance_min();
};


#endif // !_Data_h_included
