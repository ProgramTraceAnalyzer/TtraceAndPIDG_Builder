#include "ExecutionElement.h"

std::string ActionRWs::toHTML() const
{
	std::string html = "";
	std::string write = "", read = "";
	for (auto it = read_vars.cbegin(); it != read_vars.cend(); it++)
	{
		read += *it + " ";
	}
	for (auto it = write_vars.cbegin(); it != write_vars.cend(); it++)
	{
		write += *it + " ";
	}
	html = "<p>READ {" + read + "}</p> <p>WRITE {" + write + "}</p>";
	return html;
}

std::set<std::string> ExecutionElement::findReadVariables() const {
    std::set<std::string> vars;
    findExprVariables(assign_expr, vars);
    return vars;
}

bool ExecutionElement::executeAction(std::map<std::string, int>& var_values) {
    if (type == ACTION) {
        switch (act_type) {
        case ASSIGN:
            var_values[assign_var] = calcExprNode(assign_expr, var_values);
            return true;
        case LOGIC:
            return calcExprNode(assign_expr, var_values);
        default:
            break;
        }
    }
    return false; // Added return statement for completeness
}

bool ExecutionElement::assigns_var(std::string var_name) const {
    return type == ACTION && act_type == ASSIGN && assign_var == var_name;
}

std::string ExecutionElement::toHTML() const {
    std::string html = "";
    switch (type) {
    case ACTION:
        switch (act_type) {
        case ASSIGN:
            return "<p>" + assign_var + " = " + assign_expr->toString() + "</p>";
        case LOGIC:
            return "<p>" + assign_expr->toString() + "</p>";
        case SKIP:
            return "<p>skip</p>";
        }
        break;
    case STATE:
    {
        std::string first_row = "";
        std::string second_row = "";
        for (auto it = var_values.cbegin(); it != var_values.cend(); it++) {
            first_row += "<td>" + ((it->first)) + "</td>";
            second_row += "<td>" + std::to_string(((it->second))) + "</td>";
        }
        first_row = "<tr> <td>loc</td>" + first_row + "</tr>";
        second_row = "<tr> <td>" + std::to_string(loc) + "</td>" + second_row + "</tr>";
        html = "<table class=\"table table-striped\"  border=\"l\">" + first_row + second_row + "</table>";
        break;
    }
    }
    return html;
}

ActionRWs ExecutionElement::getActionRWs() const {
    std::set<std::string> write;
    if (act_type == ASSIGN)
        write.insert(assign_var);
    ActionRWs arws = { findReadVariables(), write, act_type };
    return arws;
}

ExecutionElement ExecutionElement::getAbstractedAction(bool abstract_const, bool abstract_operator) const {
    ExecutionElement e;
    if (type == ACTION) {
        e.type = type;
        e.act_type = act_type;
        e.assign_var = assign_var;
        e.assign_expr = get_abstract_expr_node(assign_expr, abstract_const, abstract_operator);
    }
    return e;
}