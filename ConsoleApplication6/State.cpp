#include "State.h"

std::string State::to_html() const
{
	//std::string html = "<table><tr><th>Location</th><td>" + std::to_string(location) + "</td></tr><tr><th>Memory</th><td>" + memory.to_html() + "</td></tr></table>";
	std::string html = "<table style=\"margin: 0px; padding: 0px;\" class=\"table\"><tr><th>Location</th><th>Scalar Memory</th><th>Array Memory</th></tr> <tr><td>"+ std::to_string(location) +"</td><td>"+ memory.scalarMemory.to_html() +"</td><td>"+ memory.arrayMemory.to_html() +"</td></tr></table>";
	return html;
}

std::string State::to_json() const
{
	std::string json = "{\"location\":" + std::to_string(this->location)+",\"memory\":" + this->memory.to_json()+"}";
	return json;
}