#pragma once
#include "Path.h"

class ProgramGraph {
private:
    std::set<std::string> var_names;
    std::map<int, std::map<int, ExecutionElement>> action_matrix;
    int terminal_loc;

public:
    ProgramGraph(std::set<std::string> var_names,
        std::map<int, std::map<int, ExecutionElement>> action_matrix,
        int terminal_loc);

    ProgramGraph(std::set<std::string> var_names, int terminal_loc);

    void addAssignAction(int loc_from, int loc_to, std::string assign_variable, ExprNode* expr);
    void addLogicAction(int loc_from, int loc_to, ExprNode* expr);
    Path execute(int max_steps = 10000) const;
    std::string toDOT() const;

private:
    int do_step(int loc, std::map<std::string, int>& var_values, struct Path& path) const;
};