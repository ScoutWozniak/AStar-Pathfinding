#include "basePathfinding.h"
#include "math.h"
#include <queue>
#include <map>


// Minimum step cost
#define HEURISTIC 1.0f

namespace Pathfinding {

    // NOTE: This heuristic is build for grid movement only in 4 directions
    // However this demo translates the grid into nodes at the start
    // We can find a better way to do this via a node based system later
    float AStarHeuristic(const Node* _start, const Node* _goal) {
        float dx = std::abs(_start->m_PosX - _goal->m_PosX);
        float dy = std::abs(_start->m_PosY - _goal->m_PosY);

        return HEURISTIC * (dx + dy);
    }


    // Credit to https://www.redblobgames.com/pathfinding/a-star/implementation.html
    PathResult AStar::ResolvePath(Node* _start, Node* _end)
    {
        std::cout << "Starting Pathfinding" << std::endl;
        StartTiming();

        PathResult path = {};

        // Priority queue so we can access the nodes with the lowest cost easier
        std::priority_queue<PathFindingEntry, std::vector<PathFindingEntry>, std::greater<PathFindingEntry>> open;
        // Cost for each node
        std::map<Node*, float> costSoFar;

        open.push({_start, 0.0f});

        path.m_Results[_start] = _start;
        costSoFar[_start] = 0;

        // While we have no nodes left, access each node with the lowest priority
        while (!open.empty()) {
            Node* current = open.top().node;
            open.pop();

            if (current == _end) {
                break;
            }
                
            
            for (auto next : current->m_Neighbors) {
                float new_cost = costSoFar[current] + Pathfinding::DistanceBetweenNodes(current,next);
                if (costSoFar.find(next) == costSoFar.end() || new_cost < costSoFar[next]) {
                    costSoFar[next] = new_cost;
                    float priority = new_cost + AStarHeuristic(next, _end);

                    // Emplace the node into the queue as long as it has a decent priority calculation
                    // This means if a node gets recalculated to have a lower priority it will be checked at again
                    open.emplace(next, priority);
                    path.m_Results[next] = current;
                }
            }
        }

        path.m_CalcTime = EndTiming(true);

        return path;
    }
}

