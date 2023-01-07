#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <fstream>

#include "Grammar_part.h"

using namespace std;

const string WHITESPACE{ " \n\r\t\f\v" };

struct ParseException {
	string message;
	int line = 0;

	ParseException() {};
	ParseException(const string& message, int line) : message(message), line(line) {}
};

class Grammar_parser
{
private:
	unordered_set<int> terminal_codes;
	unordered_map<string, int> terminals;

	string start_nonterminal;
	unordered_map<string, int> nonterminals;

	unordered_map<int, vector<vector<Grammar_part>>> rules;

	static vector<string> split_string(const string& input, const string& delim);
	static string ltrim(const string& s, const string& chars = WHITESPACE);
	static string rtrim(const string& s, const string& chars = WHITESPACE);
	static string trim(const string& s, const string& chars = WHITESPACE);

public:
	Grammar_parser(const string& grammar_file);
};

