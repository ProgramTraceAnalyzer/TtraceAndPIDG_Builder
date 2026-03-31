#include "Path.h"

OnlyActionPath OnlyActionPath::getAbstractedActionPath(bool abstract_const, bool abstract_operator) const {
    std::vector<ExecutionElement> new_sequence;
    for (auto it = sequence.cbegin(); it != sequence.cend(); it++) {
        new_sequence.push_back(it->getAbstractedAction(abstract_const, abstract_operator));
    }
    OnlyActionPath abstracted_action_path = { new_sequence };
    return abstracted_action_path;
}

std::set<std::pair<int, int>> OnlyActionPath::buildPIDG() const {
    std::set<std::pair<int, int>> pidg;
    int size = sequence.size();
    for (int i = 0; i < size; i++) {
        std::set<std::string> read_vars = sequence[i].findReadVariables();
        for (auto it = read_vars.cbegin(); it != read_vars.cend(); it++) {
            bool found = false;
            for (int j = i - 1; j >= 0 && !found; j--) {
                if (sequence[j].assigns_var(*it)) {
                    std::pair<int, int> edge{ i, j };
                    pidg.insert(edge);
                    found = true;
                }
            }
        }
    }
    return pidg;
}

std::map<int, PIDG_node*> OnlyActionPath::build_PIDG_nodes() const {
    std::map<int, PIDG_node*> node_map;
    for (auto it = sequence.cbegin(); it != sequence.cend(); it++) {
        int id = it - sequence.cbegin();
        PIDG_node* node = new PIDG_node;
        node->expr = it->assign_expr;
        node->id = id;
        node_map[id] = node;
        std::map<std::string, PIDG_node*> parent_nodes;
        node->parent_nodes_by_var = parent_nodes;
        if (it->act_type == ASSIGN) {
            node->assign_var = it->assign_var;
        }
    }

    std::set<std::pair<int, int>> pidg = buildPIDG();
    for (auto it = pidg.cbegin(); it != pidg.cend(); it++) {
        std::pair<int, int> edge = *it;
        std::string assign_var = node_map.at(edge.second)->assign_var;
        node_map.at(edge.first)->parent_nodes_by_var[assign_var] = node_map[edge.second];
    }

    return node_map;
}

std::string OnlyActionPath::buildPIDG_dot() const {
    std::string  dot = "";
    std::set<std::pair<int, int>> pairs = buildPIDG();
    for (auto it = sequence.cbegin(); it != sequence.cend(); it++) {
        dot += std::to_string(it - sequence.cbegin()) + "[label=\"" + it->action_string + "\"]\n";
    }
    for (auto it = pairs.cbegin(); it != pairs.cend(); it++) {
        dot += std::to_string(it->first) + " -> " + std::to_string(it->second) + "\n";
    }
    dot = "digraph G {" + dot + "}";
    return dot;
}

std::string OnlyActionPath::toHTML() const {
    std::string html = "";
    int act_counter = 0;
    int state_counter = 0;
    for (auto it = sequence.cbegin(); it != sequence.cend(); it++) {
        ExecutionElement el = *it;
        std::string sign = ((el.type == ACTION) ? "A" + std::to_string(++act_counter) : "S" + std::to_string(++state_counter));

        ActionRWs rws = el.getActionRWs();

        html += "<tr><td>" + sign + "</td><td>" + el.toHTML() + "</td><td>" + rws.toHTML() + "</td></tr>";

    }
    html = "<table class=\"table table-striped\" border=\"l\">" + html + "</table>";
    return html;
}

OnlyStatePath OnlyStatePath::removeStutteringRepeats() const {
    std::vector<ExecutionElement> result;
    result.push_back(sequence[0]);

    for (size_t i = 1; i < sequence.size(); ++i) {
        if (sequence[i].var_values != result.back().var_values) {
            ExecutionElement curr_el = sequence[i];
            curr_el.loc = -1;
            result.push_back(curr_el);
        }
    }

    OnlyStatePath path = { result };
    return path;
}

