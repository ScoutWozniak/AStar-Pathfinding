#pragma once
#include "nodes.h"
#include <map>
#include <vector>
#include <algorithm>

namespace Pathfinding {

    // Contains a list of nodes IN ORDER to get to the target location
    class PathResult {
        public:
        std::map<Node*, Node*> m_Results;

        float m_CalcTime;
    };

    inline std::vector<Node*> ReconstructPath(Node* _start, Node* _end, PathResult _result) {
        std::vector<Node*> path;
        Node* current = _end;
        if (_result.m_Results.find(_end) == _result.m_Results.end()) {
            return path;
        }
        while (current != _start) {
            path.push_back(current);
            current = _result.m_Results[current];
        }
        path.push_back(_start);
        std::reverse(path.begin(), path.end());
        return path;
    }
}