#include "nodes.h"

namespace Pathfinding {

    // Contains a list of nodes IN ORDER to get to the target location
    class PathResult {
        int m_Steps;
        std::list<Node> result;
    };
}