std::map <std::string, int> OnlyStatePath::get_submemory(const std::map <std::string, int>& var_values, std::set<std::string>& var_names) const {
    std::map <std::string, int> submemory;
    for (auto it = var_names.cbegin(); it != var_names.cend(); it++) {
        if (var_values.count(*it))
            submemory[*it] = var_values.at(*it);
    }
    return submemory;
}

OnlyStatePath OnlyStatePath::removeStutteringRepeats(std::set<std::string>& meaning_vars) const {
    std::vector<ExecutionElement> result;
    ExecutionElement seq0 = sequence[0];
    seq0.var_values = get_submemory(seq0.var_values, meaning_vars);
    result.push_back(seq0);

    for (auto it = sequence.cbegin(); it != sequence.cend(); ++it) {
        std::map <std::string, int> submemory = get_submemory(it->var_values, meaning_vars);
        std::map <std::string, int> back_submemory = get_submemory((result.cend() - 1)->var_values, meaning_vars);
        if (submemory != back_submemory) {
            ExecutionElement curr_el;
            curr_el.loc = -1;
            curr_el.type = STATE;
            curr_el.var_values = submemory;
            result.push_back(curr_el);
        }
    }
    OnlyStatePath path = { result };
    return path;
}

std::string OnlyStatePath::toHTML() const {
    std::string html = "";
    int act_counter = 0;
    int state_counter = 0;
    for (auto it = sequence.cbegin(); it != sequence.cend(); it++) {
        ExecutionElement el = *it;
        std::string sign = ((el.type == ACTION) ? "A" + std::to_string(++act_counter) : "S" + std::to_string(++state_counter));
        html += "<tr><td>" + sign + "</td><td>" + el.toHTML() + "</td></tr>";

    }
    html = "<table class=\"table table-striped\" border=\"l\">" + html + "</table>";
    return html;
}

void Path::addAction(std::string action) {
    ExecutionElement act = { ACTION,{},action, ASSIGN, "", nullptr }; // Changed to use default values
    this->sequence.push_back(act);
}

void Path::addAction_ExecutionElement(ExecutionElement act) {
    this->sequence.push_back(act);
}

ExecutionElement Path::addAssignAction(std::string action, std::string assign_variable, ExprNode* expr) {
    ExecutionElement act = { ACTION,{},action, ASSIGN, assign_variable, expr };
    this->sequence.push_back(act);
    return act;
}

ExecutionElement Path::addSkipAction() {
    ExecutionElement act = { ACTION,{},"skip", SKIP, "", nullptr }; // Changed to use default values
    this->sequence.push_back(act);
    return act;
}

ExecutionElement Path::addLogicAction(std::string action, ExprNode* expr) {
    ExecutionElement act = { ACTION,{},action, LOGIC, "", expr };
    this->sequence.push_back(act);
    return act;
}

void Path::addState(std::map <std::string, int> var_values, int loc) {
    ExecutionElement state = { STATE, var_values , "" }; // Fixed the constructor call
    state.loc = loc;
    this->sequence.push_back(state);
}

std::string Path::toHTML() const {
    std::string html = "";
    int act_counter = 0;
    int state_counter = 0;
    for (auto it = sequence.cbegin(); it != sequence.cend(); it++) {
        ExecutionElement el = *it;
        std::string sign = ((el.type == ACTION) ? "A" + std::to_string(++act_counter) : "S" + std::to_string(++state_counter));
        html += "<tr><td>" + sign + "</td><td>" + el.toHTML() + "</td></tr>";
    }
    html = "<table class=\"table table-striped\" border=\"l\">" + html + "</table>";
    return html;
}

OnlyStatePath Path::getOnlyStatePath() const {
    std::vector <ExecutionElement> only_state_sequence;
    for (auto it = sequence.cbegin(); it != sequence.cend(); it++) {
        if (it->type == STATE) {
            only_state_sequence.push_back(*it);
        }
    }
    OnlyStatePath res = { only_state_sequence };
    return res;
}

OnlyActionPath Path::getOnlyActionPath() const {
    std::vector <ExecutionElement> only_state_sequence;
    for (auto it = sequence.cbegin(); it != sequence.cend(); it++) {
        if (it->type == ACTION) {
            only_state_sequence.push_back(*it);
        }
    }
    OnlyActionPath res = { only_state_sequence };
    return res;
}