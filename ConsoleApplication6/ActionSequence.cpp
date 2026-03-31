#include "ActionSequence.h"

std::string ActionSequence::to_html() const
{
	std::string html = "<table>";
	for (auto it = this->action_seq.cbegin(); it != this->action_seq.cend(); it++)
	{
		int num = it - this->action_seq.cbegin();
		html += "<tr><th>" + std::to_string(num) + "</th><td>" + it->to_html() + "</td></tr>";
	}
	html += "</table>";
	return html;
}

std::string ActionSequence::to_json() const
{
	std::string json = "[";
	for (auto it = this->action_seq.cbegin(); it != this->action_seq.cend(); it++)
	{
		if (it != this->action_seq.cbegin())
			json += ",";
		json += it->to_json();
		//int num = it - this->action_seq.cbegin();
		//json += "<tr><th>" + std::to_string(num) + "</th><td>" + it->to_html() + "</td></tr>";
	}
	json += "]";
	return json;
}
