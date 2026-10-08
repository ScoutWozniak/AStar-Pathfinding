#include "npc.h"
#include "global.h"
#include "raylib.h"
#include "raymath.h"
#include "iostream"
#include "basePathfinding.h"



void NPC::UpdateGoal(Pathfinding::Node *_goal)
{
    m_Goal = _goal;
    RecalculatePath();
}

void NPC::Initialise()
{
}

void NPC::Update()
{
    m_CurrentAngle = Lerp(m_CurrentAngle, m_TargetAngle, GetFrameTime() * 20.0f);
    if (m_CurrentPath.empty()) {
        return;
    } 

    Vector2 nodePos =  {m_CurrentPath.front()->m_PosX, m_CurrentPath.front()->m_PosY};
    Vector2 dirToFirstPoint = Vector2Normalize(nodePos - m_Position) * m_Speed * GetFrameTime();
    m_TargetAngle = Vector2Angle(Vector2UnitY, dirToFirstPoint);

    m_Position += dirToFirstPoint;


    float distToFirstPoint = Vector2Distance(m_Position,nodePos);
    if (distToFirstPoint < 0.05f) {
        m_CurrentPath.erase(m_CurrentPath.begin());
    }



}

void NPC::Draw()
{
    Vector2 worldPos = WorldPosToRenderPos(m_Position);
    DrawCircle(worldPos.x + centeringValue, worldPos.y + centeringValue, 4.0f, PURPLE);
    
    DrawRectanglePro({worldPos.x + centeringValue, worldPos.y + centeringValue, MAP_SIZE, MAP_SIZE}, {MAP_SIZE * 0.5f, MAP_SIZE * 0.5f}, RAD2DEG * m_CurrentAngle, WHITE);


    // Draw path!
    if (!m_CurrentPath.empty()) {
			Pathfinding::Node* lastNode;
		 	for(auto node : m_CurrentPath) {
				float worldX = (node->m_PosX * MAP_SCALE) + centeringValue;
				float worldY = (node->m_PosY * MAP_SCALE) + centeringValue;
				DrawCircle(worldX, worldY, MAP_SCALE * 0.15f, BLUE);
				if (lastNode && node != *m_CurrentPath.begin()) {
					float lastWorldX = (lastNode->m_PosX * MAP_SCALE) + centeringValue;
					float lastWorldY = (lastNode->m_PosY * MAP_SCALE) + centeringValue;

					DrawLineDashed({worldX, worldY}, {lastWorldX, lastWorldY},8, 4, YELLOW);
				}
				lastNode = node;
			}
		}
}

void NPC::RecalculatePath()
{
    Pathfinding::Node* closestNode = Pathfinding::GetNodeInRadius(m_World, m_Position.x, m_Position.y, 0.5f);
    if (closestNode == nullptr) {
        return;
    } 
    
    Pathfinding::AStar pathfinder = {};
    m_CurrentResult = pathfinder.ResolvePath(closestNode, m_Goal);
    m_CurrentPath =  Pathfinding::ReconstructPath(closestNode, m_Goal, m_CurrentResult);

}
