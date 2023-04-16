#pragma once
#ifndef Configuration_h_included
#define Configuration_h_included
#include<iostream>
#include"Data.h"
class Configuration {
public:
	//common
	static int _iteration_max;
	static int _no_improvement_iteration_max;
	static std::string _fname;
	static std::string _graph_name;
	static float _duration_time;

	//solution
	static int _p_max;
	static float _node_relevance_min;
	static float _edge_relevance_min;
	static float _state_relevance_min;
	static float _lamda;
	static float _alpha;
	static int _node_begin_position;
	static int _node_end_position;

	Configuration();
	
};













#endif // !Configuration_h_included
