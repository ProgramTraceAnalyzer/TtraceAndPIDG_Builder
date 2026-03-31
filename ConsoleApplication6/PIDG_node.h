#pragma once
#include "ExprNode.h"

struct PIDG_node
{
	int id;
	bool is_final;
	std::string assign_var = "";
	ExprNode* expr;
	std::map<std::string, PIDG_node*> parent_nodes_by_var;

	std::string to_string() const;
	bool equals_to(PIDG_node* other) const;

};

PIDG_node* get_abstract_PIDG_node(PIDG_node* node, bool abstract_const = false, bool abstract_operator = false);