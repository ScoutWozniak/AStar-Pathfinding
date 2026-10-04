#include "basePathfinding.h"
#include "math.h"
// Minimum step cost
#define HEURISTIC 1.0f

namespace Pathfinding {
    // NOTE: This heuristic is build for grid movement only in 4 directions
    // However this demo translates the grid into nodes at the start
    // We can find a better way to do this via a node based system later
    float AStarHeuristic(Node* start, Node* goal) {
        float dx = std::abs(start->m_PosX - goal->m_PosX);
        float dy = std::abs(start->m_PosY - goal->m_PosY);

        return HEURISTIC * (dx + dy);
    }

    PathResult AStar::ResolvePath(Node *start, Node *end)
    {
        return PathResult();
    }
}

