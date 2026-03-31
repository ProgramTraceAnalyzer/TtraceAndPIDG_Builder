#pragma once
#include "Memory.h"
struct State
{
	int location;
	Memory memory;
	std::string to_html() const;
	std::string to_json() const;
};