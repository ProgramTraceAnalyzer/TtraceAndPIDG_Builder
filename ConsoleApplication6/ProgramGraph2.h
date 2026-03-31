#pragma once
#include "ExecutionFragment.h"
class ProgramGraph2
{
private:
	std::map<int, std::map<int, Action>> action_matrix;
	int terminal_loc;
	Memory initial_memory;
	int do_step(int loc, Memory & memory, ExecutionFragment & execution_element) const;
public:
	ExecutionFragment execute(int max_steps = 10000) const;
	ProgramGraph2();
	ProgramGraph2(Memory initial_memory, int terminal_loc);
	void add_action_edge(int loc_from, int loc_to, Action& act);
	std::string to_dot() const;
	void set_terminal_loc(int l);
	//void add_assign_action();
};

