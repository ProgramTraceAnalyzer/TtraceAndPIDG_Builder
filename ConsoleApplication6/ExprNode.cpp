#include "ExprNode.h"
#include <stack>
#include <sstream>

ExprNode* parse_expr_from_RPN(std::string RPN) {
	std::stack<ExprNode*> s;
	std::stringstream ss(RPN);
	std::string token;

	while (ss >> token) {
		if (token == "+") {
			ExprNode* right = s.top();
			s.pop();
			ExprNode* left = s.top();
			s.pop();
			s.push(add(left, right));
		}
		else if (token == "-") {
			ExprNode* right = s.top();
			s.pop();
			ExprNode* left = s.top();
			s.pop();
			s.push(sub(left, right));
		}
		else if (token == "_+") {
			ExprNode* left = s.top();
			s.pop();
			s.push(create_unary_plus(left));
		}
		else if (token == "_-") {
			ExprNode* left = s.top();
			s.pop();
			s.push(create_unary_minus(left));
		}
		else if (token == "*") {
			ExprNode* right = s.top();
			s.pop();
			ExprNode* left = s.top();
			s.pop();
			s.push(mul(left, right));
		}
		else if (token == "/") {
			ExprNode* right = s.top();
			s.pop();
			ExprNode* left = s.top();
			s.pop();
			s.push(div(left, right));
		}
		else if (token == "%") {
			ExprNode* right = s.top();
			s.pop();
			ExprNode* left = s.top();
			s.pop();
			s.push(mod(left, right));
		}
		else if (token == "!=") {
			ExprNode* right = s.top();
			s.pop();
			ExprNode* left = s.top();
			s.pop();
			s.push(not_equal(left, right));
		}
		else if (token == "<") {
			ExprNode* right = s.top();
			s.pop();
			ExprNode* left = s.top();
			s.pop();
			s.push(less(left, right));
		}
		else if (token == ">") {
			ExprNode* right = s.top();
			s.pop();
			ExprNode* left = s.top();
			s.pop();
			s.push(greater(left, right));
		}
		else if (token == "<=") {
			ExprNode* right = s.top();
			s.pop();
			ExprNode* left = s.top();
			s.pop();
			s.push(le(left, right));
		}
		else if (token == ">=") {
			ExprNode* right = s.top();
			s.pop();
			ExprNode* left = s.top();
			s.pop();
			s.push(ge(left, right));
		}
		else if (token == "==") {
			ExprNode* right = s.top();
			s.pop();
			ExprNode* left = s.top();
			s.pop();
			s.push(eq(left, right));
		}
		else if (token == "&&") {
			ExprNode* right = s.top();
			s.pop();
			ExprNode* left = s.top();
			s.pop();
			s.push(and_op(left, right));
		}
		else if (token == "||") {
			ExprNode* right = s.top();
			s.pop();
			ExprNode* left = s.top();
			s.pop();
			s.push(or_op(left, right));
		}
		else if (token == "=") {
			ExprNode* left = s.top();
			s.pop();
			ExprNode* right = s.top();
			s.pop();
			s.push(assign(left, right));
		}
		else if (token == "!") {
			ExprNode* left = s.top();
			s.pop();
			s.push(create_not_node(left));
		}
		else if (token == "[]") {
			ExprNode* right = s.top();
			s.pop();
			ExprNode* left = s.top();
			if (left->type == VAR)
			{
				s.pop();
				s.push(get_element_by_index(left->var_name, right));
			}
		}
		else if (token == "[]=")
		{
			ExprNode* ind_expr = s.top();
			s.pop();
			ExprNode* arr_name = s.top();
			s.pop();
			ExprNode* expr = s.top();
			s.pop();
			ExprNode* arr_element_node = get_element_by_index(arr_name->var_name,ind_expr);
			s.push(assign(arr_element_node, expr));
		}
		else if (token == "?:")
		{
		ExprNode* falseBranch = s.top();
		s.pop();
		ExprNode* trueBranch = s.top();
		s.pop();
		ExprNode* condition = s.top();
		s.pop();
		ExprNode* ternar = create_ternar(condition, trueBranch, falseBranch);//arr_element_node = get_element_by_index(arr_name->var_name, ind_expr);
		s.push(ternar);
		}
		else {
			try {
				int num = std::stoi(token);
				s.push(create_int_node(num));
			}
			catch (const std::invalid_argument& ia) {
				// If it's not a number, assume it's a variable
				s.push(create_var_node(token));
			}
		}
		}
		

	if (s.size() != 1) {
		// Error: The RPN expression is invalid.  Should only have one item left on stack
		//std::cerr << "Error: Invalid RPN expression." << std::endl;
		return nullptr;
	}

	return s.top();
}

