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
    tree.print();

    auto fnd = tree.find(vector<string>{"Wallis and Futuna", "Mata-Utu", "-13.95", "-171.933333", "WF", "Australia"});
    cout << (fnd != nullptr ? "Found!" : "Not Found") << endl;
}
