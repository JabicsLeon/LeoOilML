#include <iostream>
#include <sstream>
#include <algorithm>
#include <utility>
#include <string>
#include <vector>
#include <exception>
#include <map>
#include "LeoLib.hpp"

int levenshtein_dist(std::string str1, std::string str2, double empl = 1, double del = 1, double swp = 1)
{
	int size_str1 = str1.size();
	int size_str2 = str2.size();
	leo::matrix<double> mt(size_str1 + 1, size_str2 + 1);
	
	for (int i = 0; i <= size_str1; ++i) for (int j = 0; j <= size_str2; ++j)
	{
		if ( i == 0 && j == 0) continue;
		else if (i == 0 && j != 0) mt[i][j] = j * empl;
		else if (j == 0 && i != 0) mt[i][j] = i * del;
		else mt[i][j] = std::min<double>
					({
						mt[i][j - 1] + empl,
						mt[i - 1][j] + del,
						mt[i - 1][j - 1] + (str1[i - 1] == str2[j - 1] ? 0 : swp) 
					});
	}

	return mt[size_str1][size_str2];
}

leo::matrix<double> synonym_matrix(std::vector<std::string> str_vec, double empl = 1, double del = 1, double swp = 1)
{
	int n = str_vec.size();
	leo::matrix<double> syn(n, n);
	bool standart = (empl == 1 && del == 1 && swp == 1);

	for (int i = 0; i < n; ++i)
	{
		int k = 0;
		if (standart) k = i;
		
		for (int j = k; j < n; ++j)
		{
			syn[i][j] = levenshtein_dist(str_vec[i], str_vec[j], empl, del, swp);
			if (standart) syn[j][i] = syn[i][j];
		}
	}

	return syn;
}

std::map<std::string, leo::vector<std::string>> dict_synonyms(std::vector<std::string> str_vec, double empl = 1, double del = 1, double swp = 1)
{
	leo::matrix<double> syn = synonym_matrix(str_vec, empl, del, swp);

	std::map<std::string, leo::vector<std::string>> dict_syn;

	int n = str_vec.size();

	for (int i = 0; i <  n; ++i)
	{
		std::vector<std::pair<std::string, double>> line;
		line.reserve(n - 1);

		for (int j = 0; j < n; ++j) if (i != j) line.emplace_back(std::pair(str_vec[j], syn[i][j]));

		std::sort(line.begin(), line.end(), 
			[](std::pair<std::string, double>& a, std::pair<std::string, double>& b)
			{ return a.second < b.second; });

		leo::vector<std::string> sline(line.size());

		std::transform(line.begin(), line.end(),
				sline.begin(),
				[](std::pair<std::string, double>& a)
				{ return a.first; });

		dict_syn[str_vec[i]] = sline;
	}

	return dict_syn;

}

void process(std::string way, std::string out="dict_synonyms.json", double empl = 1, double del = 1, double swp = 1)
{
	std::stringstream ss = leo::ReadFile(way);

	std::vector<std::string> line;

	int n = 0;
	
	ss >> n;

	line.reserve(n);

	//while (n--, n >=0)
	for (int i = 0; i < n; ++i)
	{
		std::string str;
		ss >> str;
		line.emplace_back(str);
	}

	std::map<std::string, leo::vector<std::string>> dict_syn = dict_synonyms(line, empl, del, swp);

	std::stringstream so;
	
	int num = 0;
	int max_num = dict_syn.size() - 1;
	so << "{\n";
	for (auto it : dict_syn)
	{
		so << "\t'"<< it.first << "' : [";
		for (int i = 0; i < it.second.size(); ++i)
		{
			if (i) so << ", ";
			so << "'" << it.second[i] << "'";
		}
		so << "]";
		if (num != max_num) so <<","; num++;
		so << "\n";
	}
	so << "}";

	leo::WriteFile(out, so);
}

int main(int narg, char* args[])
{
	if (narg < 2) throw std::invalid_argument("There's no way to file!\nUsage: program list_synomycs.txt  [output_file] [cost_of_emplace] [cost_of_delete] [cost_of_swap]");

	std::string way = args[1];
	std::string out = "dict_synonyms.json";
	double empl = 1.0;
	double del = 1.0;
	double swp = 1.0;

	if (narg == 3) out = std::string(args[2]);
	if (narg == 4) empl = std::stod(args[3]);
	if (narg == 5) del = std::stod(args[4]);
	if (narg == 6) swp = std::stod(args[5]);
	
	process(way, out, empl, del, swp);
	
	return 0;
}
