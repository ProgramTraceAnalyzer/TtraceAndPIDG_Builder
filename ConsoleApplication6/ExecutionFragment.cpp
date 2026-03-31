#include "ExecutionFragment.h"

std::string ExecutionFragment::to_html() const
{
	std::string html = "<table class = \"table table-striped\">";
	auto memseq_it = this->state_sequence.states.cbegin();
	auto actseq_it = this->action_sequence.action_seq.cbegin();
	html += "<tr><th>S0</th><td>" + memseq_it->to_html() + "</td></tr>";
	memseq_it++;
	while (memseq_it != this->state_sequence.states.cend() && actseq_it != this->action_sequence.action_seq.cend())
	{
		int act_num = actseq_it - this->action_sequence.action_seq.cbegin();
		int memseq_num = memseq_it - this->state_sequence.states.cbegin();
		html += "<tr><th>A"+std::to_string(act_num)+"</th><td>" + actseq_it->to_html() + "</td></tr>";
		html += "<tr><th>S" + std::to_string(memseq_num) + "</th><td>" + memseq_it->to_html() + "</td></tr>";
		actseq_it++;
		memseq_it++;
	}
	html += "</table>";
	return html;
}

std::vector<std::set<Variable>> ExecutionFragment::get_sequence_read_variables() const
{
	std::vector<std::set<Variable>> read_seq;
	int size = this->action_sequence.action_seq.size();
	for (int i = 0; i < size; i++)
	{
		Action act = this->action_sequence.action_seq.at(i);
		std::set<Variable> read_variables;
		find_all_read_variables_in_expr(act.action_expr, read_variables, this->state_sequence.states.at(i).memory);

		if (act.type == ASSIGN && act.assigned_variable.type == CONCRETE_ARR_ELEMENT)
		{
			find_all_read_variables_in_expr(act.assigned_variable.index_expr, read_variables, this->state_sequence.states.at(i).memory);
		}
		read_seq.push_back(read_variables);
	}
	return read_seq;
}

std::string ExecutionFragment::get_json_sequence_read_variables() const
{
	std::string json = "";
	std::vector<std::set<Variable>> read_seq = get_sequence_read_variables();
	for (int i = 0; i < read_seq.size(); i++)
	{
		if (i > 0)
			json += ",";
		std::set<Variable> read_vars = read_seq.at(i);
		std::string curr_json = "";
		for (auto it = read_vars.cbegin(); it != read_vars.cend(); it++)
		{
			if (it != read_vars.cbegin())
				curr_json += ",";
			curr_json += "\"" + it->to_string() + "\"";
		}
		json += "[" + curr_json + "]";
	}
	json = "[" + json + "]";
	return json;
}

PIDG ExecutionFragment::build_PIDG() const
{
	PIDG pidg;
	pidg.action_sequence = this->action_sequence;
	
	int size = this->action_sequence.action_seq.size();
	for (int i = 0; i < size; i++)
	{
		Action act = this->action_sequence.action_seq.at(i);
		std::set<Variable> read_variables;
		find_all_read_variables_in_expr(act.action_expr, read_variables, this->state_sequence.states.at(i).memory);
		
		if (act.type == ASSIGN && act.assigned_variable.type == CONCRETE_ARR_ELEMENT)
		{
			find_all_read_variables_in_expr(act.assigned_variable.index_expr, read_variables, this->state_sequence.states.at(i).memory);
		}
		for (auto it = read_variables.cbegin(); it != read_variables.cend(); it++)
		{
			int last_assign = find_last_variable_assign_for_action(i, *it);
			if (last_assign != -1)
				pidg.edges[i][last_assign] = *it;
			else
				pidg.initial_variables[i].insert(*it);
		}

	}
	return pidg;
}

std::map<int, std::map<int, float>> match_two_PIDGs(PIDG pidg1, PIDG pidg2)
{
	std::map<int, std::map<int, float>> heat_map;
	for (int i = 0; i < pidg1.action_sequence.action_seq.size(); i++)
	{
		for (int j = 0; j < pidg2.action_sequence.action_seq.size(); j++)
		{
			Action act1 = pidg1.action_sequence.action_seq.at(i);
			Action act2 = pidg2.action_sequence.action_seq.at(j);
			if (act1.type != act2.type)
				heat_map[i][j] = 0;
			else
			{
				float score = 0;
				if (act1.type == ASSIGN)
				{
					if (act1.assigned_variable == act2.assigned_variable)
					{
						score += 0.25;
					}
					if (act1.changed_value == act2.changed_value)
					{
						score += 0.25;
					}
					score += 0.5 * comapre_pidg_nodes_by_read_variables(pidg1, i, pidg2, j);
				}
				else if (act1.type == LOGIC)
				{
					score =  comapre_pidg_nodes_by_read_variables(pidg1, i, pidg2, j);
				}
				heat_map[i][j] = score;
			}
		}
	}
	return heat_map;
}

