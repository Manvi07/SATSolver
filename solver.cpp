// DPLL based SAT solver
// Author: Manvi Gupta
// RWTH Aachen University Germany
// Course: Satisfiability Checking (12.14115)

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int num_literals = 0, decision_level=0;
std::vector<pair<pair<int, bool>, pair<int, int> > >trail;
vector<vector<int> > clauses;
vector<int>X;

std::vector<std::vector<int>> parse_dimacs(std::string filename) {
	std::vector<std::vector<int>> clauses;
	std::ifstream in(filename);
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		if (line == "") continue;
		if (line.front() == 'c') continue;
		if (line.front() == 'p') 
			{
				string s = "";
				for(auto x: line)
				{
					if(isdigit(x))
					{
						s+=x;
					}
					else if(s=="")
						continue;
					else
					{
						num_literals = stoi(s);
						break;
					}
				}
				continue;
			}
		clauses.emplace_back();
		const char* begin = line.c_str();
		char* end = nullptr;
		while (true) {
			auto id = std::strtoll(begin, &end, 10);
			begin = end;
			if (id == 0) break;
			clauses.back().emplace_back(id);
		}
		if (clauses.back().empty()) clauses.pop_back();
	}
	return clauses;
}

int find_unit()
{
	for(int i=0; i<clauses.size(); i++)
	{
		int cnt = 0, false_cnt = 0;
		bool SAT = false;
		for(int j=0; j<clauses[i].size(); j++)
		{
			if(X[abs(clauses[i][j]) - 1] == -1)
			{
				cnt++;
			}
			else if ((X[abs(clauses[i][j]) - 1] == 1 and clauses[i][j]>0) or (X[abs(clauses[i][j]) - 1] == 0 and clauses[i][j]<0))
			{
				SAT = true;
			}
			else
				false_cnt++;
		}
		if(false_cnt == clauses[i].size())
			{
				return -2;
			}
		if (cnt == 1 and SAT == false)
			{
				return i;
			}
	}
	return -1;
}

void assign_unit(int t)
{
	for(int i=0; i<clauses[t].size(); i++)
	{
		if(X[abs(clauses[t][i])-1] == -1)
		{
			if(clauses[t][i] < 0)
			{
				X[abs(clauses[t][i]) - 1] = 0;
				trail.push_back({{abs(clauses[t][i])-1, false}, {1, decision_level}});
			}
			else
			{
				X[clauses[t][i] -1] = 1;
				trail.push_back({{abs(clauses[t][i])-1, true}, {1, decision_level}});
			}
			break;
		}	
	}
}

bool BCP()
{
	int t = find_unit();
	while(t >= 0)
	{
		assign_unit(t);
		t = find_unit();
	}
	if(t == -2)
	{
		return false;
	}
	return true;
}


int find_unassigned()
{
	for(int i=0; i<X.size(); i++)
	{
		if(X[i] == -1)
			return i;
	}
	return -1;
}

bool decide()
{
	int t = find_unassigned();
	if(t == -1)
	{
		return false;
	}
	int index = t;
	decision_level++;
	X[index] = false;

	trail.push_back({{index, false}, {1, decision_level}});
	return true;
}

bool backtrack()
{
	while(!trail.empty())
	{
		int n = trail.size();
		
		if(decision_level == 0)
			return false;
			
		while(n > 1 and trail.back().second.second == decision_level)
		{
			if(trail[n-2].second.second == decision_level)
			{
				X[trail.back().first.first] = -1;
				trail.pop_back();
			}
			else break;
			n = trail.size();
		}
		decision_level--;
		auto p = trail.back();
		trail.pop_back();

		if(p.second.first == 1)
			{
				X[p.first.first] = !(p.first.second);
				trail.push_back({{p.first.first, !(p.first.second)}, {2, decision_level}});
				return true;
			}
		else
		{
			X[p.first.first] = -1;
		}

	}
	return false;
}

int main(int argc, char* argv[]) {
	assert(argc == 2);

	ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
	
	X.clear(); trail.clear();

	clauses = parse_dimacs(argv[1]);
	int num_clauses = clauses.size();
	X.resize(num_literals, -1);

	if(BCP() == false)
	{
		printf("unsat\n");
		return 20;
	}
	while(true)
	{
		if(decide() == false)
		{
			cout<<"sat\n";
			for(int i=0; i<num_literals; i++)
			{
				if(X[i] == 0)
					cout << -(i+1) << " ";
				else cout << i+1 << " ";
			}
			cout << "\n";
			return 10;
		}
		while(BCP() == false)
		{
			if(backtrack() == false)
			{
				printf("unsat\n");
				return 20;
			}
		}
	}
}