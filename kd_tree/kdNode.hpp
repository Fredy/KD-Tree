#pragma once
#include <string>
#include <utility>
#include <vector>
using namespace std;

using Point = vector<double>;

class KDNode {
public:
  Point data;
  KDNode *childs[2];

  KDNode() {
    data = Point(0);
    childs[0] = childs[1] = nullptr;
  }

  KDNode(const Point &data) {
    this->data = data;
    childs[0] = childs[1] = nullptr;
  }

  KDNode(Point &&data) {
    this->data = move(data);
    childs[0] = childs[1] = nullptr;
  }

  ~KDNode(){};
};
