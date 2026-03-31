#include "StateSequence.h"

std::string StateSequence::to_html() const
{
	std::string html = "<table>";
	for (auto it = this->states.cbegin(); it != this->states.cend(); it++)
	{
		int num = it - this->states.cbegin();
		html += "<tr><th>" + std::to_string(num) + "</th><td>" + it->to_html() + "</td></tr>";
	}
	html += "</table>";
	return html;
}

std::string StateSequence::to_json() const
{
	std::string json;
	for (auto it = this->states.cbegin(); it != this->states.cend(); it++)
	{
		if (it != this->states.cbegin())
			json += ",";
		json += it->to_json();
	}
	json = "[" + json + "]";
	return json;
}

MemorySequence StateSequence::get_memory_sequence() const
{
	MemorySequence ms = {};
	for (auto it = this->states.cbegin(); it != this->states.cend(); it++)
	{
		ms.memory_seq.push_back(it->memory);
	}
	return ms;
}