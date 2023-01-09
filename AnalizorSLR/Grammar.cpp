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

const vector<unordered_set<Element>>& Grammar::getCanonicalClosure() const
{
	return canonical_closure;
}

int Grammar::getAcceptState() const
{
	return accept_state;
}

const unordered_map<int, unordered_map<Grammar_part, int>>& Grammar::getAF() const
{
	return AF;
}

const unordered_map<int, unordered_map<Grammar_part, Action>>& Grammar::getSLRTable() const
{
	return SLR_table;
}

const vector<pair<int, vector<Grammar_part>>>& Grammar::getAllRules() const
{
	return all_rules;
}

void Grammar::enrichGrammar()
{
	int added = nonterminals.size();
	nonterminals.insert(added);
	rules[added] = { { Grammar_part { Grammar_part::NONTERMINAL, start_nonterminal } } };
	start_nonterminal = added;
}

void Grammar::computeCanonicalClosure()
{
	unordered_set<Element> I = closure(Element{ start_nonterminal, rules[start_nonterminal][0], DOLLAR, 0 });
	canonical_closure.push_back(I);
	
	bool finished = false;

	while (!finished) {
		finished = true;

		for (int i = 0; i < canonical_closure.size(); i++) {
			auto handle_collection = [&](const Grammar_part& part) {
				I = goTo(canonical_closure[i], part);

				if (I.size() == 0) {
					return;
				}

				const auto& it = find(canonical_closure.begin(), canonical_closure.end(), I);
				int index = it - canonical_closure.begin();

				if (it == canonical_closure.end()) {
					canonical_closure.push_back(I);

					if (accept_state < 0) {
						if (I.size() == 1) {
							const Element& e = *(I.begin());
							if (
								e.left == start_nonterminal &&
								e.right.size() == 1 &&
								e.right[0].type == Grammar_part::NONTERMINAL &&
								e.right[0].id == original_start_nonterminal &&
								e.u.type == Grammar_part::DOLLAR &&
								e.isDotAtEnd()
								) {
								accept_state = index;
							}
						}
					}
				}

				if (
					AF.find(i) == AF.end() ||
					AF.at(i).find(part) == AF.at(i).end()
				) {
					AF[i][part] = index;
					finished = false;
				}
			};

			for (int term : terminals) {
				Grammar_part part{ Grammar_part::TERMINAL, term };
				handle_collection(part);
			}

			for (int nonterm : nonterminals) {
				Grammar_part part{ Grammar_part::NONTERMINAL, nonterm };
				handle_collection(part);
			}
		}
	}
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
				}
				const auto& X = rule[rule.size() - 1];
				if (X.type == Grammar_part::NONTERMINAL) {
					auto copyFollow1A = follow1[A];
					follow1[X.id].merge(copyFollow1A);
				}
			}
		}
	} while (modified);
}

void Grammar::computeSLRTable()
{
	//add shifts
	for (const auto& pair1 : AF) {
		int start_id = pair1.first;
		for (const auto& pair2 : pair1.second) {
			SLR_table[start_id][pair2.first] = Action{ Action::SHIFT, pair2.second };
		}
	}

	//add reduce
	for (int i = 0; i < canonical_closure.size(); i++) {
		for (const Element& el : canonical_closure[i]) {
			if (!el.isDotAtEnd()) {
				continue;
			}

			int rule_idx = find(all_rules.begin(), all_rules.end(), make_pair(el.left, el.right)) - all_rules.begin();
			
			if (SLR_table[i].find(el.u) == SLR_table[i].end()) {
				if (i != accept_state) {
					SLR_table[i][el.u] = Action{ Action::REDUCE, rule_idx };
				}
				else {
					SLR_table[i][el.u] = Action{ Action::ACCEPT, rule_idx };
				}
			}
			else {
				string err;
				if (SLR_table[i][el.u].type == Action::SHIFT) {
					err = "Shift / Reduce conflict";
				}
				else if (SLR_table[i][el.u].type == Action::REDUCE) {
					err = "Reduce / Reduce conflict";
				}
				else {
					err = "Unindentified conflict";
				}

				throw Grammar_exception(err);
			}
		}
	}
}

void Grammar::gatherAllRules()
{
	for (const auto& p1 : rules) {
		int nonterminal_id = p1.first;
		for (const auto& rule : p1.second) {
			all_rules.push_back(make_pair(nonterminal_id, rule));
		}
	}

	sort(all_rules.begin(), all_rules.end(), [](pair<int, vector<Grammar_part>> a, pair<int, vector<Grammar_part>> b) -> bool {
		return a.first < b.first;
		});
}

unordered_set<Element> Grammar::closure(const Element& e) const
{
	unordered_set<Element> rez;
	unordered_set<int> explored;
	rez.insert(e);

	if (e.isDotAtEnd()) {
		return rez;
	}

	bool finished = false;

	while (!finished) {
		finished = true;

		for (const Element& el : rez) {
			Grammar_part B = el.getSymbolPastDot();
			if (B.type == Grammar_part::NONTERMINAL && explored.count(B.id) == 0) {
				explored.insert(B.id);

				if (rules.find(B.id) != rules.end()) {
					for (const auto& rule : rules.at(B.id)) {
						for (const auto& u : follow1.at(B.id)) {
							rez.insert(Element{ B.id , rule, u, 0 });
						}
					}
				}

				finished = false;
				break;
			}
		}
	}

	return rez;
}

unordered_set<Element> Grammar::goTo(const unordered_set<Element>& element_set, const Grammar_part& p) const
{
	unordered_set<Element> goto_pre_closure;
	unordered_set<Element> res;

	for (const auto& el : element_set) {
		if (!el.isDotAtEnd()) {
			Grammar_part X = el.getSymbolPastDot();
			if (p == X) {
				Element temp_el = el;
				temp_el.dot_pos += 1;

				goto_pre_closure.insert(temp_el);
			}
		}
	}

	for (const auto& el : goto_pre_closure) {
		res.merge(closure(el));
	}

	return res;
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
	original_start_nonterminal(start_nonterminal),
	rules(rules)
{
	enrichGrammar();
	computeFirst1();
	computeFollow1();
	computeCanonicalClosure();
	gatherAllRules();
	computeSLRTable();
}
