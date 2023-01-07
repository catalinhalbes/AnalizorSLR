#include <iostream>
#include "Grammar_parser.h"
#include "Grammar.h"

using std::cout;
using std::endl;

void printPlus1(unordered_set<vector<Grammar_part>> L1, unordered_set<vector<Grammar_part>> L2) {
    auto res = Grammar::plus1(L1, L2);

    for (const auto& vec : res) {
        cout << " [ ";
        for (const auto& el : vec) {
            cout << (char)el.id << ", ";
        }
        cout << " ] ";
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <grammar_file> <input_fip_file>" << endl;
        return 1;
    }

    try {
        Grammar_parser parser(argv[1]);
        Grammar grammar(
            parser.get_terminals_codes(),
            parser.get_nonterminals_codes(),
            parser.get_start_nonterminal_code(),
            parser.get_rules()
        );

        unordered_set<vector<Grammar_part>> L1 = {
                { {Grammar_part::TERMINAL, 'a'} },
                { {Grammar_part::TERMINAL, 'a'}, {Grammar_part::TERMINAL, 'b'} },
                { {Grammar_part::TERMINAL, 'a'}, {Grammar_part::TERMINAL, 'b'}, {Grammar_part::TERMINAL, 'b'} }
        };

        unordered_set<vector<Grammar_part>> L2 = {
                { {Grammar_part::TERMINAL, 'c'} },
                { {Grammar_part::TERMINAL, 'c'}, {Grammar_part::TERMINAL, 'c'} },
                { {Grammar_part::TERMINAL, 'c'}, {Grammar_part::TERMINAL, 'c'}, {Grammar_part::TERMINAL, 'c'} }
        };

        unordered_set<vector<Grammar_part>> L3 = {
                { {Grammar_part::EPSILON, 0} },
                { {Grammar_part::TERMINAL, 'a'} }
        };

        unordered_set<vector<Grammar_part>> L4 = {
                { {Grammar_part::EPSILON, 0} },
                { {Grammar_part::TERMINAL, 'b'} }
        };

        cout << "L1 (+)1 L2: { ";
        printPlus1(L1, L2);
        cout << " } \n";

        cout << "L3 (+)1 L4: { ";
        printPlus1(L3, L4);
        cout << " } \n";

        cout << "L3 (+)1 L2: { ";
        printPlus1(L3, L2);
        cout << " } \n";

        cout << "L2 (+)1 L3: { ";
        printPlus1(L2, L3);
        cout << " } \n";
    }
    catch (const Parse_exception& ex) {
        cout << "Line " << ex.line << ": " << ex.message << endl;
    }

    return 0;
}