bool ExprNode::equalsTo(ExprNode* other) const
{
	if (other == NULL)
		return false;
	if (type != other->type)
		return false;
	if (type == INT && int_val == other->int_val)
		return true;
	if (type == VAR && var_name == other->var_name)
		return true;
	if (type == other->type && (type == ABSTRACT_CONST))
		return true;
	return left->equalsTo(other->left) && right->equalsTo(other->right);
}

std::string ExprNode::toString() const
{
	switch (type)
	{
	case ASSIGN_EXPR:
		return left->toString() + " = " + right->toString();
		break;
	case ADD:
		return left->toString() + " + " + right->toString();
		break;
	case UNARY_PLUS:
		return "+ (" + left->toString() + ")";
	case UNARY_MINUS:
		return "- (" + left->toString() + ")";
	case SUB:
		return "" + left->toString() + " - (" + right->toString() + ")";
		break;
	case MUL:
		return "(" + left->toString() + ") * (" + right->toString() + ")";
		break;
	case DIV:
		return "(" + left->toString() + ") / (" + right->toString() + ")";
		break;
	case MOD:
		return "(" + left->toString() + ") % (" + right->toString() + ")";
		break;
	case INT:
		return std::to_string(int_val);
		break;
	case VAR:
		return var_name;
		break;
	case NOT_EQUAL:
		return left->toString() + " != " + right->toString();
	case LESS:
		return left->toString() + " < " + right->toString();
	case GREATER:
		return left->toString() + " > " + right->toString();
	case LE:
		return left->toString() + " <= " + right->toString();
	case GE:
		return left->toString() + " >= " + right->toString();
	case EQ:
		return left->toString() + " == " + right->toString();
	case AND:
		return "(" + left->toString() + ") && (" + right->toString() + ")";
	case OR:
		return "(" + left->toString() + ") || (" + right->toString() + ")";
	case TERNAR:
		return "(" + left->toString() + ") ? ( " + right->left->toString() + " : " + right->right->toString() + ")";
	case NOT:
		return "! ("+ left->toString() +")";
	case ABSTRACT_ARITHMETIC_OP:
		return "(" + left->toString() + ") Abstract_AOP (" + right->toString() + ")";
	case ABSTRACT_BOOL_OP:
		return "(" + left->toString() + ") Abstract_BOP (" + right->toString() + ")";
	case ABSTRACT_CONST:
		return "Abstract_CONST";
	case ARRAY_INDEX:
		return left->toString() + "[" + right->toString() + "]";
	default:
		break;
	}
}

ExprNode* copy_node(ExprNode* e)
{
	ExprNode* res = new ExprNode;
	res->int_val = e->int_val;
	res->left = e->left;
	res->right = e->right;
	res->var_name = e->var_name;
	res->type = e->type;
	return res;
}

ExprNode* assign(ExprNode* left, ExprNode* right)
{
	ExprNode* res = new ExprNode;
	res->type = ASSIGN_EXPR;
	res->left = copy_node(left);
	res->right = copy_node(right);
	return res;
}

ExprNode* add(ExprNode* left, ExprNode* right)
{
	ExprNode* res = new ExprNode;
	res->type = ADD;
	res->left = copy_node(left);
	res->right = copy_node(right);
	return res;
}

ExprNode* sub(ExprNode* left, ExprNode* right)
{
	ExprNode* res = new ExprNode;
	res->type = SUB;
	res->left = left;
	res->right = right;
	return res;
}

