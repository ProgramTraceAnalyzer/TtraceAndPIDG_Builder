#include "Action.h"

bool Action::executeAction(Memory& memory, Action* action_finger_print) const
{
    if (action_finger_print)
    {
        action_finger_print->type = this->type;
        action_finger_print->action_expr = copy_node(this->action_expr);
    }
    
    switch (type) {
    case ASSIGN:
        
        if (this->assigned_variable.type == ABSTRACT_ARR_ELEMENT)
        {
            int index = calcExprNode(this->assigned_variable.index_expr, memory);
            int calc = calcExprNode(this->action_expr, memory);
            memory.arrayMemory.array_values[assigned_variable.name].index_values[index] = calc;
            if (action_finger_print)
                action_finger_print->assigned_variable = { CONCRETE_ARR_ELEMENT, assigned_variable.name , index, copy_node(this->assigned_variable.index_expr)};
            return true;
        }
        else if (this->assigned_variable.type == SCALAR_VAR)
        {
            int calc = calcExprNode(this->action_expr, memory);
            memory.scalarMemory.var_values[assigned_variable.name] = calc;
            if (action_finger_print)
                action_finger_print->assigned_variable = { SCALAR_VAR, assigned_variable.name};
        }
        
        //var_values[assign_var] = calcExprNode(assign_expr, var_values);
        return true;
    case LOGIC:
        return calcExprNode(this->action_expr,memory);
    default:
        break;
    }
    return false;
}

std::string Variable::to_string() const
{
    std::string str = "";
    switch (this->type)
    {
    case SCALAR_VAR:
        str = name;
        break;
    case CONCRETE_ARR_ELEMENT:
        str = name + "[" + std::to_string(index) + "]";
        break;
    case ABSTRACT_ARR_ELEMENT:
        str = name + "[" + index_expr->toString() + "]";
        break;
    default:
        break;
    }
    return str;
}

std::string Action::to_html() const
{
    std::string html = "<table style=\"margin: 0px; padding: 0px;\">";
    switch (type)
    {
    case ASSIGN:
        //html += "<tr><th>ASSIGN</th><th>EXPR<th></tr>";
        //html += "<tr><td>"+this->assigned_variable.to_string()+"</td><td>"+this->action_expr->toString()+"</td></tr>";
        html += "<tr><th>ASSIGN</th><td>" + assigned_variable.to_string() + "=" + this->action_expr->toString() + "</td></tr>";
        break;
    case LOGIC:
        //html += "<tr><th>LOGIC</th><th>EXPR<th></tr>";
        //html += "<tr><td></td><td>" + this->action_expr->toString() + "</td></tr>";
        html += "<tr><th>LOGIC_EXPR</th><td>"+ this->action_expr->toString() +"</td></tr>";
        break;
    }
    html += "</table>";
    return html;
}

std::string Action::to_json() const
{
    std::string json;
    switch (type)
    {
    case ASSIGN:
        //html += "<tr><th>ASSIGN</th><th>EXPR<th></tr>";
        //html += "<tr><td>"+this->assigned_variable.to_string()+"</td><td>"+this->action_expr->toString()+"</td></tr>";
        json += "\"type\":\"assign\", \"assigned_variable\":\"" + assigned_variable.to_string() + "\", \"expression\":\"" + this->action_expr->toString() + "\"";
        break;
    case LOGIC:
        //html += "<tr><th>LOGIC</th><th>EXPR<th></tr>";
        //html += "<tr><td></td><td>" + this->action_expr->toString() + "</td></tr>";
        json += "\"type\":\"logic\", \"expression\":\"" + this->action_expr->toString() + "\"";
        break;
    }
    return "{" + json + "}";
}


Action create_assign_action(Variable& var, ExprNode* assign_expr)
{
    Action act = {ASSIGN, copy_node(assign_expr), var};
    return act;
}

Action create_logic_action(ExprNode* logic_expr)
{
    Action act = { LOGIC, copy_node(logic_expr) };
    return act;
}

Action create_skip_action()
{
    Action act = { SKIP };
    return act;
}

std::string Action::to_string(bool add_effect) const
{
    std::string str = "";
    switch (this->type)
    {
    case ASSIGN:
        str = this->assigned_variable.to_string() + " = " + this->action_expr->toString();
        if (add_effect)
            str += "\\n" + this->assigned_variable.to_string() + " -> " + std::to_string(this->changed_value);
        break;
    case LOGIC:
        str = this->action_expr->toString();
        break;
    case SKIP:
        str = "skip";
        break;
    }
    return str;
}