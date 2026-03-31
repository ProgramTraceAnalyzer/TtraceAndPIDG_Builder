#include "ProgramGraph2.h"

ExecutionFragment ProgramGraph2::execute(int max_steps) const
{
	
	ExecutionFragment execution_fragment;
	Memory memory = initial_memory;
	execution_fragment.state_sequence.states.push_back({ 0,memory });
	int currentLoc = 0;
	int step_count = 0;
	while (currentLoc != terminal_loc && step_count != max_steps) {
		currentLoc = do_step(currentLoc, memory, execution_fragment);
		if (currentLoc == -1)
			return execution_fragment;
		step_count++;
	}
	return execution_fragment;
}

ProgramGraph2::ProgramGraph2()
{

}

void ProgramGraph2::set_terminal_loc(int l)
{
	this->terminal_loc = l;
}

int ProgramGraph2::do_step(int loc, Memory& memory, ExecutionFragment& execution_element) const
{
	auto it = action_matrix.find(loc);
	if (it == action_matrix.end()) {
		return -1; // No actions from this location
	}
	const std::map<int, Action> action_variants = it->second;
	for (auto it = action_variants.cbegin(); it != action_variants.cend(); it++) {
		int new_loc = it->first;
		Action act = it->second;
		Action* finger_print_act = new Action;
		if (act.executeAction(memory, finger_print_act)) {
			if (finger_print_act->type == ASSIGN)
			{
				switch (finger_print_act->assigned_variable.type)
				{
				case SCALAR_VAR:
					finger_print_act->changed_value = memory.get_scalar_variable_value(finger_print_act->assigned_variable.name);
					break;
				case CONCRETE_ARR_ELEMENT:
					finger_print_act->changed_value = memory.get_array_element_value(finger_print_act->assigned_variable.name, finger_print_act->assigned_variable.index);
					break;
				default:
					break;
				}

			}
				
			execution_element.action_sequence.action_seq.push_back(*finger_print_act);
			Memory mem = memory;
			execution_element.state_sequence.states.push_back({new_loc,mem});
			return new_loc;
		}
	}
	return -1;
	
}

ProgramGraph2::ProgramGraph2(Memory initial_memory, int terminal_loc)
{
	this->initial_memory = initial_memory;
	this->terminal_loc = terminal_loc;
}

void ProgramGraph2::add_action_edge(int loc_from, int loc_to, Action& act)
{
	action_matrix[loc_from][loc_to] = act;
}

std::string ProgramGraph2::to_dot() const
{
	std::string dot = "";
	for (auto it = action_matrix.cbegin(); it != action_matrix.cend(); it++)
	{
		int from_loc = it->first;
		std::map<int, Action> map_to = it->second;
		for (auto it2 = map_to.cbegin(); it2 != map_to.cend(); it2++)
		{
			int to_loc = it2->first;
			Action act = it2->second;
			dot += std::to_string(from_loc) + " -> " + std::to_string(to_loc) + " [label=\"" + act.to_string() + "\"]\n";
		}
	}
	dot = "digraph G { " + dot + "}";
	return dot;
}