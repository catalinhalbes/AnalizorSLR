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
	inline const Grammar_part& getSymbolPastDot() const;

	inline bool isDotAtEnd() const;

	inline bool operator == (const Element& ot) const;
};

