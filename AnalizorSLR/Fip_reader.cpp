#include "Fip_reader.h"

vector<int> Fip_reader::read(const string& filename)
{
    vector<int> rez;
    int num;

    ifstream fin(filename);

    while (fin >> num) {
        rez.push_back(num);
    }

    return rez;
}
