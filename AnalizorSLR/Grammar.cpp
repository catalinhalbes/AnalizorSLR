#include "Grammar.h"



unordered_set<vector<Grammar_part>> Grammar::plus1(
	const unordered_set<vector<Grammar_part>>& L1,
	const unordered_set<vector<Grammar_part>>& L2
)
{
	unordered_set<vector<Grammar_part>> result;

	for (const auto& x : L1) {
		for (const auto& y : L2) {
			vector<Grammar_part> w = x;
			w.insert(w.end(), y.begin(), y.end());
			w.erase(remove(w.begin(), w.end(), Grammar_part{ Grammar_part::EPSILON, 0 }), w.end());


			if (w.size() > 0) {
				result.insert({ w[0] });
			}
			else {
				result.insert({ Grammar_part{ Grammar_part::EPSILON, 0}});
			}
		}
	}

	return result;
}

Grammar::Grammar(
	const unordered_set<int>& terminals, 
	const unordered_set<int>& nonterminals, 
	int start_nonterminal, 
	const unordered_map<int, vector<vector<Grammar_part>>>& rules
)	:
	terminals(terminals),
	nonterminals(nonterminals),
	start_nonterminal(start_nonterminal),
	rules(rules)
{
}
