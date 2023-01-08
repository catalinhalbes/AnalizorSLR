#include "Grammar.h"



unordered_set<Grammar_part> Grammar::plus1(
	const unordered_set<Grammar_part>& L1,
	const unordered_set<Grammar_part>& L2
)
{
	unordered_set<Grammar_part> result;

	for (const auto& x : L1) {
		for (const auto& y : L2) {
			vector<Grammar_part> w = { x, y };
			w.erase(remove(w.begin(), w.end(), EPSILON), w.end());

			if (w.size() > 0) {
				result.insert(w[0]);
			}
			else {
				result.insert(EPSILON);
			}
		}
	}

	return result;
}

const unordered_map<int, unordered_set<Grammar_part>>& Grammar::getFirst1() const
{
	return first1;
}

const unordered_map<int, unordered_set<Grammar_part>>& Grammar::getFollow1() const
{
	return follow1;
}

void Grammar::computeFirst1() {
	unordered_map<int, unordered_set<Grammar_part>> F0;
	for (int A : nonterminals) {
		F0[A] = {};
		if (rules.find(A) == rules.end()) // if no rules with A on left
			continue;
		for (int a : terminals) {
			for (const vector<Grammar_part>& rule : rules.at(A)) {
				const Grammar_part& firstToken = rule.at(0);
				if (firstToken.id == a && firstToken.type == Grammar_part::TERMINAL) {
					F0[A].insert(firstToken);
				}
				else if (rule.size() == 1 && firstToken.type == Grammar_part::EPSILON) {
					F0[A].insert(EPSILON); // insert epsilon
				}
			}
		}
	}

	bool different;
	do {
		different = false;
		for (int A : nonterminals) {
			first1[A] = {}; // initialize set
			if (rules.find(A) == rules.end()) // if no rules with A on left
				continue;
			for (const vector<Grammar_part>& rule : rules.at(A)) {
				unordered_set<Grammar_part> Fsum;
				Grammar_part x = rule.at(0);
				if (x.type == Grammar_part::NONTERMINAL) {
					Fsum = F0[x.id];
				}
				else {
					Fsum = { x };
				}
				for (int i = 1; i < rule.size(); i++) {
					x = rule.at(i);
					unordered_set<Grammar_part> F1x;
					if (x.type == Grammar_part::NONTERMINAL) {
						F1x = F0[x.id];
					}
					else {
						F1x = { x };
					}
					Fsum.merge(plus1(Fsum, F1x));
				}
				first1[A].merge(Fsum);
			}
			if (F0[A].size() != first1[A].size())
				different = true;
		}
		F0 = first1;
	} while (different);
}

void Grammar::computeFollow1()
{
	for (int N : nonterminals) {
		if (N == start_nonterminal)
			follow1[N] = { DOLLAR };
		else
			follow1[N] = {};
	}

	bool modified; 
	do {
		modified = false;
		for (const auto& pair : rules) {
			int A = pair.first;
			for (const auto& rule : pair.second) {
				for (int i = 0; i < rule.size() - 1; i++) {
					const auto& B = rule[i];
					if (B.type == Grammar_part::NONTERMINAL) {
						const auto& beta = rule[i + 1];
						unordered_set<Grammar_part> FIRST_beta = 
							beta.type == Grammar_part::NONTERMINAL
							? first1.at(beta.id)
							: unordered_set<Grammar_part>{ beta };
						const auto init_len = follow1[B.id].size();
						const bool had_epsilon = FIRST_beta.erase(EPSILON);
						follow1[B.id].merge(FIRST_beta);
						if (had_epsilon) {
							auto copyFollow1A = follow1[A];
							follow1[B.id].merge(copyFollow1A);
						}
						if (init_len != follow1[B.id].size()) {
							modified = true;
						}
					}
					const auto& X = rule[rule.size() - 1];
					if (X.type == Grammar_part::NONTERMINAL) {
						auto copyFollow1A = follow1[A];
						follow1[X.id].merge(copyFollow1A);
					}
				}
			}
		}
	} while (modified);
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
	computeFirst1();
	computeFollow1();
}