ExprNode* create_unary_plus(ExprNode* left)
{
	ExprNode* res = new ExprNode;
	res->type = UNARY_PLUS;
	res->left = left;
	res->right = nullptr;
	return res;
}

ExprNode* create_unary_minus(ExprNode* left)
{
	ExprNode* res = new ExprNode;
	res->type = UNARY_MINUS;
	res->left = left;
	res->right = nullptr;
	return res;
}

ExprNode* mul(ExprNode* left, ExprNode* right)
{
	ExprNode* res = new ExprNode;
	res->type = MUL;
	res->left = left;
	res->right = right;
	return res;
}

ExprNode* div(ExprNode* left, ExprNode* right)
{
	ExprNode* res = new ExprNode;
	res->type = DIV;
	res->left = left;
	res->right = right;
	return res;
}

ExprNode* mod(ExprNode* left, ExprNode* right)
{
	ExprNode* res = new ExprNode;
	res->type = MOD;
	res->left = left;
	res->right = right;
	return res;
}
// LESS
ExprNode* less(ExprNode* left, ExprNode* right) {
	ExprNode* res = new ExprNode;
	res->type = LESS;
	res->left = left;
	res->right = right;
	return res;
}

// LESS
ExprNode* not_equal(ExprNode* left, ExprNode* right) {
	ExprNode* res = new ExprNode;
	res->type = NOT_EQUAL;
	res->left = left;
	res->right = right;
	return res;
}

// GREATER
ExprNode* greater(ExprNode* left, ExprNode* right) {
	ExprNode* res = new ExprNode;
	res->type = GREATER;
	res->left = left;
	res->right = right;
	return res;
}

// LE (Less or Equal)
ExprNode* le(ExprNode* left, ExprNode* right) {
	ExprNode* res = new ExprNode;
	res->type = LE;
	res->left = left;
	res->right = right;
	return res;
}

// GE (Greater or Equal)
ExprNode* ge(ExprNode* left, ExprNode* right) {
	ExprNode* res = new ExprNode;
	res->type = GE;
	res->left = left;
	res->right = right;
	return res;
}

// AND
ExprNode* and_op(ExprNode* left, ExprNode* right) {
	ExprNode* res = new ExprNode;
	res->type = AND;
	res->left = left;
	res->right = right;
	return res;
}

ExprNode* eq(ExprNode* left, ExprNode* right)
{
	ExprNode* res = new ExprNode;
	res->type = EQ;
	res->left = left;
	res->right = right;
	return res;
}

// OR
ExprNode* or_op(ExprNode* left, ExprNode* right) {
	ExprNode* res = new ExprNode;
	res->type = OR;
	res->left = left;
	res->right = right;
	return res;
}

ExprNode* create_ternar(ExprNode* condition, ExprNode* trueExpr, ExprNode* falseExpr)
{
	ExprNode* res = new ExprNode;
	res->type = TERNAR;
	res->left = condition;

	ExprNode* choise = new ExprNode;
	choise->left = trueExpr;
	choise->right = falseExpr;
	res->right = choise;
	return res;
}

ExprNode * create_not_node(ExprNode* left)
{
	ExprNode* res = new ExprNode;
	res->type = NOT;
	res->left = left;
	res->right = nullptr;
	return res;
}

ExprNode* get_element_by_index(std::string array_name, ExprNode* index_expr)
{
	ExprNode* res = new ExprNode;
	res->type = ARRAY_INDEX;
	res->left = create_var_node(array_name);
	res->right = index_expr;
	return res;
}

ExprNode* create_int_node(int val)
{
	ExprNode* res = new ExprNode;
	res->type = INT;
	res->int_val = val;
	res->left = NULL;
	res->right = NULL;
	return  res;
}

ExprNode* create_var_node(std::string var)
{
	ExprNode* res = new ExprNode;
	res->type = VAR;
	res->var_name = var;
	res->left = NULL;
	res->right = NULL;
	return  res;
}





std::string expr_node_string(ExprNode* expr)
{
	if (expr == NULL)
		return "";
	return expr->toString();
}

