#include "Memory.h"

ScalarMemory ScalarMemory::get_submemorty(std::set<std::string> variable_names) const
{
	std::map<std::string, int> values;
	
	for (auto it = var_values.cbegin(); it != var_values.cend(); it++)
	{
		if (variable_names.count(it->first))
			values[it->first] = it->second;
	}
	ScalarMemory submemory = {values};
	return submemory;
}

ArrayMemory ArrayMemory::get_submemorty(std::set<std::string> variable_names) const
{
	std::map<std::string, Array> values;
	for (auto it = array_values.cbegin(); it != array_values.cend(); it++)
	{
		if (variable_names.count(it->first))
			values[it->first] = it->second;
	}
	ArrayMemory submemory = { values };
	return submemory;
}

Memory Memory::get_submemorty(std::set<std::string> variable_names) const
{
	Memory submemory = {scalarMemory.get_submemorty(variable_names), arrayMemory.get_submemorty(variable_names)};
	return submemory;
}

std::string ScalarMemory::to_html() const
{
	std::string html = "<table style=\"margin: 0px; padding: 0px;\" class=\"table\">",tr1,tr2;
	for (auto it = var_values.cbegin(); it != var_values.cend(); it++)
	{
		tr1 += "<th style=\"margin: 0px; padding: 0px;\" >" + it->first + "</th>";
		tr2 += "<td style=\"margin: 0px; padding: 0px;\">" + std::to_string(it->second) + "</td>";
	}
	html += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></table>";
	return html;
}

std::string ScalarMemory::to_json() const
{
	std::string json = "";
	for (auto it = var_values.cbegin(); it != var_values.cend(); ++it) {
		if (it != var_values.cbegin()) json += ",";
		json += "\"";
		json += it->first;
		json += "\":";
		json += std::to_string(it->second);
	}
	json = "{" + json + "}";
	return json;
}

bool ScalarMemory::equals_to(const ScalarMemory& other) const
{
	return this->var_values == other.var_values;
}

bool ScalarMemory::operator==(const ScalarMemory& other) const
{
	return equals_to(other);
}

bool Array::equals_to(const Array& other) const
{
	return this->size == other.size && this->index_values == other.index_values;
}

bool Array::operator==(const Array& other) const
{
	return equals_to(other);
}

bool ArrayMemory::operator==(const ArrayMemory& other) const
{
	return this->array_values == other.array_values;
}

bool Memory::operator==(const Memory& other) const
{
	return this->arrayMemory == other.arrayMemory && this->scalarMemory == other.scalarMemory;
}

std::string Array::to_html() const
{
	std::string html = "<table style=\"margin: 0px; padding: 0px;\" class=\"table\">", tr1, tr2;
	for (int i = 0; i < size; i++)
	{
		int val_by_index = 0;
		if (index_values.count(i) > 0)
		{
			tr1 += "<th style=\"margin: 0px; padding: 0px;\">"+std::to_string(i)+"</th>";
			tr2 += "<td style=\"margin: 0px; padding: 0px;\">" + std::to_string(index_values.at(i)) + "</td>";
		}
		else
		{
			tr1 += "<th style='background: #CCCCCC; margin: 0px; padding: 0px;'>" + std::to_string(i) + "</th>";
			tr2 += "<td style='background: #CCCCCC; margin: 0px; padding: 0px;'></td>";
		}
	}
	tr1 = "<tr>" + tr1 + "</tr>";
	tr2 = "<tr>" + tr2 + "</tr>";
	html += tr1 + tr2 + "</table>";
	return html;
}

std::string Array::to_json() const
{
	std::string json = "";
	for (auto it = index_values.cbegin(); it != index_values.cend(); ++it) {
		if (it != index_values.cbegin()) json += ",";
		json += "\"";
		json += std::to_string(it->first);
		json += "\":";
		json += std::to_string(it->second);
	}
	json = "{" + json + "}";
	return json;
}

std::string ArrayMemory::to_html() const
{
	std::string html = "<table style=\"margin: 0px; padding: 0px;\" class=\"table\">";
	for (auto it = this->array_values.cbegin(); it != this->array_values.cend(); it++)
	{
		html += "<tr><th style=\"margin: 0px; padding: 0px;\">" + it->first + "</th><td style=\"margin: 0px; padding: 0px;\">" + it->second.to_html() + "</td></tr>";
	}
	html += "</table>";
	return html;
}

std::string ArrayMemory::to_json() const
{
	std::string json = "";
	for (auto it = array_values.cbegin(); it != array_values.cend(); ++it) {
		if (it != array_values.cbegin()) json += ",";
		json += "\"";
		json += it->first;
		json += "\":";
		json += it->second.to_json();
	}
	json = "{" + json + "}";
	return json;
}

std::string Memory::to_html() const
{
	//std::string html = "<table><tr><th>Scalar</th><td>"+scalarMemory.to_html()+"</td></tr><tr><th>Arrays</th><td>"+arrayMemory.to_html()+"</td></tr></table>";
	std::string html = "<table style=\"margin: 0px; padding: 0px;\" class=\"table\"><tr><th>Scalars</th><th>Arrays</th></tr> <tr><td>"+ scalarMemory.to_html() +"</td><td>"+ arrayMemory.to_html() +"</td></tr> </table>";
	return html;
}

std::string Memory::to_json() const
{
	std::string json = "{\"scalar_memory\":"+this->scalarMemory.to_json()+","+"\"array_memory\":"+this->arrayMemory.to_json()+"}";
	return json;
}



int Memory::get_scalar_variable_value(std::string var_name) const
{
	return this->scalarMemory.var_values.at(var_name);
}
int Memory::get_array_element_value(std::string arr_name, int index) const
{
	return this->arrayMemory.array_values.at(arr_name).index_values.at(index);
}