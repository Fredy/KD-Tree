#include <iostream>
#include <fstream>
#include "kdTree.hpp"
#include "utils.hpp"
#include <string>

using namespace std;


int main() {
    string name = "dataM.csv";
    ifstream file(name);
    auto points = readCSV(file, true);
    auto sp = points[200];
    KDTree tree(points, 100000000);
    //tree.print();

    auto fnd = tree.find(sp);
    cout << (fnd != nullptr ? "Found!" : "Not Found") << endl;
}