float comapre_pidg_nodes_by_read_variables(const PIDG& pidg1, int node1, const PIDG& pidg2, int node2)
{
	std::set<int> matched_vars_pidg1, matched_vars_pidg2;
	std::set<int> no_matched_vars_pidg1, no_matched_vars_pidg2;
	
	std::map<int, Variable> pidg1_edges = {};
	if (pidg1.edges.count(node1))
		pidg1_edges = pidg1.edges.at(node1);

	std::map<int, Variable> pidg2_edges = {};
	if (pidg2.edges.count(node2))
		pidg2_edges = pidg2.edges.at(node2);

	int size = pidg1_edges.size();
	int fully_matched = 0;
	int by_var_matched = 0;
	int by_effect_matched = 0;
	for (auto it1 = pidg1_edges.cbegin(); it1 != pidg1_edges.cend(); it1++)
	{
		bool matched = false;
		bool effect_matched = false;
		for (auto it2 = pidg2_edges.cbegin(); it2 != pidg2_edges.cend() && !matched; it2++)
		{
			int num1 = it1->first;
			int num2 = it2->first;
			if (it1->second == it2->second)
			{
				matched_vars_pidg1.insert(it1->first);
				matched = true;
			}
			if (pidg1.action_sequence.action_seq.at(num1).type == ASSIGN &&
				pidg2.action_sequence.action_seq.at(num2).type == ASSIGN)
			effect_matched = (pidg1.action_sequence.action_seq.at(num1).changed_value
				== pidg2.action_sequence.action_seq.at(num2).changed_value);
			
		}
		if (matched && effect_matched)
			fully_matched++;
		else if (matched)
			by_var_matched++;
		else if (effect_matched)
			by_effect_matched++;
		if (!matched)
		{
			no_matched_vars_pidg1.insert(it1->first);
		}
	}

	int init_size = {};
	if (pidg1.initial_variables.count(node1))
		init_size = pidg1.initial_variables.at(node1).size();
	int init_matched = 0;
	if (pidg1.initial_variables.count(node1))
	for (auto it = pidg1.initial_variables.at(node1).cbegin(); it != pidg1.initial_variables.at(node1).cend(); it++)
	{
		if (pidg2.initial_variables.count(node2) && pidg2.initial_variables.at(node2).count(*it))
			init_matched++;
	}
	int sum_size = size + init_size;
	if (sum_size == 0)
		return 1;
	if (init_size>0 && size>0)
		return 0.6*(float)(fully_matched) / size + 0.4*(float)init_matched/init_size + 0.5*(float)by_var_matched / sum_size;
	else if (init_size==0 && size>0)
		return  (float)(fully_matched) / size  + 0.5 * (float)by_var_matched / sum_size;
}

int ExecutionFragment::find_last_variable_assign_for_action(int action_index, const Variable& var) const
{
	
	for (int i = action_index - 1; i >= 0; i--)
	{
		Action act = this->action_sequence.action_seq.at(i);
		if (act.type == ASSIGN && act.assigned_variable == var)
			return i;
	}
	
	return -1;
}

std::string ExecutionFragment::build_PIDG_dot() const
{
	
	std::string dot = "digraph G { ";
	PIDG pidg = build_PIDG();
	for (auto it = this->action_sequence.action_seq.cbegin(); it != this->action_sequence.action_seq.cend(); it++)
	{
		int num = it - this->action_sequence.action_seq.cbegin();
		dot += std::to_string(num) + " [label=\"#"+std::to_string(num)+ "\\n" + it->to_string(true) + "\"] ";
		/*if (pidg.edges.count(num) == 0)
			dot += std::to_string(num) + " -> init ";*/
	}
	for (auto it1 = pidg.edges.cbegin(); it1 != pidg.edges.cend(); it1++)
	{
		for (auto it2 = it1->second.cbegin(); it2 != it1->second.cend(); it2++)
		{
			dot += std::to_string(it1->first) + " -> " + std::to_string(it2->first)+" [label=\""+it2->second.to_string()+"\"]";
		}
	}

	for (auto it1 = pidg.initial_variables.cbegin(); it1 != pidg.initial_variables.cend(); it1++)
	{
		for (auto it2 = it1->second.cbegin(); it2 != it1->second.cend(); it2++)
			dot += std::to_string(it1->first) + " -> init [label=\"" + it2->to_string() + "\"] ";
	}
	dot += "}";
	return dot;
}