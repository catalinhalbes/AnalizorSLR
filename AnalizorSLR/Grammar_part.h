#pragma once
#include <functional>

class Grammar_part
{
public:
	enum Grammar_part_type { TERMINAL, NONTERMINAL, EPSILON };
	Grammar_part_type type;
	int id;
};


