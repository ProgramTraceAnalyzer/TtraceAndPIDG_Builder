#pragma once
#include <map>
#include <vector>
#include <string>
#include <set>
//#include "Variable.h"

struct ScalarMemory
{
	std::map<std::string,int> var_values;
	ScalarMemory get_submemorty(std::set<std::string> variable_names) const;
	std::string to_html() const;
	bool equals_to(const ScalarMemory & other) const;
	bool operator==(const ScalarMemory& other) const;
	std::string to_json() const;
};

struct Array
{
	int size;
	std::map<int,int> index_values;
	std::string to_html() const;
	std::string to_json() const;
	bool equals_to(const Array& other) const;
	bool operator==(const Array& other) const;
	//std::set<int> written_indexes;
};

struct ArrayMemory
{
	std::map<std::string, Array> array_values;
	std::string to_html() const;
	std::string to_json() const;
	ArrayMemory get_submemorty(std::set<std::string> variable_names) const;
	bool operator==(const ArrayMemory& other) const;
};

struct Memory
{
	ScalarMemory scalarMemory;
	ArrayMemory arrayMemory;
	std::string to_html() const;
	std::string to_json() const;
	Memory get_submemorty(std::set<std::string> variable_names) const;
	int get_scalar_variable_value(std::string var_name) const;
	int get_array_element_value(std::string arr_name, int index) const;
	bool operator==(const Memory & other) const;
};