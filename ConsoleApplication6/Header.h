#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <string>

using namespace std;

// Структура для хранения разницы между векторами
struct MapDifference {
    map<int, int> matched; // Индексы "совпадающих точек" (reference_index -> comparing_index)
    int excess_start; // Начало излишней подпоследовательности (если ее нет, то -1)
    set<int> unmatched_reference_points; // Индексы точек эталонной последовательности, не имеющие пары в comparing

};

// Функция для сравнения двух map<string, int> на равенство
bool maps_equal(const map<string, int>& map1, const map<string, int>& map2) {
    return map1 == map2;
}

// Функция для сравнения двух векторов map<string, int> и поиска совпадающей подпоследовательности
MapDifference compare_vectors(const vector<map<string, int>>& reference, const vector<map<string, int>>& comparing) {
    MapDifference result;
    result.excess_start = -1; // Изначально нет излишней подпоследовательности

    size_t ref_idx = 0;
    size_t comp_idx = 0;

    while (ref_idx < reference.size()) {
        bool found_match = false;
        size_t start_comp_idx = comp_idx;  // Store the current value of comp_idx

        while (comp_idx < comparing.size()) {
            if (maps_equal(reference[ref_idx], comparing[comp_idx])) {
                result.matched[ref_idx] = comp_idx;
                ref_idx++;
                comp_idx++;
                found_match = true;
                break; // Found a match for current reference, move to next reference
            }
            comp_idx++;
        }

        if (!found_match) {
            result.unmatched_reference_points.insert(ref_idx);
            ref_idx++;
            comp_idx = start_comp_idx; // Reset comp_idx for the next reference element
        }
        else {
            start_comp_idx = comp_idx;
        }
        comp_idx = start_comp_idx;
    }

    // Определяем начало излишней подпоследовательности
    if (!result.matched.empty()) {
        int last_ref_index;
        int last_comp_index;

        // Находим последний элемент в map
        auto it = result.matched.rbegin();
        last_ref_index = it->first;
        last_comp_index = it->second;

        if (last_comp_index < comparing.size() - 1) {
            result.excess_start = last_comp_index + 1;
        }
    }
    else {
        result.excess_start = 0;
        if (comparing.empty()) {
            result.excess_start = -1;
        }
    }

    return result;
}

string generate_html_difference_report(vector<map<string, int>>& reference, vector<map<string, int>>& comparing)
{
    MapDifference diff = compare_vectors(reference, comparing);
    map<int, string> comparing_element_colors;
    string html = "<table>";
    for (auto it = reference.cbegin(); it != reference.cend(); it++)
    {
        int num = it - reference.cbegin();
        string color = "#FFFFFF";
        if (diff.unmatched_reference_points.count(num) > 0)
            color = "#FFAAAA";
        else if (diff.matched.count(num) > 0)
        {
            color = "#AAFFAA";
            comparing_element_colors[diff.matched.at(num)] = "#AAFFAA";
        }

        html += "<tr style=\"background:" + color + "\"><td>" + std::to_string(num) + "</td>";
        for (auto map_it = it->cbegin(); map_it != it->cend(); map_it++)
        {
            html += "<td> " + map_it->first + " = " + std::to_string(map_it->second) + "</td>";

        }
        html += "</tr>";
    }
    html += "</table>";

    html += "<table>";


    for (auto it = comparing.cbegin(); it != comparing.cend(); it++)
    {
        int num = it - comparing.cbegin();
        string color = (comparing_element_colors.count(num) > 0) ? comparing_element_colors.at(num) : ((num <= diff.excess_start) ? "#FFFFFF" : "#FFAAAA");


        html += "<tr style=\"background:" + color + "\"><td>" + std::to_string(num) + "</td>";
        for (auto map_it = it->cbegin(); map_it != it->cend(); map_it++)
        {
            html += "<td> " + map_it->first + " = " + std::to_string(map_it->second) + "</td>";
        }
    }
    html += "</table>";

    return html;
}