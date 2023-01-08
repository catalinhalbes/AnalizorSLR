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

        auto follow1 = grammar.getFollow1();
        printf("FOLLOW = {\n");
        for (auto kv_pair : follow1) {
            printf("\t%d: ", kv_pair.first);
            for (auto symbol : kv_pair.second) {
                printf("%d, ", symbol.id);
            }
            printf("\n");
        }
        printf("}\n");

        const auto& cc = grammar.getCanonicalClosure();

        for (int i = 0; i < cc.size(); i++) {
            printf("I%d {\n", i);
            for (const Element& el : cc[i]) {
                printf("\t[%d -> ", el.left);
                for (int j = 0; j < el.right.size(); j++) {
                    if (el.dot_pos == j) {
                        printf(". ");
                    }
                    if (el.right[j].type == Grammar_part::NONTERMINAL) {
                        printf("%d(N) ", el.right[j].id);
                    }
                    else if (el.right[j].type == Grammar_part::TERMINAL) {
                        printf("%d(T) ", el.right[j].id);
                    }
                    else if (el.right[j].type == Grammar_part::EPSILON) {
                        printf("EPS ");
                    }
                    else if (el.right[j].type == Grammar_part::DOLLAR) {
                        printf("DOLL ");
                    }
                }
                if (el.dot_pos == el.right.size()) {
                    printf(". ");
                }
                printf(", %d]\n", el.u.id);
            }
            printf("}\n");
        }

        printf("Accept state: I%d\n", grammar.getAcceptState());

        auto const& AF = grammar.getAF();

        for (const auto& pair1 : AF) {
            int start_idx = pair1.first;
            for (const auto& pair2 : pair1.second) {
                if (pair2.first.type == Grammar_part::NONTERMINAL)
                    printf("I%d --%d(N)--> I%d\n", start_idx, pair2.first.id, pair2.second);
                else
                    printf("I%d --%d(T)--> I%d\n", start_idx, pair2.first.id, pair2.second);
            }
        }
    }
    catch (const Parse_exception& ex) {
        cout << "Line " << ex.line << ": " << ex.message << endl;
    }

    return 0;
}
