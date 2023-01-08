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

        auto first1 = grammar.getFirst1();
        printf("FIRST = {\n");
        for (auto kv_pair : first1) {
            printf("\t%d: ", kv_pair.first);
            for (auto symbol : kv_pair.second) {
                printf("%d, ", symbol.id);
            }
            printf("\n");
        }
        printf("}\n");
    }
    catch (const Parse_exception& ex) {
        cout << "Line " << ex.line << ": " << ex.message << endl;
    }

    return 0;
}
