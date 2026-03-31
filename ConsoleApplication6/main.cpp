// ConsoleApplication6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <utility>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <iomanip>
#include <sstream>
//#include "ExprNode.h"
//#include "PIDG_node.h"
//#include "ExecutionElement.h"
//#include "PIDG.h"
//#include "Path.h"
//#include "ProgramGraph.h"
#include "ProgramGraph2.h"
//#include "Header.h"



std::string numberic_table, formula_table;

const int N = 5;






// Функция для преобразования значения [0, 1] в шестнадцатеричный код цвета (зеленый)
std::string valueToGreenColor(float value) {
	if (value < 0 || value > 1) {
		return "#FFFFFF"; // Белый для некорректных значений
	}

	int greenValue = static_cast<int>(value * 255); // Преобразуем значение в диапазон 0-255
	std::stringstream ss;
	ss << "#" << std::hex << std::setw(2) << std::setfill('0') << 0 << std::setw(2) << std::setfill('0') << greenValue << std::setw(2) << std::setfill('0') << 0;
	return ss.str();
}


// Функция для вывода heat_map в виде HTML таблицы
void printHeatMapAsHtmlTable(const std::map<int, std::map<int, float>>& heat_map) {
	std::cout << "<table border='1'>" << std::endl;

	// Находим максимальные индексы строк и столбцов для определения размера таблицы.
	int maxRowIndex = -1;
	int maxColIndex = -1;

	for (const auto& rowPair : heat_map) {
		maxRowIndex = std::max(maxRowIndex, rowPair.first);
		for (const auto& colPair : rowPair.second) {
			maxColIndex = std::max(maxColIndex, colPair.first);
		}
	}

	// Выводим заголовок таблицы (номера столбцов)
	std::cout << "  <tr>" << std::endl;
	std::cout << "    <th></th>" << std::endl; // Пустая ячейка для углового заголовка
	for (int j = 0; j <= maxColIndex; ++j) {
		std::cout << "    <th>" << j << "</th>" << std::endl;
	}
	std::cout << "  </tr>" << std::endl;


	// Выводим строки таблицы
	for (int i = 0; i <= maxRowIndex; ++i) {
		std::cout << "  <tr>" << std::endl;
		std::cout << "    <th>" << i << "</th>" << std::endl; // Заголовок строки

		for (int j = 0; j <= maxColIndex; ++j) {
			float value = 0.0f; // Значение по умолчанию (белый цвет)

			// Проверяем, существует ли значение в heat_map для данной ячейки
			auto row_it = heat_map.find(i);
			if (row_it != heat_map.end()) {
				auto col_it = row_it->second.find(j);
				if (col_it != row_it->second.end()) {
					value = col_it->second;
				}
			}

			std::string color = valueToGreenColor(value);
			std::cout << "    <td style='background-color: " << color << ";'>" << std::fixed << std::setprecision(2) << value << "</td>" << std::endl;
		}
		std::cout << "  </tr>" << std::endl;
	}

	std::cout << "</table>" << std::endl;
}




void printTable(std::string action = "")
{
	/*
	formula_table += "<tr><td colspan='5'>" + action + "</td></tr>";
	formula_table  += "<tr> <td>" + expr_node_string(x_node) + "</td><td>" + expr_node_string(y_node) + "</td><td>" + expr_node_string(i_node) + "</td><td>" + expr_node_string(old_y_node) + "</td></tr>";
	

	numberic_table += "<tr><td colspan='5'>" + action + "</td></tr>";
	numberic_table += "<tr> <td> "+std::to_string(x) + "</td><td>" + std::to_string(y) + "</td><td>" + std::to_string(i) + "</td><td>" + std::to_string(old_y) + "</td><td>" + ((x >= 1) ? "true" : "false") + "</td></tr>";

	/*std::map <std::string, int> var_values;
	var_values["x"] = x;
	var_values["y"] = y;
	path.addAction(action);
	path.addState(var_values);*/
}


