
#include "nodes.h"
#include "math.h"
#include <algorithm>
#include <iostream>

void Pathfinding::World::NewWorld()
{
    m_Nodes.clear();
}

void Pathfinding::World::CreateNode(const float posX, const float posY, const int id)
{
    m_Nodes.emplace_back(Pathfinding::Node{posX, posY, id});
}

Pathfinding::Node* Pathfinding::World::GetNodeWithId(const int id)
{
    std::list<Pathfinding::Node>::iterator it = std::find_if(m_Nodes.begin(), m_Nodes.end(), [&id](const Pathfinding::Node& x) {return x.m_Id == id;});
    return &(*it);
}

float DistBetweenPoints(float pos1[], float pos2[]) {
        float ret = 0.0f;

        float xDist = pos1[0]- pos2[0];
        float yDist = pos1[1] - pos2[1];

        ret = xDist * xDist + yDist * yDist;

        return ret > 0.0f ? sqrt(ret) : 0.0f;
}

float Pathfinding::DistanceBetweenNodes(Node *a, Node *b)
{
    float aPos[2] = {(float)a->m_PosX, (float)a->m_PosY};
    float bPos[2] = {(float)b->m_PosX, (float)b->m_PosY};
    return DistBetweenPoints(aPos, bPos);
}

int Pathfinding::GetIDFromPos(const int x,const int y)
{
    return (y * 16) + x;
}



Pathfinding::Node *Pathfinding::GetNodeInRadius(Pathfinding::World* world, const float posX, const float posY, const float radius)
{
    float posA[2] = {posX, posY};
    Pathfinding::Node* closestNode = nullptr;
    float closestDist = 0.0f;

    for (auto it = world->m_Nodes.begin(); it != world->m_Nodes.end(); ++it) {
        float nodePos[2] = {(*it).m_PosX, (*it).m_PosY};
        float dist = DistBetweenPoints(posA, nodePos);
        if (dist <= radius && (dist < closestDist || closestNode == nullptr)) {
            closestNode = &(*it);
            closestDist = dist;
        }
    }
    return closestNode;
}
