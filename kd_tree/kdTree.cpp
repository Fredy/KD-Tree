#include "kdTree.hpp"
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
#include "utils.hpp"

KDTree::KDTree(vector<Point>& points, const size_t maxDepth) {
    this->dimensions = points.at(0).size();
    this->depth = maxDepth;
    this->types = typesOfPoints(points);
    this->root = buildTree(points, this->dimensions, maxDepth);
}

KDNode* KDTree::buildTree(vector<Point>& points, const size_t dimensions,
                          const size_t maxDepth) {
    vector<vector<size_t> > sortedIdxs(dimensions,
                                       vector<size_t>(points.size()));
    // Just initialize the first vector, because we need to find duplicates
    // before sorting all the other vectors.
    // To do that, first the vector at 0 is sorted, then the
    // removeDuplicatesIndex is applied in there,
    // finally it is copied to all the remaining vectors.
    // Based on: http://jcgt.org/published/0004/01/03/
    for (size_t i = 0; i < points.size(); i++) sortedIdxs[0][i] = i;

    size_t signDim = 0;
    auto compareFunc = [&](const auto& a, const auto& b) {
        return comparePoints(points[a], points[b], this->types, signDim,
                             dimensions) < 0;
    };

    sort(sortedIdxs[0].begin(), sortedIdxs[0].end(), compareFunc);
    signDim++;
    removeDuplicatesIndex(points, sortedIdxs[0]);

    // Copying and sorting the remaining vectors:
    for (size_t i = 1; i < dimensions; i++) {
        sortedIdxs[i] = sortedIdxs[0];
        sort(sortedIdxs[i].begin(), sortedIdxs[i].end(), compareFunc);
        signDim++;
    }
    return buildSubTrees(points, sortedIdxs, 0, sortedIdxs.at(0).size(),
                         dimensions, maxDepth, 0);
}

KDNode* KDTree::buildSubTrees(vector<Point>& points,
                              vector<vector<size_t> >& sortedIdxs,
                              const size_t first, const size_t last,
                              const size_t dimensions, const size_t maxDepth,
                              const size_t crrntDepth) {
    // Builds the tree recursively.

    // if (first > last) ERROR
    size_t pointsQtty = last - first;
    size_t actualDim = crrntDepth % dimensions;

    if (pointsQtty <= 3) {
        if (pointsQtty == 3) {
            KDNode* node = new KDNode(move(points[sortedIdxs[0][first + 1]]));
            node->childs[0] = new KDNode(move(points[sortedIdxs[0][first]]));
            node->childs[1] =
                new KDNode(move(points[sortedIdxs[0][first + 2]]));
            return node;
        } else if (pointsQtty == 2) {
            KDNode* node = new KDNode(move(points[sortedIdxs[0][first + 1]]));
            node->childs[0] = new KDNode(move(points[sortedIdxs[0][first]]));
            return node;
        } else if (pointsQtty == 1) {
            KDNode* node = new KDNode(move(points[sortedIdxs[0][first]]));
            return node;
        } else
            return nullptr;
    }

    size_t median = first + (pointsQtty - 1) / 2;
    size_t medianIdx = sortedIdxs[0][median];
    KDNode* node = new KDNode(move(points[medianIdx]));
    // The median is now in the KDNode node

    if (crrntDepth >= maxDepth - 1) {
        KDNode* tmp = node;
        for (size_t i = 0; i < median - first; i++) {
            tmp->childs[0] =
                new KDNode(move(points[sortedIdxs[0][median - 1 - i]]));
            tmp = tmp->childs[0];
        }
        tmp = node;
        for (size_t i = 0; i < last - median; i++) {
            tmp->childs[1] =
                new KDNode(move(points[sortedIdxs[0][median + 1 + i]]));
            tmp = tmp->childs[1];
        }
        return node;
    }

    // Reordering the indexes:
    vector<size_t> temp(sortedIdxs[0].begin() + first,
                        sortedIdxs[0].begin() + last);
    for (size_t i = 1; i < dimensions; i++) {
        size_t firstHalf = first;
        size_t secondHalf = median + 1;
        for (size_t j = first; j < last; j++) {
            size_t idx = sortedIdxs[i][j];
            if (points[idx].empty()) continue;
            int cmp = comparePoints(points[idx], node->data, this->types,
                                    actualDim, dimensions);
            if (cmp < 0) {
                sortedIdxs[i - 1][firstHalf] = idx;
                firstHalf++;
            } else if (cmp > 0) {
                sortedIdxs[i - 1][secondHalf] = idx;
                secondHalf++;
            }
        }
    }
    for (size_t i = first; i < last; i++)
        sortedIdxs.back()[i] = temp[i - first];

    // Calling the function recursively
    node->childs[0] = this->buildSubTrees(points, sortedIdxs, first, median,
                                          dimensions, maxDepth, crrntDepth + 1);
    node->childs[1] = this->buildSubTrees(points, sortedIdxs, median + 1, last,
                                          dimensions, maxDepth, crrntDepth + 1);

    return node;
}

KDNode* KDTree::find(const Point& srchPoint) {
    // Find the srchPoint in the tree, if it is found, returns a pointer to it
    // if not, returns a nullptr.
    KDNode* ptr = this->root;
    int comp = -1;
    size_t crrntDepth = 0;
    while (ptr and comp != 0) {
        // TODO: test this !: if the current depth is greater than this->depth
        // all the nodes are
        // in 2 lists: all the left childs and all the right childs,
        // in this case use the crrntDepth % dimensions to perform all the
        // remaining comparisons.
        if (crrntDepth == this->depth - 1) {
            size_t crrntDim = crrntDepth % this->dimensions;
            while (ptr and comp != 0) {
                comp = comparePoints(srchPoint, ptr->data, this->types,
                                     crrntDim, this->dimensions);
                crrntDepth++;
                if (comp > 0)
                    ptr = ptr->childs[1];
                else if (comp < 0)
                    ptr = ptr->childs[0];
            }
            return ptr;
        }

        comp = comparePoints(srchPoint, ptr->data, this->types,
                             crrntDepth % this->dimensions, this->dimensions);
        crrntDepth++;
        if (comp > 0)
            ptr = ptr->childs[1];
        else if (comp < 0)
            ptr = ptr->childs[0];
    }
    return ptr;
}

void KDTree::print() {
    // Prints the tree in a BFS order
    queue<KDNode*> qq;
    qq.push(this->root);
    while (!qq.empty()) {
        if (qq.front()) {
            cout << "-- ";
            for (const auto& i : qq.front()->data) cout << i << ", ";
            cout << '\n';
            qq.push(qq.front()->childs[0]);
            qq.push(qq.front()->childs[1]);
        }
        qq.pop();
    }
}

KDTree::~KDTree(){
    //  TODO
};
