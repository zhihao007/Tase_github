#pragma once
#ifndef _Heuristique_our_h_included
#define _Heuristique_our_h_included
#include"Graph.h"
#include"Data.h"
#include"Solution.h"
#include"Printer.h"
#include <random>

class Heuristique_our {
private:
	std::string _fname;
	int _iteration_max;
	int _no_improvement_iteration_max;
	int _p_max;
	float _duration_time;

	Data _data;
	std::mt19937 _rng;
	std::uniform_real_distribution<double> _distribution_init;

	std::shared_ptr<Solution> _best_solution;

public:
	Heuristique_our(Data& data);
	bool initialisation();
	void print();
	Data& get_data();
	void initialise_random_generator();
	bool node_existe(Graph& graph, std::string label, int position);
	int node_find(Graph& graph, std::string label, int position);
	void go();
	void complete_graph(Graph& graph, std::set<std::string>& stat_set);
	void insert_node_graph(Graph& graph, std::string label, int position, std::set<std::string>& state_set);
	bool repair_solution_progressive_nonfixed_size(std::shared_ptr<Solution> old_solution, std::shared_ptr<Solution> new_solution);
	float best_solution();
	std::string get_instance_name();
	std::shared_ptr<Solution> random_solution();
	void cal_node_score(std::shared_ptr<Solution> solution, std::vector<std::tuple<std::string,int,int>>& node_score);
	void test();
};


#endif // !_Heuristique_our_h_included
