#include <iostream>
#include "Grammar_parser.h"
#include "Grammar.h"

using std::cout;
using std::endl;

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

        /*auto res = Grammar::plus1(
            { 
                {
                    {Grammar_part::TERMINAL, 'a'}
                },
                {
                    {Grammar_part::TERMINAL, 'a'},
                    {Grammar_part::TERMINAL, 'b'}
                },
                {
                    {Grammar_part::TERMINAL, 'a'},
                    {Grammar_part::TERMINAL, 'b'},
                    {Grammar_part::TERMINAL, 'b'}
                }
            },
            {
                {
                    {Grammar_part::TERMINAL, 'c'}
                },
                {
                    {Grammar_part::TERMINAL, 'c'},
                    {Grammar_part::TERMINAL, 'c'}
                },
                {
                    {Grammar_part::TERMINAL, 'c'},
                    {Grammar_part::TERMINAL, 'c'},
                    {Grammar_part::TERMINAL, 'c'}
                }
            }
        );

        cout << "L1 (+)1 L2: { ";
        for (const auto& vec : res) {
            cout << " [ ";
            for (const auto& el : vec) {
                cout << (char)el.id << ", ";
            }
            cout << " ] ";
        }
        cout << " } \n";*/

        auto res = Grammar::plus1(
            {
                {
                    {Grammar_part::EPSILON, 0}
                },
                {
                    {Grammar_part::TERMINAL, 'a'},
                }
            },
            {
                {
                    {Grammar_part::EPSILON, 0}
                },
                {
                    {Grammar_part::TERMINAL, 'b'}
                }
            }
        );
        cout << "L3 (+)1 L4: { ";
        for (const auto& vec : res) {
            cout << "[";
            for (const auto& el : vec) {
                cout << (char)el.id << ", ";
            }
            cout << "]";
        }
        cout << " } \n";
    }
    catch (const Parse_exception& ex) {
        cout << "Line " << ex.line << ": " << ex.message << endl;
    }

    return 0;
}
