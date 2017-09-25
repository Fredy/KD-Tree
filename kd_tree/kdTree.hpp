#pragma once
#include "kdNode.hpp"
#include "utils.hpp"

class KDTree {
public:
  KDNode *root;
  size_t dimensions;
  size_t depth;
  vector<Types> types;

  KDTree() { root = nullptr; }

  KDTree(vector<Point> &points, const size_t depth);
  KDNode *buildTree(vector<Point> &points, const size_t dimensions,
                    const size_t maxDepth);
  KDNode *buildSubTrees(vector<Point> &points,
                        vector<vector<size_t>> &sortedIdxs, const size_t first,
                        const size_t last, const size_t dimensions,
                        const size_t maxDepth, const size_t crrntDepth);

  KDNode *find(const Point &srchPoint);
  void print();
  void clear();
  void clearImp(KDNode *node);
  ~KDTree();
};
