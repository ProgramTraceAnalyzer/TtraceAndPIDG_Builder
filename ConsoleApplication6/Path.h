#pragma once
#include "ExecutionElement.h"
#include "PIDG_node.h"

#ifndef PATH_H
#define PATH_H

#pragma once
#include "ExecutionElement.h"
#include "PIDG_node.h"  // Assuming this is where PIDG_node is defined
#include <vector>
#include <set>
#include <map>
#include <string>

struct OnlyActionPath {
    std::vector<ExecutionElement> sequence;

    OnlyActionPath getAbstractedActionPath(bool abstract_const = false, bool abstract_operator = false) const;
    std::set<std::pair<int, int>> buildPIDG() const;
    std::map<int, PIDG_node*> build_PIDG_nodes() const;
    std::string buildPIDG_dot() const;
    std::string toHTML() const;
};

struct OnlyStatePath {
    std::vector<ExecutionElement> sequence;

    OnlyStatePath removeStutteringRepeats() const;
    std::map <std::string, int> get_submemory(const std::map <std::string, int>& var_values, std::set<std::string>& var_names) const;
    OnlyStatePath removeStutteringRepeats(std::set<std::string>& meaning_vars) const;
    std::string toHTML() const;
};


struct Path {
    std::vector<ExecutionElement> sequence;

    void addAction(std::string action);
    void addAction_ExecutionElement(ExecutionElement act);
    ExecutionElement addAssignAction(std::string action, std::string assign_variable, ExprNode* expr);
    ExecutionElement addSkipAction();
    ExecutionElement addLogicAction(std::string action, ExprNode* expr);
    void addState(std::map <std::string, int> var_values, int loc = -1);
    std::string toHTML() const;
    OnlyStatePath getOnlyStatePath() const;
    OnlyActionPath getOnlyActionPath() const;
};

#endif