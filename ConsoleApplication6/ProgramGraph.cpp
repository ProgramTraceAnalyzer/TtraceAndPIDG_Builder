#include "ProgramGraph.h"

ProgramGraph::ProgramGraph(std::set<std::string> var_names,
    std::map<int, std::map<int, ExecutionElement>> action_matrix,
    int terminal_loc) {
    this->var_names = var_names;
    this->action_matrix = action_matrix;
    this->terminal_loc = terminal_loc;
}

ProgramGraph::ProgramGraph(std::set<std::string> var_names, int terminal_loc) {
    this->var_names = var_names;
    this->terminal_loc = terminal_loc;
}

void ProgramGraph::addAssignAction(int loc_from, int loc_to, std::string assign_variable, ExprNode* expr) {
    ExecutionElement act = { ACTION,{},assign_variable + "=" + expr->toString(), ASSIGN, assign_variable, expr };
    action_matrix[loc_from][loc_to] = act;
}

void ProgramGraph::addLogicAction(int loc_from, int loc_to, ExprNode* expr) {
    ExecutionElement act = { ACTION,{},expr->toString(), LOGIC, "", expr };
    action_matrix[loc_from][loc_to] = act;
}

Path ProgramGraph::execute(int max_steps) const {
    struct Path path;
    std::map<std::string, int> var_values;
    path.addState(var_values, 0);
    int currentLoc = 0;
    int step_count = 0;
    while (currentLoc != terminal_loc && step_count != max_steps) {
        currentLoc = do_step(currentLoc, var_values, path);
        if (currentLoc == -1)
            return path;
        step_count++;
    }
    return path;
}

int ProgramGraph::do_step(int loc, std::map<std::string, int>& var_values, struct Path& path) const {
    auto it = action_matrix.find(loc);
    if (it == action_matrix.end()) {
        return -1; // No actions from this location
    }

    const std::map<int, ExecutionElement> action_variants = it->second;
    for (auto it = action_variants.cbegin(); it != action_variants.cend(); it++) {
        int new_loc = it->first;
        ExecutionElement act = it->second;
        if (act.executeAction(var_values)) {
            path.addAction_ExecutionElement(act);
            path.addState(var_values, new_loc);
            return new_loc;
        }
    }
    return -1;
}

std::string ProgramGraph::toDOT() const {
    std::string dot = "";
    for (auto it = action_matrix.cbegin(); it != action_matrix.cend(); it++) {
        int from_loc = it->first;
        std::map<int, ExecutionElement> map_to = it->second;
        for (auto it2 = map_to.cbegin(); it2 != map_to.cend(); it2++) {
            int to_loc = it2->first;
            ExecutionElement act = it2->second;
            dot += std::to_string(from_loc) + " -> " + std::to_string(to_loc) + " [label=\"" + act.toHTML() + "\"]\n";
        }
    }
    dot = "digraph G { " + dot + "}";
    return dot;
}