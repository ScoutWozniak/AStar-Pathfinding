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
    float AStarHeuristic(Node* start, Node* goal) {
        float dx = std::abs(start->m_PosX - goal->m_PosX);
        float dy = std::abs(start->m_PosY - goal->m_PosY);

        return HEURISTIC * (dx + dy);
    }


    // Credit to https://www.redblobgames.com/pathfinding/a-star/implementation.html
    PathResult AStar::ResolvePath(Node* start, Node* end)
    {
        std::cout << "Starting Pathfinding" << std::endl;
        StartTiming();

        PathResult path = {};

        // Priority queue so we can access the nodes with the lowest cost easier
        std::priority_queue<PathFindingEntry, std::vector<PathFindingEntry>, std::greater<PathFindingEntry>> open;
        // Cost for each node
        std::map<Node*, float> costSoFar;

        open.push({start, 0.0f});

        path.results[start] = start;
        costSoFar[start] = 0;

        // While we have no nodes left, access each node with the lowest priority
        while (!open.empty()) {
            Node* current = open.top().node;
            open.pop();

            if (current == end)
                break;
            
            for (auto next : current->m_Neighbors) {
                float new_cost = costSoFar[current] + Pathfinding::DistanceBetweenNodes(current,next);
                if (costSoFar.find(next) == costSoFar.end() || new_cost < costSoFar[next]) {
                    costSoFar[next] = new_cost;
                    float priority = new_cost + AStarHeuristic(next, end);

                    //std::cout << "Heurisitic from " << next->m_Id << " is " << AStarHeuristic(next, end) << std::endl; -- DEBUG TEXT

                    // Emplace the node into the queue as long as it has a decent priority calculation
                    // This means if a node gets recalculated to have a lower priority it will be checked at again
                    open.emplace(next, priority);
                    path.results[next] = current;
                }
            }
        }

        path.m_CalcTime = EndTiming(true);

        return path;
    }
}

