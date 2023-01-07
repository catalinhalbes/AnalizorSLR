#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "Grammar_part.h"

using namespace std;

struct Grammar_exception {
	string message;

	Grammar_exception() {};
	Grammar_exception(const string& message) : message(message) {}
};

class Grammar
{
private:
	const unordered_set<int>& terminals;
	const unordered_set<int>& nonterminals;
	int start_nonterminal;
	const unordered_map<int, vector<vector<Grammar_part>>>& rules;

public:
	Grammar(
		const unordered_set<int>& terminals, 
		const unordered_set<int>& nonterminals, 
		int start_nonterminal, 
		const unordered_map<int, vector<vector<Grammar_part>>>& rules
	);

	static unordered_set<vector<Grammar_part>> plus1(
		const unordered_set<vector<Grammar_part>>& L1,
		const unordered_set<vector<Grammar_part>>& L2
	);
};

