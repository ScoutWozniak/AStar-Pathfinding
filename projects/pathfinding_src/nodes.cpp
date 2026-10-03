#include "nodes.h"

void Pathfinding::World::NewWorld()
{
    m_Nodes.clear();
}

void Pathfinding::World::CreateNode(float posX, float posY, int id)
{
    m_Nodes.emplace_back(Pathfinding::Node{posX, posY, id});
}

Pathfinding::Node* Pathfinding::World::GetNodeWithId(int id)
{
    for(Pathfinding::Node node : m_Nodes) {
        if (node.m_Id == id) {
            return &node;
        }
    }
    return nullptr;
}
