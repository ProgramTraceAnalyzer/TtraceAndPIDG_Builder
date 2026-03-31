#pragma once
#include "ExprNode.h"

enum variable_type { SCALAR_VAR, CONCRETE_ARR_ELEMENT, ABSTRACT_ARR_ELEMENT };
struct Variable
{
	enum variable_type type;
	std::string name;
	int index;
	ExprNode* index_expr;
	std::string to_string() const;
	bool operator==(const Variable & other) const;
	bool operator<(const Variable & other) const;
};

Variable create_scalar_variable(std::string name);
Variable create_concrete_array_element_variable(std::string name, int index);
Variable create_abstract_array_element_variable(std::string name, ExprNode* index_expr);
void find_all_read_variables_in_expr(ExprNode* node, std::set<Variable>& read_variables, const Memory& memory);