/*

std::string generate_html_report(std::vector<ProgramGraph> program_graphs)
{

	std::vector <std::string> dot_items;
	for (auto it = program_graphs.cbegin(); it != program_graphs.cend(); it++)
	{
		dot_items.push_back(it->toDOT());
	}

	std::vector <Path> paths;
	for (auto it = program_graphs.cbegin(); it != program_graphs.cend(); it++)
	{
		paths.push_back(it->execute());
	}

	std::vector<OnlyStatePath> onlyStatePaths;
	for (auto it = paths.cbegin(); it != paths.cend(); it++)
	{
		onlyStatePaths.push_back(it->getOnlyStatePath());
	}

	std::vector<OnlyStatePath> normalizedStatePaths;
	for (auto it = onlyStatePaths.cbegin(); it != onlyStatePaths.cend(); it++)
	{
		normalizedStatePaths.push_back(it->removeStutteringRepeats());
	}

	std::vector<OnlyStatePath> normalizedStatePathsByVars;
	std::set<std::string> vars = { "x","y" };
	for (auto it = onlyStatePaths.cbegin(); it != onlyStatePaths.cend(); it++)
	{
		normalizedStatePathsByVars.push_back(it->removeStutteringRepeats(vars));
	}

	std::vector<OnlyActionPath> actionPaths;
	for (auto it = paths.cbegin(); it != paths.cend(); it++)
	{
		actionPaths.push_back(it->getOnlyActionPath());
	}

	std::vector<std::string> PIDG_dots;
	for (auto it = actionPaths.cbegin(); it != actionPaths.cend(); it++)
	{
		PIDG_dots.push_back(it->buildPIDG_dot());
	}

	std::vector<OnlyActionPath> abstractedConstActionPaths;
	for (auto it = actionPaths.cbegin(); it != actionPaths.cend(); it++)
	{
		abstractedConstActionPaths.push_back(it->getAbstractedActionPath(true));
	}

	std::vector<OnlyActionPath> abstractedOperatorActionPaths;
	for (auto it = actionPaths.cbegin(); it != actionPaths.cend(); it++)
	{
		abstractedOperatorActionPaths.push_back(it->getAbstractedActionPath(false,true));
	}

	std::vector<OnlyActionPath> abstractedConstAndOperatorActionPaths;
	for (auto it = actionPaths.cbegin(); it != actionPaths.cend(); it++)
	{
		abstractedConstAndOperatorActionPaths.push_back(it->getAbstractedActionPath(true, true));
	}


	std::string html = "";
	html += "<head>";
	html += "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.6/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-4Q6Gf2aSP4eDXB8Miphtr37CMZZQ5oXLH2yaXMJ2w8e2ZtHTl7GptT4jmndRuHDT\" crossorigin=\"anonymous\">";
	html += "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.6/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-j1CDi7MgGQ12Z7Qab0qlWQ/Qqz24Gc6BM0thvEMVjHnfYGF0rmFCozFSxQBxwHKO\" crossorigin=\"anonymous\"></script>";
	html += "</head>";
	
	html += "<div class=\"alert alert-primary\" role=\"alert\"> Граф программ </div>";
	html += "<table class=\"table\">";
	html += "<tbody>";
	html += "<tr>";
	for (auto it = dot_items.cbegin(); it != dot_items.cend(); it++)
	{
		html += "<td>" + *it + "</td>";
	}
	html += "</tr>";
	html += "</tr></tbody></table>";


	html += "<div class=\"alert alert-primary\" role=\"alert\"> Фрагмент исполнения: состяние1, действие1, состояние2, действие2, ..., состояниеN<br>Состояние s = (loc, mem), где loc - локация в графе программы, mem - состояние памяти (значения переменных)</div>";
	html += "<table class=\"table\">";
	html += "<tbody>";
	html += "<tr>";
	for (auto it = paths.cbegin(); it != paths.cend(); it++)
	{
		html += "<td>" + it->toHTML() + "</td>";
	}
	html += "</tr>";
	html += "</tr></tbody></table>";


	html += "<div class=\"alert alert-primary\" role=\"alert\"> Только состояния: состяние1, состояние2, ..., состояниеN</div>";
	html += "<table class=\"table\">";
	html += "<tbody>";
	html += "<tr>";
	for (auto it = onlyStatePaths.cbegin(); it != onlyStatePaths.cend(); it++)
	{
		html += "<td>" + it->toHTML() + "</td>";
	}
	html += "</tr>";
	html += "</tr></tbody></table>";


	html += "<div class=\"alert alert-primary\" role=\"alert\"> Без заикающихся состояний: состяние1, состояние2, ..., состояниеN</div>";
	html += "<table class=\"table\">";
	html += "<tbody>";
	html += "<tr>";
	for (auto it = normalizedStatePaths.cbegin(); it != normalizedStatePaths.cend(); it++)
	{
		html += "<td>" + it->toHTML() + "</td>";
	}
	html += "</tr>";
	html += "</tr></tbody></table>";

	html += "<div class=\"alert alert-primary\" role=\"alert\"> Без заикающихся состояний: состяние1, состояние2, ..., состояниеN. Только значимые переменные</div>";
	html += "<table class=\"table\">";
	html += "<tbody>";
	html += "<tr>";
	for (auto it = normalizedStatePathsByVars.cbegin(); it != normalizedStatePathsByVars.cend(); it++)
	{
		html += "<td>" + it->toHTML() + "</td>";
	}
	html += "</tr>";
	html += "</tr></tbody></table>";


	html += "<div class=\"alert alert-primary\" role=\"alert\"> Только действия: действие1, действие2, ..., действиеN</div>";
	html += "<table class=\"table\">";
	html += "<tbody>";
	html += "<tr>";
	for (auto it = actionPaths.cbegin(); it != actionPaths.cend(); it++)
	{
		html += "<td>" + it->toHTML() + "</td>";
	}
	html += "</tr>";
	html += "</tr></tbody></table>";

	html += "<div class=\"alert alert-primary\" role=\"alert\"> PIDG </div>";
	html += "<table class=\"table\">";
	html += "<tbody>";
	html += "<tr>";
	for (auto it = PIDG_dots.cbegin(); it != PIDG_dots.cend(); it++)
	{
		html += "<td>" + *it + "</td>";
	}
	html += "</tr>";
	html += "</tr></tbody></table>";

	html += "<div class=\"alert alert-primary\" role=\"alert\"> Абстракция по константам </div>";
	html += "<table class=\"table\">";
	html += "<tbody>";
	html += "<tr>";
	for (auto it = abstractedConstActionPaths.cbegin(); it != abstractedConstActionPaths.cend(); it++)
	{
		html += "<td>" + it->toHTML() + "</td>";
	}
	html += "</tr>";
	html += "</tr></tbody></table>";

	html += "<div class=\"alert alert-primary\" role=\"alert\"> Абстракция по операциям </div>";
	html += "<table class=\"table\">";
	html += "<tbody>";
	html += "<tr>";
	for (auto it = abstractedOperatorActionPaths.cbegin(); it != abstractedOperatorActionPaths.cend(); it++)
	{
		html += "<td>" + it->toHTML() + "</td>";
	}
	html += "</tr>";
	html += "</tr></tbody></table>";


	html += "<div class=\"alert alert-primary\" role=\"alert\"> Абстракция по константам операциям </div>";
	html += "<table class=\"table\">";
	html += "<tbody>";
	html += "<tr>";
	for (auto it = abstractedConstAndOperatorActionPaths.cbegin(); it != abstractedConstAndOperatorActionPaths.cend(); it++)
	{
		html += "<td>" + it->toHTML() + "</td>";
	}
	html += "</tr>";
	html += "</tr></tbody></table>";


	return html;
}

// Функция для генерации HTML таблицы из std::map<int, std::map<int, bool>>
std::string generateHtmlTable(const std::map<int, std::map<int, bool>>& accordance) {
	std::string html = "";

	html += "<table border='1'>\n";

	// Шапка таблицы (номера столбцов)
	html += "  <tr>\n    <th></th>\n";
	for (auto it1 = accordance.cbegin(); it1 != accordance.cend(); ++it1) {
		html += "    <th>" + std::to_string(it1->first) + "</th>\n";
	}
	html += "  </tr>\n";

	// Строки таблицы (номера строк и ячейки с данными)
	for (auto it1 = accordance.cbegin(); it1 != accordance.cend(); ++it1) {
		html += "  <tr>\n    <th>" + std::to_string(it1->first) + "</th>\n";  // Номер строки

		for (auto it2 = accordance.cbegin(); it2 != accordance.cend(); ++it2) {
			bool found = false;
			bool value = false;

			// Находим значение в соответствии с it1->first и it2->first
			if (it1->second.count(it2->first) > 0) {
				value = it1->second.at(it2->first);
				found = true;
			}

			// Заполняем ячейку данными, используя стиль в зависимости от значения
			html += "    <td style=\"";

			if (found) {
				if (value) {
					html += "background-color: #AAFFAA;";
				}
				else {
					html += "background-color: #FFAAAA;";
				}
			}
			html += "\">";
			if (found) {
				html += (value ? "true" : "false");
			}
			html += "</td>\n";
		}
		html += "  </tr>\n";
	}

	html += "</table>\n";

	return html;
}

std::string generate_html_report_two_PIDGs(std::map<int, PIDG_node*> first, std::map<int, PIDG_node*> second)
{
	std::string html = "";
	//html += "<p>First PIDG nodes</p>";

	std::string table1 = "<table border=\"l\">";
	table1 += "<tr> <th>№</th> <th>Действие</th> <th>Действие с абстракцией по константам</th> </tr>";
	for (auto it = first.cbegin(); it != first.cend(); it++)
	{
		table1 += "<tr><th>" + std::to_string(it->first) + "</th><td>"+it->second->to_string()+"</td><td>"+get_abstract_PIDG_node(it->second,true)->to_string()+"</td></tr>";
	}
	table1 += "</table>";


	std::string table2 = "<table border=\"l\">";
	table2 += "<tr> <th>№</th> <th>Действие</th> <th>Действие с абстракцией по константам</th> </tr>";
	for (auto it = second.cbegin(); it != second.cend(); it++)
	{
		table2 += "<tr><th>" + std::to_string(it->first) + "</th><td>" + it->second->to_string() + "</td><td>" + get_abstract_PIDG_node(it->second,true)->to_string() + "</td></tr>";
	}
	table2 += "</table>";
	html = "<p>First PIDG nodes</p>" + table1 + "<p>Second PIDG nodes</p>" + table2;

	std::map<int, std::map<int, bool>> accordance, accordance_abstr_const;

	for (auto it1 = first.cbegin(); it1 != first.cend(); it1++)
	{
		for (auto it2 = second.cbegin(); it2 != second.cend(); it2++)
		{
			bool equals = it1->second->equals_to(it2->second);
			accordance[it1->first][it2->first] = equals;

			bool equals_abstr_consts = get_abstract_PIDG_node(it1->second, true)->equals_to(get_abstract_PIDG_node(it2->second, true));
			accordance_abstr_const[it1->first][it2->first] = equals_abstr_consts;
		}
	}

	std::string table3 = generateHtmlTable(accordance);
	std::string table4 = generateHtmlTable(accordance_abstr_const);
	html += "<p>Соответствие узлов PIDG без абстракции</p>" + table3 + "<p>Соответствие узлов PIDG с абстракцией по константам</p>" + table4;
	return html;
}


*/

