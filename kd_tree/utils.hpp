#pragma once
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

using Point = vector<double>;

enum class Types {
  STRING,
  INT,
  DOUBLE,
};

//---READ FILE---
inline size_t countSplits(const std::string &str, const char delim) {
  // Count the items(splits) in a string.
  size_t cnt = std::count(str.begin(), str.end(), delim);
  if (!str.empty())
    cnt++;
  return cnt;
}

inline size_t countFileLines(std::istream &file) {
  size_t cnt = count(std::istreambuf_iterator<char>(file), {}, '\n');
  file.seekg(std::ios_base::beg);
  return cnt;
}

inline Point lineToPoint(const std::string &str, const char delim,
                         const size_t numSplits) {
  Point ret(numSplits);
  size_t nextPos = std::string::npos;

  size_t i = 0;
  do {
    size_t tmp;
    ret[i] = stod(str.substr(nextPos + 1), &tmp);
    nextPos += tmp;
    nextPos = str.find_first_of(delim, nextPos);
    i++;
  } while (i < numSplits and nextPos != std::string::npos);

  return ret;
}

/**
 * @brief Reads a CSV file that contains floating values.
 * @param file File to be read.
 * @param hasHeader If the first line of the file is the header.
 * @return Vector of points.
 */
inline std::vector<Point> readCSV(std::istream &file,
                                  const bool hasHeader = false) {
  char delim = ',';
  std::string line;
  size_t numLines = countFileLines(file);

  getline(file, line);
  size_t numSplits = countSplits(line, delim);

  if (hasHeader)
    numLines--;
  else
    file.seekg(std::ios_base::beg);

  std::vector<Point> points = std::vector<Point>(numLines, Point());
  for (size_t i = 0; i < numLines; i++) {
    getline(file, line);
    points[i] = lineToPoint(line, delim, numSplits);
  }
  return points;
}


//---READ FILE---

inline int comparePoints(const Point &a, const Point &b,
                         const size_t sgnf,
                         const size_t dim) {
  // Compare a and b using the most significant dimension (or the sorting
  // dimension).
  // Compares the next dimension if the actual one is equal in bot points.
  // If a < b -> returns negative, if a >b returns positive, else returns 0.
  for (size_t i = 0; i < dim; i++) {
    size_t it = i + sgnf;
    if (it >= dim)
      it = it - dim;
    if (a[it] != b[it]) {
      bool comp = a[it] < b[it];
      if (comp)
        return -1;
      else
        return 1;
    }
  }
  // If all the dimensions are equal, it return False, which is equal to a <
  // b;
  return 0;
}

inline void removeDuplicates(vector<Point> &a) {
  // Removes the duplicate points.
  // The vector must be sort.
  auto last = unique(a.begin(), a.end());
  if (last != a.end())
    a.resize(last - a.begin());
}

inline void removeDuplicatesIndex(const vector<Point> &points,
                                  vector<size_t> &indexes) {
  // Same as removeDuplicates, but instead of removing then in the points
  // vector,
  // they are removed in the indexes vector.
  // The vector must be sorted.
  auto last =
      unique(indexes.begin(), indexes.end(), [&](const auto &a, const auto &b) {
        return points[a] == points[b];
      });
  if (last != indexes.end())
    indexes.resize(last - indexes.begin());
}
