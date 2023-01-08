#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "Grammar_part.h"
#include "Hash.h"

using namespace std;

struct Grammar_exception {
	string message;

	Grammar_exception() {};
	Grammar_exception(const string& message) : message(message) {}
};

class Grammar
{
private:
	int start_nonterminal;
	unordered_set<int> terminals;
	unordered_set<int> nonterminals;
	unordered_map<int, vector<vector<Grammar_part>>> rules;
	unordered_map<int, unordered_set<Grammar_part>> first1;
	unordered_map<int, unordered_set<Grammar_part>> follow1;

	vector<unordered_set<Element>> canonical_closure;

	void enrichGrammar();
	void computeFirst1();
	void computeFollow1();

public:
	Grammar(
		const unordered_set<int>& terminals, 
		const unordered_set<int>& nonterminals, 
		int start_nonterminal, 
		const unordered_map<int, vector<vector<Grammar_part>>>& rules
	);

	static unordered_set<Grammar_part> plus1(
		const unordered_set<Grammar_part>& L1,
		const unordered_set<Grammar_part>& L2
	);

	const unordered_map<int, unordered_set<Grammar_part>>& getFirst1() const;
	const unordered_map<int, unordered_set<Grammar_part>>& getFollow1() const;
};

