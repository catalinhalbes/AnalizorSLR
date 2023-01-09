#pragma once

class Action {
public:
	enum Action_type { SHIFT, REDUCE, ACCEPT };
	Action_type type;

	int code;
};