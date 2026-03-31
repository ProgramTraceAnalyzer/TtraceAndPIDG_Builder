#pragma once
#include "Memory.h"
#include <vector>
struct MemorySequence
{
	std::vector<Memory> memory_seq;
	MemorySequence get_submemory_sequence(std::set<std::string> variables) const;
	MemorySequence get_without_stutter_steps() const;
	std::string to_html() const;
};