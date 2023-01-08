#pragma once

#include <functional>
#include <unordered_set>
#include <vector>

#include "Element.h"
#include "Grammar_part.h"

template <class T>
inline void hash_combine(std::size_t& s, const T& v) {
	std::hash<T> h;
	s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

namespace std {
	template<>
	struct hash<Grammar_part> {
		inline size_t operator()(const Grammar_part& part) const {
			size_t res = 0;
			hash_combine(res, part.type);
			hash_combine(res, part.id);
			return res;
		}
	};

	template<>
	struct hash<Element> {
		inline size_t operator()(const Element& el) const {
			size_t res = 0;
			hash_combine(res, el.left);
			hash_combine(res, el.right);
			hash_combine(res, el.u);
			hash_combine(res, el.dot_pos);
			return res;
		}
	};

	template<class T>
	struct hash<unordered_set<T>> {
		inline size_t operator()(const unordered_set<T>& parts) const {
			size_t res = 0;
			for (const auto& part : parts) {
				hash_combine(res, part);
			}
			return res;
		}
	};

	template<class T>
	struct hash<vector<T>> {
		inline size_t operator()(const vector<T>& parts) const {
			size_t res = 0;
			for (const auto& part : parts) {
				hash_combine(res, part);
			}
			return res;
		}
	};
}
