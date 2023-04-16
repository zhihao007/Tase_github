#include"Data.h"
Data::Data() {
	_mean_length = 0;
	_max_length = 0;
	_optimal_solution = 0;
	_p_max = 0 ;
	_node_relevance_min = 0;
	_edge_relevance_min = 0;
	_state_relevance_min = 0;
}

Data::Data(Eventlog_type& eventlog) {
	_eventlog = eventlog;
	_mean_length = 0;
	_max_length = 0;
	_optimal_solution = 0;
	_p_max = 0;
	_node_relevance_min = 0;
	_edge_relevance_min = 0;
	_state_relevance_min = 0;
}

bool Data::read_file(std::string fname) {
	std::vector<std::vector<std::string>> content;
	std::vector<std::string> row;
	std::string line, word;
	std::fstream file(fname, std::ios::in);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			row.clear();

			std::stringstream str(line);

			while (std::getline(str, word, ';'))
				row.push_back(word);
			content.push_back(row);
		}
	}
	else {
		std::cout << "Could not open the file\n";
		return false;
	}
	//find the cols of id, label and state
	int col_id = -1;
	int col_label = -1;
	int col_state = -1;
	int col_optimal_solution = -1;
	int col_node_relevance = -1;
	int col_edge_relevance = -1;
	int col_state_relevance = -1;
	int col_p_max = -1;

	for (int i = 0; i < content[0].size(); i++)
	{
		if (content[0][i] == "id_patient") {
			col_id = i;
		}
		if (content[0][i] == "label") {
			col_label = i;
		}
		if (content[0][i] == "patient_status") {
			col_state = i;
		}
		if (content[0][i] == "optimal_solution") {
			col_optimal_solution = i;
		}
		if (content[0][i] == "node_relevance_min") {
			col_node_relevance = i;
		}
		if (content[0][i] == "edge_relevance_min") {
			col_edge_relevance = i;
		}
		if (content[0][i] == "state_relevance_min") {
			col_state_relevance = i;
		}
		if (content[0][i] == "p_max") {
			col_p_max = i;
		}
	}
	//find all the patient ids
	std::set<std::string> patients;
	for (int i = 1; i < content.size(); i++) {
		if (content[i][col_id] == "") {
			continue;
		}
		patients.insert(content[i][col_id]);
	}
	std::vector<int> patients_sorted;
	for (auto patient : patients) {
		patients_sorted.push_back(std::stoi(patient));
	}
	std::sort(patients_sorted.begin(), patients_sorted.end());

	//remplir eventlog
	for (auto patient : patients_sorted) {
		Trace_type trace;
		for (int i = 1; i < content.size(); i++) {
			if (content[i][col_id] == std::to_string(patient)) {
				std::map<std::string, std::string> record;
				record["label"] = content[i][col_label];
				record["state"] = content[i][col_state];
				trace.push_back(record);
			}
		}
		trace.back()["state"] = "";
		_eventlog.push_back(trace);
	}
	if (col_optimal_solution != -1 && col_node_relevance != -1 && col_edge_relevance != -1 && col_state_relevance != -1 && col_p_max != -1) {
		_optimal_solution = std::stof(content[1][col_optimal_solution]);
		_node_relevance_min = std::stof(content[1][col_node_relevance]);
		_edge_relevance_min = std::stof(content[1][col_edge_relevance]);
		_state_relevance_min = std::stof(content[1][col_state_relevance]);
		_p_max = std::stoi(content[1][col_p_max]);
	}
	file.close();
	return true;
}


Eventlog_type& Data::get_eventlog() {
	return _eventlog;
}

void Data::print() {
	/*
	for (auto i = _eventlog.begin(); i != _eventlog.end(); i++) {
		auto trace = *i;
		std::cout << "trace: " << std::endl;
		for (auto j = trace.begin(); j != trace.end(); j++) {
			auto event = *j;
			std::cout << "label: " << event["label"] << " state: " << event["state"] << std::endl;
		}
	}
	std::cout << std::endl;

	std::cout << "-------------------event set------------------------" << std::endl;
	std::cout << _event_set << std::endl;
	std::cout << "-------------------event position set------------------------" << std::endl;
	std::cout << _event_position_set << std::endl;
	std::cout << "-------------------state set------------------------" << std::endl;
	std::cout << _state_set << std::endl;
	std::cout << "-------------------transition set------------------------" << std::endl;
	std::cout << _transition_table << std::endl;
	*/
	std::cout << "-------------------patients------------------------" << std::endl;
	std::cout << _eventlog.size() << std::endl;
	std::cout << "-------------------mean length------------------------" << std::endl;
	std::cout << _mean_length << std::endl;
	std::cout << "-------------------max length------------------------" << std::endl;
	std::cout << _max_length << std::endl;
}

void Data::analyse_eventlog() {
	int length = 0;
	for (auto trace : _eventlog) {
		Trace_state_table_type trace_state_table;
		length = length + (int)trace.size();
		if (_max_length < (int)trace.size()) {
			_max_length = (int)trace.size();
		}
		for (int i = 0; i < trace.size(); i++) {
			_event_set.insert(trace[i]["label"]);
			_event_position_set.insert({ trace[i]["label"],i });
			if (trace[i]["state"] != "") {
				_state_set.insert(trace[i]["state"]);
			}
			if (i != trace.size() - 1) {
				if (_transition_table.find({ trace[i]["label"] ,trace[i + 1]["label"],trace[i]["state"] }) != _transition_table.end()) {
					_transition_table[{ trace[i]["label"], trace[i + 1]["label"], trace[i]["state"] }] = _transition_table[{ trace[i]["label"], trace[i + 1]["label"], trace[i]["state"] }] + 1;
				}
				else {
					_transition_table[{ trace[i]["label"], trace[i + 1]["label"], trace[i]["state"] }] = 1;
				}
			}
			if (_event_occurence_table.find(trace[i]["label"]) != _event_occurence_table.end()) {
				_event_occurence_table[trace[i]["label"]] = _event_occurence_table[trace[i]["label"]] + 1;
			}
			else {
				_event_occurence_table[trace[i]["label"]] = 1;
			}
			for (int j = 0; j < trace.size(); j++) {
				if (i < j) {
					std::vector<std::string> states;
					for (int k = i; k < j; k++) {
						states.push_back(trace[k]["state"]);
					}
					trace_state_table[std::make_pair(i, j)] = states;
				}
			}
		}
		_trace_state_table_list.push_back(trace_state_table);
	}
	_mean_length = length / (int)_eventlog.size();
}

std::vector<Trace_state_table_type>& Data::get_trace_state_table_list() {
	return _trace_state_table_list;
}

std::map<Transition_state_type, int>& Data::get_transition_table() {
	return _transition_table;
}

std::set<std::string>& Data::get_state_set() {
	return _state_set;
}

int Data::get_mean_length() {
	return _mean_length;
}

std::map<std::string, int>& Data::get_event_occurence_table() {
	return _event_occurence_table;
}

std::set<std::string>& Data::get_event_set() {
	return _event_set;
}

bool Data::initialisation(std::string fname) {
	if (read_file(fname) == false) {
		return false;
	}
	analyse_eventlog();
	return true;
}

float Data::get_optimal_solution() {
	return _optimal_solution;
}

int Data::get_p_max() {
	return _p_max;
}

float Data::get_node_relevance_min() {
	return _node_relevance_min;
}

float Data::get_edge_relevance_min() {
	return _edge_relevance_min;
}

float Data::get_state_relevance_min() {
	return _state_relevance_min;
}

