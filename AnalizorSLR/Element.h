#pragma once
#include <vector>

#include "Grammar_part.h"

using namespace std;

class Element
{
public:
	int left;
	vector<Grammar_part> right;
	Grammar_part u;
	int dot_pos = 0;

	/// <summary>
	/// Returns the symbol at the index dot_pos;
	/// Pre: dot_pos is not at the end of the rule;
	/// Throws: out_of_range exception;
	/// </summary>
	/// <returns>Grammar_part</returns>
	inline const Grammar_part& getSymbolPastDot() const {
		return right[dot_pos];
	}

	inline bool isDotAtEnd() const {
		return dot_pos == right.size() || getSymbolPastDot().type == Grammar_part::EPSILON;
	}

	inline bool operator == (const Element& ot) const {
		return left == ot.left && right == ot.right && u == ot.u && dot_pos == ot.dot_pos;
	}
};

