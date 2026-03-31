#include "MemorySequence.h"

MemorySequence MemorySequence::get_submemory_sequence(std::set<std::string> variables) const
{
	MemorySequence res = {};
	for (auto it = memory_seq.cbegin(); it != memory_seq.cend(); it++)
	{
		res.memory_seq.push_back(it->get_submemorty(variables));
	}
	return res;
}

MemorySequence MemorySequence::get_without_stutter_steps() const
{
	MemorySequence res = {};
	res.memory_seq.push_back(memory_seq.at(0));
	for (auto it = memory_seq.cbegin(); it != memory_seq.cend(); it++)
	{
		if (!(*it == res.memory_seq.back()))
			res.memory_seq.push_back(*it);
	}
	return res;
}

std::string MemorySequence::to_html() const
{
	std::string html = "<table class=\"table table-striped\">";
	for (auto it = this->memory_seq.cbegin(); it != this->memory_seq.cend(); it++)
	{
		int num = it - this->memory_seq.cbegin();
		html += "<tr><th>" + std::to_string(num) + "</th><td>" + it->to_html() + "</td></tr>";
	}
	html += "</table>";
	return html;
}