#include "Variable.h"

Variable create_scalar_variable(std::string name)
{
	Variable var = {SCALAR_VAR, name};
	return var;
}

Variable create_concrete_array_element_variable(std::string name, int index)
{
	Variable var = { CONCRETE_ARR_ELEMENT, name, index };
	return var;
}

Variable create_abstract_array_element_variable(std::string name, ExprNode* index_expr)
{
	Variable var = {ABSTRACT_ARR_ELEMENT, name};
	var.index_expr = copy_node(index_expr);
	return var;
}

bool Variable::operator==(const Variable& other) const
{
	if (this->type != other.type)
		return false;
	bool res;
	switch (type)
	{
	case SCALAR_VAR:
		res = (name == other.name);
		break;
	case CONCRETE_ARR_ELEMENT:
		res = (name == other.name && index == other.index);
		break;
	case ABSTRACT_ARR_ELEMENT:
		res = (name == other.name && index_expr->equalsTo(other.index_expr));
		break;
	}
	return res;
}

void find_all_read_variables_in_expr(ExprNode* node, std::set<Variable> &read_variables, const Memory & memory)
{
	if (node)
	{
		if (node->type == exprType::VAR)
			read_variables.insert(create_scalar_variable(node->var_name));
		else if (node->type == ARRAY_INDEX)
		{
			read_variables.insert(create_concrete_array_element_variable(node->left->var_name, calcExprNode(node->right, memory)));
			find_all_read_variables_in_expr(node->right, read_variables, memory);
		}
		else
		{
			find_all_read_variables_in_expr(node->left, read_variables, memory);
			find_all_read_variables_in_expr(node->right, read_variables, memory);
		}
	}
}

bool Variable::operator<(const Variable& other) const
{
	if (this->type != other.type)
		return this->type < other.type;
	bool res;
	switch (type)
	{
	case SCALAR_VAR:
		res = (name < other.name);
		break;
	case CONCRETE_ARR_ELEMENT:
		if (name == other.name)
			res = (index < other.index);
		else
		    res = (name < other.name && index < other.index);
		break;
	case ABSTRACT_ARR_ELEMENT:
		if (name == other.name)
			res = (index_expr->toString() < other.index_expr->toString());
		else
			res = (name < other.name && index_expr->toString() < other.index_expr->toString());
		break;
	}
	return res;
}