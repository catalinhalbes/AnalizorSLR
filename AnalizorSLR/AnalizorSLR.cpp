#include <iostream>
#include "Grammar_parser.h"

using std::cout;
using std::endl;

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <grammar_file> <input_fip_file>" << endl;
        return 1;
    }

    try {
        Grammar_parser grammar(argv[1]);
    }
    catch (const ParseException& ex) {
        cout << "Line " << ex.line << ": " << ex.message << endl;
    }

    return 0;
}
