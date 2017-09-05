#include <iostream>
#include <fstream>
#include "kdTree.hpp"
#include "utils.hpp"
using namespace std;


int main() {
    ifstream file("country-capitals.csv");
    auto points = readCSV(file, true);
    KDTree tree(points, 3);

}
