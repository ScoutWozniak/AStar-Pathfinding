#pragma once
#include <list>

namespace Pathfinding {
    class Node {
    public:
        float m_PosX;
        float m_PosY;

        int m_Id;

        std::list<Node*> m_Neighbors;

        Node(float posX, float posY, int id) {
            m_PosX = posX;
            m_PosY = posY;
            m_Id = id;
        }
    };

    float DistanceBetweenNodes(Node* a, Node* b);

    int GetIDFromPos(int x, int y);

    
    

    // Contains all nodes
    class World {
    public:
        std::list<Node> m_Nodes;

        // Clears out all nodes in the current map
        void NewWorld();

        // Add a node with an optional ID to the array
        // IDs exist so we can reference based on grid index
        void CreateNode(float posX, float posY, int id = -1);

        Node* GetNodeWithId(int id);
    };

    // NOTE - EXPENSIVE!
    Node* GetNodeInRadius(World* world, float posX, float posY, float radius);
}

