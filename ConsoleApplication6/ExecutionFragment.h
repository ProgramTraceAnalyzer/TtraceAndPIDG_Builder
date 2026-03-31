#pragma once
#include "MemorySequence.h"
#include "ActionSequence.h"
#include "StateSequence.h"

struct PIDG
{
	std::map<int, std::map<int, Variable>> edges;
	std::map<int, std::set<Variable>> initial_variables;
	ActionSequence action_sequence;
};

std::map<int, std::map<int, float>> match_two_PIDGs(PIDG pidg1, PIDG pidg2);
float comapre_pidg_nodes_by_read_variables(const PIDG& pidg1, int node1, const PIDG& pidg2, int node2);

struct ExecutionFragment
{
	StateSequence state_sequence;
	ActionSequence action_sequence;

	std::string to_html() const;
	PIDG build_PIDG() const;
	int find_last_variable_assign_for_action(int action_index, const Variable& var) const;
	std::string build_PIDG_dot() const;
	std::vector<std::set<Variable>> get_sequence_read_variables() const;
	std::string get_json_sequence_read_variables() const;
};