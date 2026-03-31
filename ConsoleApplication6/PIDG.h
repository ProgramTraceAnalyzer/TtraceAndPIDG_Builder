#pragma once
#include "PIDG_node.h"
#include "ExecutionElement.h"

class PIDG
{
private:
	std::map<int, std::map<int, ExecutionElement>> adjacencyMatrix;
public:
	PIDG(std::set<std::pair<int, int>> edges);
};