#include <iostream>
#include "Grammar_parser.h"
#include "Grammar.h"
#include "Fip_reader.h"

using namespace std;

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

        /*auto first1 = grammar.getFirst1();
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
        printf("}\n");*/

        /*const auto& cc = grammar.getCanonicalClosure();

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

        printf("\nAccept state: I%d\n\n", grammar.getAcceptState());

        auto const& AF = grammar.getAF();

        for (const auto& pair1 : AF) {
            int start_idx = pair1.first;
            for (const auto& pair2 : pair1.second) {
                if (pair2.first.type == Grammar_part::NONTERMINAL)
                    printf("I%d --%d(N)--> I%d\n", start_idx, pair2.first.id, pair2.second);
                else
                    printf("I%d --%d(T)--> I%d\n", start_idx, pair2.first.id, pair2.second);
            }
        }*/

        printf("\nRules\n");

        const auto& all_rules = grammar.getAllRules();

        for (int i = 0; i < all_rules.size(); i++) {
            const auto& rule = all_rules[i];
            printf("(%d) %d(N) -> ", i, rule.first);
            for (const auto& el : rule.second) {
                if (el.type == Grammar_part::NONTERMINAL)
                    printf("%d(N) ", el.id);
                else if (el.type == Grammar_part::TERMINAL)
                    printf("%d(T) ", el.id);
                else if (el.type == Grammar_part::EPSILON)
                    printf("EPS ");
            }
            printf("\n");
        }

        /*printf("\nSLR TABLE:\n");

        const auto& SLR_table = grammar.getSLRTable();

        for (const auto& pair1 : SLR_table) {
            int start_idx = pair1.first;
            for (const auto& pair2 : pair1.second) {
                if (pair2.first.type == Grammar_part::NONTERMINAL) {
                    if (pair2.second.type == Action::SHIFT) {
                        printf("I%d --%d(N)--> S%d\n", start_idx, pair2.first.id, pair2.second.code);
                    }
                    else if (pair2.second.type == Action::REDUCE) {
                        printf("I%d --%d(N)--> R%d\n", start_idx, pair2.first.id, pair2.second.code);
                    }
                }
                else if(pair2.first.type == Grammar_part::TERMINAL) {
                    if (pair2.second.type == Action::SHIFT) {
                        printf("I%d --%d(T)--> S%d\n", start_idx, pair2.first.id, pair2.second.code);
                    }
                    else if (pair2.second.type == Action::REDUCE) {
                        printf("I%d --%d(T)--> R%d\n", start_idx, pair2.first.id, pair2.second.code);
                    }
                }
                else if (pair2.first.type == Grammar_part::DOLLAR) {
                    if (pair2.second.type == Action::SHIFT) {
                        printf("I%d --($)--> S%d\n", start_idx, pair2.second.code);
                    }
                    else if (pair2.second.type == Action::REDUCE) {
                        printf("I%d --($)--> R%d\n", start_idx, pair2.second.code);
                    }
                    else if (pair2.second.type == Action::ACCEPT) {
                        printf("I%d --($)--> ACC\n", start_idx);
                    }
                }
            }
        }*/

        cout << "\nEvaluate: ";
        vector<int> fip = Fip_reader::read(argv[2]);

        for (int val : fip) {
            cout << val << " ";
        }
        cout << endl << endl;

        vector<int> prod = grammar.analyze(fip);

        cout << "Prod: ";
        for (int val : prod) {
            cout << val << " ";
        }
        cout << endl << endl;
    }
    catch (const Parse_exception& ex) {
        cout << "Line " << ex.line << ": " << ex.message << endl;
    }
    catch (const Grammar_exception& ex) {
        cout << ex.message << endl;
    }

    return 0;
}
