#pragma once

class Grammar_part
{
public:
	enum Grammar_part_type { TERMINAL, NONTERMINAL, EPSILON, DOLLAR };
	Grammar_part_type type;
	int id;

	inline bool operator==(const Grammar_part& other) const {
		return type == other.type && id == other.id;
	}
};

const Grammar_part EPSILON{ Grammar_part::EPSILON, -1 };
const Grammar_part DOLLAR{ Grammar_part::DOLLAR, -4 };