std::string generate_hml_compare(const ProgramGraph2 &reference, const ProgramGraph2 &test)
{
	std::string html = "<html>";
	html += "</html>";
	return html;
}

void parse_dot_text(ProgramGraph2 &pg, const std::string& dot_text, int& final_node) {
	
	std::stringstream ss(dot_text);
	std::string line;

	// Регулярное выражение для поиска строк вида "X -> Y [label="..."]"
	std::string regex_string = "(q?(\\d+)\\s*\\-\\>\\s*q?(\\d+)\\s*\\[label=\"(.*?)\"\\]\\s*)";
	std::string regex_final_string = "(q?(\\d+)\\s*\\[shape\\=doublecircle\\])";
	std::cout << regex_string;
	std::regex edge_regex(regex_string);
	std::regex final_regex(regex_final_string);
		std::smatch match;

	while (std::getline(ss, line)) {

		if (std::regex_search(line, match, final_regex))
		{
			if (match.size() == 3)
			{
				final_node = std::stoi(match[2].str());
				pg.set_terminal_loc(final_node);
			}
		}

		if (std::regex_search(line, match, edge_regex)) {
			if (match.size() == 5) {
				try {
					int source_node = std::stoi(match[2].str());
					int target_node = std::stoi(match[3].str());
					std::string label = match[4].str();

					// Удаление лишних пробелов в начале и конце label
					size_t first = label.find_first_not_of(' ');
					if (std::string::npos == first)
					{
						label = "";
					}
					else {
						label = label.substr(first, (label.find_last_not_of(' ') - first + 1));
					}


					ExprNode* expr = parse_expr_from_RPN(label);
					if (expr->type == ASSIGN_EXPR)
					{
						Variable var;
						if (expr->left->type == VAR)
						{
							var.type = SCALAR_VAR;
							var.name = expr->left->var_name;
						}
						else if (expr->left->type == ARRAY_INDEX)
						{
							var.type = ABSTRACT_ARR_ELEMENT;
							var.name = expr->left->left->var_name;
							var.index_expr = copy_node(expr->left->right);
						}
						Action action = create_assign_action(var,expr->right);
						pg.add_action_edge(source_node, target_node, action);
					}
					else
					{
						Action action = create_logic_action(expr);
						pg.add_action_edge(source_node, target_node, action);
					}
						

				}
				catch (const std::invalid_argument& e) {
					std::cerr << "Ошибка при преобразовании строки в число: " << e.what() << std::endl;
				}
				catch (const std::out_of_range& e) {
					std::cerr << "Число слишком велико для типа int: " << e.what() << std::endl;
				}
			}
		}
	}
}

