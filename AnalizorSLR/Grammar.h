#pragma once
#include <vector>
#include <stack>
#include <list>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#include "Grammar_part.h"
#include "Hash.h"
#include "Action.h"

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
	int original_start_nonterminal;
	unordered_set<int> terminals;
	unordered_set<int> nonterminals;
	unordered_map<int, vector<vector<Grammar_part>>> rules;
	unordered_map<int, unordered_set<Grammar_part>> first1;
	unordered_map<int, unordered_set<Grammar_part>> follow1;

	vector<unordered_set<Element>> canonical_closure;
	//<k1, <k2, v>>
	//k1 - canonical_closure index
	//k2 - grammar_part
	//v  - canonical_closure index
	unordered_map<int, unordered_map<Grammar_part, int>> AF;
	int accept_state = -1;	//canonical_closure index

	unordered_map<int, unordered_map<Grammar_part, Action>> SLR_table;
	vector<pair<int, vector<Grammar_part>>> all_rules;

	void enrichGrammar();
	void computeCanonicalClosure();
	void computeFirst1();
	void computeFollow1();
	void computeSLRTable();
	void gatherAllRules();
	unordered_set<Element> closure(const Element& e) const;
	unordered_set<Element> goTo(const unordered_set<Element>& es, const Grammar_part& p) const;

public:
	Grammar(
		const unordered_set<int>& terminals, 
		const unordered_set<int>& nonterminals, 
		int start_nonterminal, 
		const unordered_map<int, vector<vector<Grammar_part>>>& rules
	);

	vector<int> analyze(vector<int> fip) const;

	static unordered_set<Grammar_part> plus1(
		const unordered_set<Grammar_part>& L1,
		const unordered_set<Grammar_part>& L2
	);

	const unordered_map<int, unordered_set<Grammar_part>>& getFirst1() const;
	const unordered_map<int, unordered_set<Grammar_part>>& getFollow1() const;
	const vector<unordered_set<Element>>& getCanonicalClosure() const;
	int getAcceptState() const;
	const unordered_map<int, unordered_map<Grammar_part, int>>& getAF() const;
	const unordered_map<int, unordered_map<Grammar_part, Action>>& getSLRTable() const;
	const vector<pair<int, vector<Grammar_part>>>& getAllRules() const;
};

