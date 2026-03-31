#pragma once
#include "ExprNode.h"

enum execution_element_type { ACTION, STATE };
enum action_type { ASSIGN, LOGIC, SKIP };

struct ActionRWs
{
	std::set<std::string> read_vars;
	std::set<std::string> write_vars;
	action_type act_type;

	std::string toHTML() const;
};


struct ExecutionElement {
	execution_element_type type;
	std::map<std::string, int> var_values;
	std::string action_string;
	action_type act_type;
	std::string assign_var;
	ExprNode* assign_expr = nullptr;
	int loc = -1;

	std::set<std::string> findReadVariables() const;
	bool executeAction(std::map<std::string, int>& var_values);
	bool assigns_var(std::string var_name) const;
	std::string toHTML() const;
	ActionRWs getActionRWs() const;
	ExecutionElement getAbstractedAction(bool abstract_const = false, bool abstract_operator = false) const;
};

/*

struct ExecutionElement
{
	enum execution_element_type type;
	std::map <std::string, int> var_values;
	std::string action_string;
	action_type act_type;
	std::string assign_var;
	ExprNode* assign_expr = NULL;
	int loc = -1;

	std::set<std::string> findReadVariables() const
	{
		std::set<std::string> vars;
		findExprVariables(assign_expr, vars);
		return vars;
	}

	bool executeAction(std::map<std::string, int>& var_values)
	{
		if (type == ACTION)
		{
			switch (act_type)
			{
			case ASSIGN:
				var_values[assign_var] = calcExprNode(assign_expr, var_values);
				return true;
			case LOGIC:
				return calcExprNode(assign_expr, var_values);
			default:
				break;
			}
		}
	}

	bool assigns_var(std::string var_name) const
	{
		return type == ACTION && act_type == ASSIGN && assign_var == var_name;
	}


	std::string toHTML() const
	{
		std::string html = "";
		switch (type)
		{
		case ACTION:
			switch (act_type)
			{
			case ASSIGN:
				return "<p>" + assign_var + " = " + assign_expr->toString() + "</p>";
			case LOGIC:
				return "<p>" + assign_expr->toString() + "</p>";
			case SKIP:
				return "<p>skip</p>";
			}

			break;
		case STATE:

			std::string first_row = "";
			std::string second_row = "";
			for (auto it = var_values.cbegin(); it != var_values.cend(); it++)
			{
				first_row += "<td>" + ((it->first)) + "</td>";
				second_row += "<td>" + std::to_string(((it->second))) + "</td>";
			}
			first_row = "<tr> <td>loc</td>" + first_row + "</tr>";
			second_row = "<tr> <td>" + std::to_string(loc) + "</td>" + second_row + "</tr>";
			html = "<table class=\"table table-striped\"  border=\"l\">" + first_row + second_row + "</table>";
			break;
		}
		return html;
	}

	ActionRWs getActionRWs() const
	{
		std::set<std::string> write;
		if (act_type == ASSIGN)
			write.insert(assign_var);
		ActionRWs arws = { findReadVariables(), write, act_type };
		return arws;
	}

	ExecutionElement getAbstractedAction(bool abstract_const = false, bool abstract_operator = false) const
	{
		ExecutionElement e;
		if (type == ACTION)
		{
			e.type = type;
			e.act_type = act_type;
			e.assign_var = assign_var;
			e.assign_expr = get_abstract_expr_node(assign_expr, abstract_const, abstract_operator);
		}
		return e;
	}
};


*/