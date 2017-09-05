#include <iostream>
#include <fstream>
#include "kdTree.hpp"
#include "utils.hpp"
#include <string>

using namespace std;


int main() {
    string name = "country-capitals.csv";
    ifstream file(name);
    auto points = readCSV(file, true);
    KDTree tree(points, 31312);
}
