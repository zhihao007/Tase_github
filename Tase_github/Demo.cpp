#include"Solution.h"
#include"Data.h"
#include"Printer.h"
#include <chrono>
#include <string>
#include<numeric>
#include"Configuration.h"
#include"Heuristique_our.h"


int main(int argc, char** argv) {
	Configuration::_fname = "../input/" + std::string(argv[1]);
	
	int num_runs = 10;
	if (std::string(argv[2]) != "-1") {
		num_runs = std::stoi(argv[2]);
	}
	if (std::string(argv[3]) != "-1") {
		Configuration::_iteration_max = std::stoi(argv[3]);
	}
	if (std::string(argv[4]) != "-1") {
		Configuration::_no_improvement_iteration_max = std::stoi(argv[4]);
	}
	if (std::string(argv[5]) != "-1") {
		Configuration::_duration_time = std::stof(argv[5]);
	}
	if (std::string(argv[6]) != "-1") {
		Configuration::_node_relevance_min = std::stof(argv[6]);
	}
	if (std::string(argv[7]) != "-1") {
		Configuration::_edge_relevance_min = std::stof(argv[7]);
	}
	if (std::string(argv[8]) != "-1") {
		Configuration::_state_relevance_min = std::stof(argv[8]);
	}
	if (std::string(argv[9]) != "-1") {
		Configuration::_p_max = std::stoi(argv[9]);
	}

	std::cout << "*********************** Heuristique***********************\n";
	std::vector<float> vec_best_solution;
	std::vector<float> vec_time;
	std::string instance_name;
	float best_solution_average = 0;
	float cal_time_average = 0;
	float optimal_solution = 0;
	for (int i = 0; i < num_runs; i++) {
		std::cout << "*********************** run " << i << "***********************\n";
		auto start = std::chrono::high_resolution_clock::now();
		Data data;
		data.initialisation(Configuration::_fname);
		Heuristique_our method(data);
		method.print();
		method.go();
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
		float temp = (float)duration.count() / 1000;
		best_solution_average = best_solution_average + method.best_solution();
		vec_best_solution.push_back(method.best_solution());
		vec_time.push_back(temp);
		instance_name = method.get_instance_name();
		cal_time_average = cal_time_average + temp;
		optimal_solution = data.get_optimal_solution();
		std::cout << temp << " seconds \n";
	}
	best_solution_average = best_solution_average / num_runs;
	cal_time_average = cal_time_average / num_runs;
	std::cout << "instance name = " << instance_name << "\n";
	std::cout << "optimal solution = " << optimal_solution << "\n";
	std::cout << "best solutions of all the runs = " << vec_best_solution << "\n";
	std::cout << "time of all the runs = " << vec_time << "\n";
	std::cout << "average best solution = " << best_solution_average << "\n";
	std::cout << "average time = " << cal_time_average << " seconds \n";

	return 0;
}
