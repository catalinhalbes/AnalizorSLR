#include "Element.h"

inline const Grammar_part& Element::getSymbolPastDot() const
{
	return right[dot_pos];
}

inline bool Element::isDotAtEnd() const
{
	return dot_pos == right.size();
}

inline bool Element::operator==(const Element& ot) const
{
	return left == ot.left && right == ot.right && u == ot.u && dot_pos == ot.dot_pos;
}
