#pragma once
#include <list>

namespace Pathfinding {
    class Node {
    public:
        float m_PosX;
        float m_PosY;

        int m_Id;

        std::list<Node*> m_Neighbors;

        Node(float _posX, float _posY, int _id) {
            m_PosX = _posX;
            m_PosY = _posY;
            m_Id = _id;
        }
    };

    float DistanceBetweenNodes(Node* _a, Node* _b);

    int GetIDFromPos(int _x, int _y);

    
    

    // Contains all nodes
    class World {
    public:
        std::list<Node> m_Nodes;

        // Clears out all nodes in the current map
        void NewWorld();

        // Add a node with an optional ID to the array
        // IDs exist so we can reference based on grid index
        void CreateNode(float _posX, float _posY, int _id = -1);

        Node* GetNodeWithId(int _id);
    };

    // NOTE - EXPENSIVE!
    Node* GetNodeInRadius(World* _world, float _posX, float _posY, float _radius);
}

