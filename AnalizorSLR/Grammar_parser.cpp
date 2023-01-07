#include "Grammar_parser.h"

vector<string> Grammar_parser::split_string(const string& input, const string& delim) {
	size_t start = 0U;
	size_t end = input.find(delim);
	vector<string> rez;

	while (end != string::npos)
	{
		rez.push_back(trim(input.substr(start, end - start)));
		start = end + delim.length();
		end = input.find(delim, start);
	}

	rez.push_back(trim(input.substr(start, end)));

	return rez;
}

string Grammar_parser::ltrim(const string& s, const string& chars) {
	size_t start = s.find_first_not_of(chars);
	return (start == string::npos) ? "" : s.substr(start);
}

string Grammar_parser::rtrim(const string& s, const string& chars) {
	size_t end = s.find_last_not_of(chars);
	return (end == string::npos) ? "" : s.substr(0, end + 1);
}

string Grammar_parser::trim(const string& s, const string& chars) {
	return ltrim(rtrim(s, chars), chars);
}

Grammar_parser::Grammar_parser(const string& grammar_file) {
	ifstream file(grammar_file);
	string buffer;
	int line = 1;

	getline(file, buffer);
	buffer = trim(buffer);
	while (!file.eof()) {
		if (!buffer.empty()) {
			vector<string> parts = split_string(buffer, " ");

			if (parts[0] == "%term") {
				if (parts.size() < 3) {
					throw Parse_exception("Invalid terminal declaration", line);
				}

				int terminal_id = stoi(parts[2]);

				if (terminal_codes.count(terminal_id)) {
					throw Parse_exception("Duplicate terminal id", line);
				}

				if (nonterminals.find(parts[1]) != nonterminals.end()) {
					throw Parse_exception("Redeclaration of nonterminal as terminal", line);
				}

				if (terminals.find(parts[1]) != terminals.end()) {
					throw Parse_exception("Redeclaration of terminal", line);
				}

				terminals[parts[1]] = terminal_id;
				terminal_codes.insert(terminal_id);
			}
			else if (parts[0] == "%nonterm") {
				if (parts.size() < 2) {
					throw Parse_exception("Invalid nonterminal declaration", line);
				}

				if (start_nonterminal.empty()) {
					start_nonterminal = parts[1];
				}

				if (terminals.find(parts[1]) != terminals.end()) {
					throw Parse_exception("Redeclaration of terminal as nonterminal", line);
				}

				if (nonterminals.find(parts[1]) != nonterminals.end()) {
					throw Parse_exception("Redeclaration of nonterminal", line);
				}

				int code = nonterminals.size();
				nonterminal_codes.insert(code);
				nonterminals[parts[1]] = code;
			}
			else {
				if (parts.size() < 2) {
					throw Parse_exception("Invalid transition declaration", line);
				}

				if (nonterminals.find(parts[0]) == nonterminals.end()) {
					throw Parse_exception("Undeclared nonterminal", line);
				}

				int left_part = nonterminals[parts[0]];
				vector<Grammar_part> right_part;

				if (parts.size() == 2) {
					//epsilon transition
					right_part.push_back(Grammar_part{ Grammar_part::EPSILON, 0 });
				}
				else {
					for (int i = 2; i < parts.size(); i++) {
						if (nonterminals.find(parts[i]) != nonterminals.end()) {
							right_part.push_back(Grammar_part{ Grammar_part::NONTERMINAL, nonterminals[parts[i]] });
						}
						else if (terminals.find(parts[i]) != terminals.end()) {
							right_part.push_back(Grammar_part{ Grammar_part::TERMINAL, terminals[parts[i]] });
						}
						else {
							throw Parse_exception("Undeclared token", line);
						}
					}
				}

				rules[left_part].push_back(right_part);
			}
		}

		getline(file, buffer);
		buffer = trim(buffer);
		line = line + 1;
	}

	for (const auto& p : terminals) {
		printf("terminal %s %d\n", p.first.c_str(), p.second);
	}

	printf("\nStart %s %d\n\n", start_nonterminal.c_str(), nonterminals[start_nonterminal]);

	for (const auto& p: nonterminals) {
		printf("nonterminal %s %d\n", p.first.c_str(), p.second);
	}

	printf("\n");

	for (const auto& prods : rules) {
		int id = prods.first;
		for (const auto& prod : prods.second) {
			printf("%d -> ", id);
			for (const Grammar_part& part : prod) {
				if (part.type == Grammar_part::TERMINAL) {
					printf("%d(T) ", part.id);
				}
				else if (part.type == Grammar_part::NONTERMINAL) {
					printf("%d(N) ", part.id);
				}
				else {
					printf("EPS ");
				}
			}
			printf("\n");
		}
	}
}

const unordered_set<int>& Grammar_parser::get_terminals_codes() const {
	return terminal_codes;
}

const unordered_set<int>& Grammar_parser::get_nonterminals_codes() const {
	return nonterminal_codes;
}

int Grammar_parser::get_start_nonterminal_code() const {
	return nonterminals.at(start_nonterminal);
}

const unordered_map<int, vector<vector<Grammar_part>>>& Grammar_parser::get_rules() const {
	return rules;
}
