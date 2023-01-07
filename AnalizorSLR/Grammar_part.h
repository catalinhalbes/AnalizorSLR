#pragma once
#include <functional>
#include <unordered_set>
#include <vector>

using namespace std;

class Grammar_part
{
public:
	enum Grammar_part_type { TERMINAL, NONTERMINAL, EPSILON };
	Grammar_part_type type;
	int id;

	inline bool operator==(const Grammar_part& other) const {
		return type == other.type && id == other.id;
	}
};

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
	struct hash<unordered_set<Grammar_part>> {
		inline size_t operator()(const unordered_set<Grammar_part>& parts) const {
			size_t res = 0;
			for (const auto& part : parts) {
				hash_combine(res, part.type);
				hash_combine(res, part.id);
			}
			return res;
		}
	};

	template<>
	struct hash<vector<Grammar_part>> {
		inline size_t operator()(const vector<Grammar_part>& parts) const {
			size_t res = 0;
			for (const auto& part : parts) {
				hash_combine(res, part.type);
				hash_combine(res, part.id);
			}
			return res;
		}
	};
}
