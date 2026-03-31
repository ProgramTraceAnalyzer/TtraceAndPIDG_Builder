#pragma once
#include "Memory.h"
#include "ExprNode.h"
#include "Variable.h"

enum action_type { ASSIGN, LOGIC, SKIP };

struct Action
{
	enum action_type type;
	ExprNode* action_expr;
	struct Variable assigned_variable;
	int changed_value;
	bool executeAction(Memory & memory, Action * action_finger_print = NULL) const;
	std::string to_html() const;
	std::string to_json() const;
	std::string to_string(bool add_effect=false) const;
};

Action create_assign_action(Variable& var, ExprNode* assign_expr);
Action create_logic_action(ExprNode* logic_expr);
Action create_skip_action();