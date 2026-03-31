#pragma once
#include "Memory.h"
//#include "Variable.h"

enum exprType { ASSIGN_EXPR, ADD, SUB, UNARY_PLUS, UNARY_MINUS, MUL, DIV, MOD, EQ, NOT_EQUAL, TERNAR, CHOISE, LESS, GREATER, LE, GE, AND, OR, NOT, INT, VAR, ABSTRACT_ARITHMETIC_OP, ABSTRACT_BOOL_OP, ABSTRACT_CONST, ARRAY_INDEX };

struct ExprNode
{
	exprType type;
	ExprNode* left;
	ExprNode* right;
	int int_val;
	std::string var_name;

	bool equalsTo(ExprNode* other) const;
	std::string toString() const;
};
ExprNode* parse_expr_from_RPN(std::string RPN);
ExprNode* copy_node(ExprNode* e);
ExprNode* assign(ExprNode* left, ExprNode* right);
ExprNode* create_unary_minus(ExprNode* left);
ExprNode* create_unary_plus(ExprNode* left);
ExprNode* add(ExprNode* left, ExprNode* right);
ExprNode* sub(ExprNode* left, ExprNode* right);
ExprNode* mul(ExprNode* left, ExprNode* right);
ExprNode* div(ExprNode* left, ExprNode* right);
ExprNode* mod(ExprNode* left, ExprNode* right);
ExprNode* not_equal(ExprNode* left, ExprNode* right);
ExprNode* less(ExprNode* left, ExprNode* right);
ExprNode* greater(ExprNode* left, ExprNode* right);
ExprNode* le(ExprNode* left, ExprNode* right);
ExprNode* ge(ExprNode* left, ExprNode* right);
ExprNode* eq(ExprNode* left, ExprNode* right);
ExprNode* and_op(ExprNode* left, ExprNode* right);
ExprNode* or_op(ExprNode* left, ExprNode* right);
ExprNode* create_ternar(ExprNode* condition, ExprNode* trueExpr, ExprNode* falseExpr);
ExprNode* create_not_node(ExprNode* left);
ExprNode* get_element_by_index(std::string array_name, ExprNode* index_expr);
ExprNode* create_int_node(int val);
ExprNode* create_var_node(std::string var);
std::string expr_node_string(ExprNode* expr);
void findExprVariables(ExprNode* expr, std::set<std::string>& vars);
ExprNode* get_abstract_expr_node(ExprNode* expr, bool abstract_const = false, bool abstract_operator = false);
int calcExprNode(ExprNode* node, std::map<std::string, int>& var_values);
int calcExprNode(ExprNode* node, const Memory & memory);