void findExprVariables(ExprNode* expr, std::set<std::string>& vars)
{
	if (expr)
	{
		if (expr->type == VAR)
		{
			vars.insert(expr->var_name);
		}
		if (expr->left)
		{
			findExprVariables(expr->left, vars);
		}
		if (expr->right)
		{
			findExprVariables(expr->right, vars);
		}
	}


}

ExprNode* get_abstract_expr_node(ExprNode* expr, bool abstract_const, bool abstract_operator)
{
	if (expr == NULL)
		return NULL;

	ExprNode* res = new ExprNode;

	if (abstract_const && expr->type == INT)
	{
		res->type = ABSTRACT_CONST;
		res->left = NULL;
		res->right = NULL;
	}
	else if (abstract_operator && (expr->type == ADD || expr->type == SUB || expr->type == MUL || expr->type == DIV))
	{
		res->type = ABSTRACT_ARITHMETIC_OP;
		res->left = get_abstract_expr_node(expr->left, abstract_const, abstract_operator);
		res->right = get_abstract_expr_node(expr->right, abstract_const, abstract_operator);
	}
	else if (abstract_operator && (expr->type == AND || expr->type == OR || expr->type == NOT || expr->type == LESS || expr->type == GREATER || expr->type == GE || expr->type == LE || expr->type == EQ || expr->type == NOT))
	{
		res->type = ABSTRACT_BOOL_OP;
		res->left = get_abstract_expr_node(expr->left, abstract_const, abstract_operator);
		res->right = get_abstract_expr_node(expr->right, abstract_const, abstract_operator);
	}
	else
	{
		res->type = expr->type;
		res->var_name = expr->var_name;
		res->int_val = expr->int_val;
		res->left = get_abstract_expr_node(expr->left, abstract_const, abstract_operator);
		res->right = get_abstract_expr_node(expr->right, abstract_const, abstract_operator);
	}

	return res;
}

int calcExprNode(ExprNode* node, std::map<std::string, int>& var_values) {
	switch (node->type) {
	case ADD:
		return calcExprNode(node->left, var_values) + calcExprNode(node->right, var_values);
	case SUB:
		return calcExprNode(node->left, var_values) - calcExprNode(node->right, var_values);
	case UNARY_PLUS:
		return calcExprNode(node->left, var_values);
	case UNARY_MINUS:
		return -calcExprNode(node->left, var_values);
	case MUL:
		return calcExprNode(node->left, var_values) * calcExprNode(node->right, var_values);
	case DIV: {
		int divisor = calcExprNode(node->right, var_values);
		if (divisor == 0) {
			// Обработка деления на ноль.  Можно выбросить исключение, вернуть
			// какое-то специальное значение или вывести сообщение об ошибке.
			// В данном случае возвращаем 0.
			//std::cerr << "Error: Division by zero!" << std::endl;
			return 0;
		}
		return calcExprNode(node->left, var_values) / divisor;
	}
	case NOT_EQUAL:
		return calcExprNode(node->left, var_values) != calcExprNode(node->right, var_values);
	case EQ:
		return calcExprNode(node->left, var_values) == calcExprNode(node->right, var_values);
	case LESS:
		return calcExprNode(node->left, var_values) < calcExprNode(node->right, var_values);
	case GREATER:
		return calcExprNode(node->left, var_values) > calcExprNode(node->right, var_values);
	case LE:
		return calcExprNode(node->left, var_values) <= calcExprNode(node->right, var_values);
	case GE:
		return calcExprNode(node->left, var_values) >= calcExprNode(node->right, var_values);
	case AND:
		return calcExprNode(node->left, var_values) && calcExprNode(node->right, var_values);
	case OR:
		return calcExprNode(node->left, var_values) || calcExprNode(node->right, var_values);
	case NOT:
		// NOT - унарный оператор, нужно убедиться, что есть только один дочерний узел
		if (node->right != nullptr) {
			//std::cerr << "Error: NOT operator should only have one operand!" << std::endl;
			return 0; // Или другое подходящее значение по умолчанию/обработка ошибки
		}
		return !calcExprNode(node->left, var_values);
	case INT:
		return node->int_val;
	case VAR: {
		std::string var_name = node->var_name;
		if (var_values.count(var_name) == 0) {
			// Обработка случая, когда переменная не найдена.  Можно выбросить
			// исключение, вернуть какое-то специальное значение или вывести
			// сообщение об ошибке.  В данном случае возвращаем 0.
			//std::cerr << "Error: Variable '" << var_name << "' not found!" << std::endl;
			return 0;
		}
		return var_values[var_name];
	}
	default:
		// Обработка неизвестного типа узла.
		//std::cerr << "Error: Unknown node type!" << std::endl;
		return 0;
	}
}


