#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

using Point = vector<string>;

enum class Types {
    STRING,
    INT,
    DOUBLE,
};

inline vector<string> splitStr(const string& str, const char delim) {
    // Splits a string, using delim as the delimiter and return 
    // a vector with the result.
    vector<string> res;
    size_t fnd = str.find_first_of(delim);
    size_t pos = 0;
    while (fnd != string::npos) {
        res.emplace_back(str, pos, fnd - pos);
        pos = fnd + 1;
        fnd = str.find_first_of(delim, fnd + 1);
    }
    res.emplace_back(str, pos, fnd - pos);
    return res;
}

inline vector<string> splitStr(const string& str, const char delim, const size_t itemsQtty) {
    // Same as splitStr, but extract a certain number of items
    // or all the items if you know their quantity.
    // More efficient than splitStr if you know the item quantity.
    vector<string> res(itemsQtty);
    size_t fnd = str.find_first_of(delim);
    size_t pos = 0;
    size_t i = 0;
    while (fnd != string::npos and i < itemsQtty) {
        res[i] = string(str, pos, fnd - pos);
        pos = fnd + 1;
        fnd = str.find_first_of(delim, fnd + 1);
        i++;
    }
    res[i] = string(str, pos, fnd - pos);
    return res;
}

inline size_t countSplits(const string& str, const char delim) {
    // Count the items(splits) in a string.
    size_t cnt = count(str.begin(), str.end(), delim);
    if (!str.empty())
        cnt++;
    return cnt;
}

inline size_t countFileLines(istream& file) {
    size_t cnt = count(istreambuf_iterator<char>(file), {}, '\n');   
    file.seekg(ios_base::beg);
    return cnt;
}

inline int typeOfStrVal(const string& str) {
    // Returns an integer representing the type of data in a string:
    // 0: string
    // 1: integer
    // 2: double
    size_t firstchar = str.find_first_of("-0123456789");
    size_t lastchar = str.find_last_of("0123456789");

    if (firstchar == 0 and lastchar == str.size() - 1) {
        if (str.find_first_of('.') != string::npos)
            return 2;
        else
            return 1;
    }
    else
        return 0;
}

inline vector<Point> readCSV(istream& file, const bool hasHeader) {
    char delim = ',';
    string line;
    size_t numLines = countFileLines(file);
    size_t numSplits; 
    size_t i;
    vector<Point> points;
    if (hasHeader) {
        getline(file, line);
        numLines--;
        numSplits = countSplits(line, delim);
        points = vector<Point>(numLines, Point()); 
        i = 0 ;
    }
    else {
        getline(file, line);
        // TODO: Fix this, change Point with string.
        Point tmp =  splitStr(line, delim);
        numSplits = tmp.size();
        points = vector<Point>(numLines, Point());
        points[0] = move(tmp);
        i = 1;
    }
    for (; i < numLines; i++) {
        getline(file, line);
        points[i] = splitStr(line, delim, numSplits);
    }
    return points;
}

inline int comparePoints(const Point& a, const Point& b, const vector<Types>& types, const size_t sgnf, const size_t dim) { 
    // Compare a and b using the most significant dimension (or the sorting dimension).
    // Compares the next dimension if the actual one is equal in bot points.
    // If a < b -> returns negative, if a >b returns positive, else returns 0.
    for (size_t i = 0; i < dim; i++) {
        size_t it = i + sgnf;
        if (it >= dim)
            it = it - dim;
        if (a[it] != b[it]) {
            bool comp;
            // TODO: use switch case.
            if (types[it] == Types::STRING) {
                cout << "str " << endl;
                comp = a[it] < b[it];
            }
            else if (types[it] == Types::DOUBLE) {
                cout << "dou" << endl;
                // TODO: soncider "NULL" string!!!!
                comp = stod(a[it]) < stod(a[it]);
            }
            else if (types[it] == Types::INT) {
                cout << "int" << endl;
                comp = stoi(a[it]) < stoi(b[it]);
            }
            if (comp)
                return -1;
            else 
                return 1;
        }

    }
    // If all the dimensions are equal, it return False, which is equal to a < b;
    return 0; 
}

inline void removeDuplicates(vector<Point>& a) {
    // Removes the duplicate points.
    // The vector must be sort.
    auto last = unique(a.begin(), a.end());
    if (last != a.end())
        a.resize(last - a.begin());
}

inline void removeDuplicatesIndex(const vector<Point>& points, vector<size_t>& indexes) {
    // Same as removeDuplicates, but instead of removing then in the points vector,
    // they are removed in the indexes vector.
    // The vector must be sorted.
    auto last = unique(indexes.begin(), indexes.end(), [&](const auto& a, const auto& b) {
            return points[a] == points[b]; 
            });
    if (last != indexes.end())
        indexes.resize(last - indexes.begin());
}
