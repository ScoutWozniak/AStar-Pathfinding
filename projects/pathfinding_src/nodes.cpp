
#include "nodes.h"
#include "math.h"
#include <algorithm>
#include <iostream>

void Pathfinding::World::NewWorld()
{
    m_Nodes.clear();
}

void Pathfinding::World::CreateNode(const float _posX, const float _posY, const int _id)
{
    m_Nodes.emplace_back(Pathfinding::Node{_posX, _posY, _id});
}

Pathfinding::Node* Pathfinding::World::GetNodeWithId(const int _id)
{
    std::list<Pathfinding::Node>::iterator it = std::find_if(m_Nodes.begin(), m_Nodes.end(), [&_id](const Pathfinding::Node& x) {return x.m_Id == _id;});
    return &(*it);
}

float DistBetweenPoints(float _pos1[], float _pos2[]) {
        float ret = 0.0f;

        float xDist = _pos1[0]- _pos2[0];
        float yDist = _pos1[1] - _pos2[1];

        ret = xDist * xDist + yDist * yDist;

        return ret > 0.0f ? sqrt(ret) : 0.0f;
}

float Pathfinding::DistanceBetweenNodes(Node *_a, Node *_b)
{
    float aPos[2] = {(float)_a->m_PosX, (float)_a->m_PosY};
    float bPos[2] = {(float)_b->m_PosX, (float)_b->m_PosY};
    return DistBetweenPoints(aPos, bPos);
}

int Pathfinding::GetIDFromPos(const int _x,const int _y)
{
    return (_y * 16) + _x;
}


Pathfinding::Node *Pathfinding::GetNodeInRadius(Pathfinding::World* _world, const float _posX, const float _posY, const float _radius)
{
    float posA[2] = {_posX, _posY};
    Pathfinding::Node* closestNode = nullptr;
    float closestDist = 0.0f;

    for (auto it = _world->m_Nodes.begin(); it != _world->m_Nodes.end(); ++it) {
        float nodePos[2] = {(*it).m_PosX, (*it).m_PosY};
        float dist = DistBetweenPoints(posA, nodePos);
        if (dist <= _radius && (dist < closestDist || closestNode == nullptr)) {
            closestNode = &(*it);
            closestDist = dist;
        }
    }
    return closestNode;
}
