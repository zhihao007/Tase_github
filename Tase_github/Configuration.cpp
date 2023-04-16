#include"Configuration.h"
//common
int Configuration::_iteration_max = 200000;
int Configuration::_no_improvement_iteration_max = 200000;
std::string Configuration::_fname = "";
float Configuration::_duration_time = 600;


//solution
int Configuration::_p_max = 12;
float Configuration::_node_relevance_min = 100;
float Configuration::_edge_relevance_min = 100;
float Configuration::_state_relevance_min = 100;
float Configuration::_lamda = 0.5;
float Configuration::_alpha = 1;
//programmation dynamique
int Configuration::_node_begin_position = -1;
int Configuration::_node_end_position = 999;

Configuration::Configuration() {

}
