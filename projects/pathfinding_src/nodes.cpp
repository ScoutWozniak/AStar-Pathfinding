
#include "nodes.h"
#include "math.h"
#include <algorithm>

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

float DistBetweenPoints(int pos1[], int pos2[]) {
        float ret = 0.0f;

        float xDist = pos1[0]- pos2[0];
        float yDist = pos1[1] - pos2[1];

        ret = xDist * xDist + yDist * yDist;

        return ret > 0.0f ? sqrt(ret) : 0.0f;
}

float Pathfinding::DistanceBetweenNodes(Node *a, Node *b)
{
    int aPos[2] = {a->m_PosX, a->m_PosY};
    int bPos[2] = {b->m_PosX, b->m_PosY};
    return DistBetweenPoints(aPos, bPos);
}

int Pathfinding::GetIDFromPos(int x, int y)
{
    return (y * 16) + x;
}



Pathfinding::Node *Pathfinding::GetNodeInRadius(Pathfinding::World* world, int posX, int posY, float radius)
{
    int posA[2] = {posX, posY};
    Pathfinding::Node* closestNode = nullptr;
    float closestDist = 0.0f;

    for (auto it = world->m_Nodes.begin(); it != world->m_Nodes.end(); ++it) {
        int nodePos[2] = {(*it).m_PosX, (*it).m_PosY};
        float dist = DistBetweenPoints(posA, nodePos);
        if (dist <= radius && (dist < closestDist || closestNode == nullptr)) {
            closestNode = &(*it);
            closestDist = dist;
        }
    }
    return closestNode;
}
