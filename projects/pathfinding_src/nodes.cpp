#include "nodes.h"
#include "math.h"
#include <bits/stdc++.h>


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
    std::list<Pathfinding::Node>::iterator it = std::find_if(m_Nodes.begin(), m_Nodes.end(), [&id](const Pathfinding::Node& x) {return x.m_Id == id;});
    return &(*it);
}

float Pathfinding::DistanceBetweenNodes(Node *a, Node *b)
{
    float ret = 0.0f;

    float xDist = a->m_PosX - b->m_PosX;
    float yDist = a->m_PosY - b->m_PosY;

    ret = xDist * xDist + yDist * yDist;

    return ret > 0.0f ? sqrt(ret) : 0.0f;
}

int Pathfinding::GetIDFromPos(int x, int y)
{
    return (y * 16) + x;
}