int calcExprNode(ExprNode* node, const Memory& memory)
{
	if (node->type == ARRAY_INDEX)
	{
		return memory.arrayMemory.array_values.at(node->left->var_name).index_values.at(calcExprNode(node->right, memory));
	}

	switch (node->type) {
	case UNARY_PLUS:
		return calcExprNode(node->left, memory);
	case UNARY_MINUS:
		return -calcExprNode(node->left, memory);
	case ADD:
		return calcExprNode(node->left, memory) + calcExprNode(node->right, memory);
	case SUB:
		return calcExprNode(node->left, memory) - calcExprNode(node->right, memory);
	case MUL:
		return calcExprNode(node->left, memory) * calcExprNode(node->right, memory);
	case DIV: {
		int divisor = calcExprNode(node->right, memory);
		if (divisor == 0) {
			// Обработка деления на ноль.  Можно выбросить исключение, вернуть
			// какое-то специальное значение или вывести сообщение об ошибке.
			// В данном случае возвращаем 0.
			//std::cerr << "Error: Division by zero!" << std::endl;
			return 0;
		}
		return calcExprNode(node->left, memory) / divisor;
	}
	case MOD: {
		int divisor = calcExprNode(node->right, memory);
		if (divisor == 0) {
			// Обработка деления на ноль.  Можно выбросить исключение, вернуть
			// какое-то специальное значение или вывести сообщение об ошибке.
			// В данном случае возвращаем 0.
			//std::cerr << "Error: Division by zero!" << std::endl;
			return 0;
		}
		return calcExprNode(node->left, memory) % divisor;
	}
	case EQ:
		return calcExprNode(node->left, memory) == calcExprNode(node->right, memory);
	case NOT_EQUAL:
		return calcExprNode(node->left, memory) != calcExprNode(node->right, memory);
	case LESS:
		return calcExprNode(node->left, memory) < calcExprNode(node->right, memory);
	case GREATER:
		return calcExprNode(node->left, memory) > calcExprNode(node->right, memory);
	case LE:
		return calcExprNode(node->left, memory) <= calcExprNode(node->right, memory);
	case GE:
		return calcExprNode(node->left, memory) >= calcExprNode(node->right, memory);
	case AND:
		return calcExprNode(node->left, memory) && calcExprNode(node->right, memory);
	case OR:
		return calcExprNode(node->left, memory) || calcExprNode(node->right, memory);
	case TERNAR:
		return calcExprNode(node->left, memory) ? calcExprNode(node->right->left, memory) : calcExprNode(node->right->right, memory);
	case NOT:
		// NOT - унарный оператор, нужно убедиться, что есть только один дочерний узел
		if (node->right != nullptr) {
			//std::cerr << "Error: NOT operator should only have one operand!" << std::endl;
			return 0; // Или другое подходящее значение по умолчанию/обработка ошибки
		}
		return !calcExprNode(node->left, memory);
	case INT:
		return node->int_val;
	case VAR: {
		std::string var_name = node->var_name;
		if (memory.scalarMemory.var_values.count(var_name) == 0) {
			// Обработка случая, когда переменная не найдена.  Можно выбросить
			// исключение, вернуть какое-то специальное значение или вывести
			// сообщение об ошибке.  В данном случае возвращаем 0.
			//std::cerr << "Error: Variable '" << var_name << "' not found!" << std::endl;
			return 0;
		}
		return memory.scalarMemory.var_values.at(var_name);
	}
	default:
		// Обработка неизвестного типа узла.
		//std::cerr << "Error: Unknown node type!" << std::endl;
		return 0;
	}

}