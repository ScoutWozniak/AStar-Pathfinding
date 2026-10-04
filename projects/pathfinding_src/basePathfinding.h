#include "result.h"

namespace Pathfinding {
    // Base class to inherit from that contains various pathfinding algorithims
    class PathfindingMethod {
        private:
            World* m_CurrentWorld;

        public:
            // Create a reference to the world for this method
            // Do not inherit from
            void SetWorld(World* current_world) {
                m_CurrentWorld = current_world;
            }

            virtual PathResult ResolvePath(Node* start, Node* end) {}

            virtual ~PathfindingMethod() = default;
    };

    class Backstepping : public PathfindingMethod {
        public:
        PathResult ResolvePath(Node* start, Node* end);
    };

    class BreadthFirst : public PathfindingMethod {
        public:
        PathResult ResolvePath(Node* start, Node* end);
    };

    class DepthFirst : public PathfindingMethod {
        public:
        PathResult ResolvePath(Node* start, Node* end);
    };

    class AStar : public PathfindingMethod {
        public:
        PathResult ResolvePath(Node* start, Node* end);
    };


    struct PathFindingEntry {
    public:
        float priority;
        Node* node;

        bool operator<(const PathFindingEntry& other) const {
            return priority < other.priority;
        }

        PathFindingEntry(Node* _node, float _priority) {
            node = _node;
            priority = _priority;
        }
    };
}