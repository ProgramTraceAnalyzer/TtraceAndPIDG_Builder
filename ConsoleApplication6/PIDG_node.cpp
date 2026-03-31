#include "PIDG_node.h"

std::string PIDG_node::to_string() const
{
	return assign_var + " = " + expr->toString();
}

bool PIDG_node::equals_to(PIDG_node* other) const
{
	if (assign_var != other->assign_var)
		return false;
	if (assign_var == assign_var && expr->equalsTo(other->expr))
	{
		for (auto it = parent_nodes_by_var.cbegin(); it != parent_nodes_by_var.cend(); it++)
		{
			if (other->parent_nodes_by_var.count(it->first) == 0)
				return false;
			if (!parent_nodes_by_var.at(it->first)->equals_to(other->parent_nodes_by_var.at(it->first)))
			{
				return false;
			}
		}
		for (auto it = other->parent_nodes_by_var.cbegin(); it != other->parent_nodes_by_var.cend(); it++)
		{
			if (parent_nodes_by_var.count(it->first) == 0)
				return false;
		}
		return true;
	}
	return false;
}

PIDG_node* get_abstract_PIDG_node(PIDG_node* node, bool abstract_const , bool abstract_operator )
{
	PIDG_node* abstracted_pidg_node = new PIDG_node;
	abstracted_pidg_node->assign_var = node->assign_var;
	abstracted_pidg_node->id = node->id;
	abstracted_pidg_node->is_final = node->is_final;
	for (auto it = node->parent_nodes_by_var.cbegin(); it != node->parent_nodes_by_var.cend(); it++)
	{
		abstracted_pidg_node->parent_nodes_by_var[it->first] = get_abstract_PIDG_node(it->second, abstract_const, abstract_operator);
	}
	abstracted_pidg_node->expr = get_abstract_expr_node(node->expr, abstract_const, abstract_operator);
	return abstracted_pidg_node;
}