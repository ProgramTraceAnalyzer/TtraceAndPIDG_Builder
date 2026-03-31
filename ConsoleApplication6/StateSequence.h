#pragma once
#include "State.h"
#include "MemorySequence.h"

struct StateSequence
{
	std::vector<State> states;
	std::string to_html() const;
	std::string to_json() const;
	MemorySequence get_memory_sequence() const;
};