std::string read_file_to_string(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Ошибка: Не удалось открыть файл " << filename << std::endl;
		return ""; // Возвращаем пустую строку в случае ошибки
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

void write_to_file(const std::string &filename, std::string str)
{
	freopen(filename.data(), "w", stdout);
	std::cout << str;
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	for (int i = 0; i < argc; i++)
	{
		std::cout << i << " " << argv[i] << std::endl;
	}

	if (argc > 1)
	{
		std::cout << argc << std::endl;
		std::string pg_file = argv[1];
		ScalarMemory scalarMemory;
		std::cout << scalarMemory.to_json();
		ArrayMemory arrayMemory;
		int current_arg = 2;
		while (current_arg < argc-1)
		{
			if (!strcmp(argv[current_arg] ,"--scalar_var"))
			{
				std::string var = argv[current_arg + 1];
				int val = atoi(argv[current_arg + 2]);
				scalarMemory.var_values[var] = val;
				current_arg += 3;
			}
			else if (!strcmp(argv[current_arg],"--array_var"))
			{
				std::string var = argv[current_arg + 1];
				int len = atoi(argv[current_arg + 2]);
				current_arg += 3;
				int current_index = 0;
				std::map<int,int> arr;
				std::cout << "ARRAY READ" << std::endl;
				while (current_arg < argc && strcmp(argv[current_arg], "--scalar_var") && strcmp(argv[current_arg], "--array_var"))
				{

					int current_element = atoi(argv[current_arg]);
					arr[current_index] = current_element;
					//arr.push_back(current_element);
					current_index++;
					current_arg++;
				}
				Array array = { len, arr};
				arrayMemory.array_values[var] = array;
			}
		}
		Memory memory = { scalarMemory, arrayMemory };
		std::string dot_text = read_file_to_string(pg_file);
		ProgramGraph2 pg(memory,0);
		int final_node;
		parse_dot_text(pg, dot_text, final_node);

		std::string dot_pg = pg.to_dot();
		write_to_file("my_pg.dot", dot_pg);

		ExecutionFragment execution_fragment = pg.execute();
		write_to_file("state_sequence.json", execution_fragment.state_sequence.to_json());
		write_to_file("action_sequence.json", execution_fragment.action_sequence.to_json());
		write_to_file("read_var_sequence.json", execution_fragment.get_json_sequence_read_variables());
		write_to_file("PIDG.dot",execution_fragment.build_PIDG_dot());
	}
}