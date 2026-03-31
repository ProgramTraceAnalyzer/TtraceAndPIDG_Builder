#pragma once
#include "Action.h"

struct ActionSequence
{
	std::vector<Action> action_seq;
	std::string to_html() const;
	std::string to_json() const;	
};