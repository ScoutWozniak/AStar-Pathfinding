#pragma once
#include "nodes.h"
#include <map>
#include <vector>
#include <algorithm>

namespace Pathfinding {

    // Contains a list of nodes IN ORDER to get to the target location
    class PathResult {
        public:
        std::map<Node*, Node*> results;

        float m_CalcTime;
    };

    inline std::vector<Node*> ReconstructPath(Node* start, Node* end, PathResult result) {
        std::vector<Node*> path;
        Node* current = end;
        if (result.results.find(end) == result.results.end()) {
            return path;
        }
        while (current != start) {
            path.push_back(current);
            current = result.results[current];
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());
        return path;
    }
}