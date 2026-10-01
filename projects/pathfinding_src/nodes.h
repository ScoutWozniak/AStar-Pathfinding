#include <list>

namespace Pathfinding {
    // Contains all nodes
    class World {
        std::list<Node> m_Nodes;
    };

    class Node {
        float m_PosX;
        float m_PosY;

        std::list<Node*> m_Neighbors;
    };
}

