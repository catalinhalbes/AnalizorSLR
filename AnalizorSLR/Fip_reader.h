#pragma once
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Fip_reader
{
public:
	static vector<int> read(const string& filename);
};

