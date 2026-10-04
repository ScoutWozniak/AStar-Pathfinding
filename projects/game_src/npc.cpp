#include "npc.h"
#include "global.h"
#include "iostream"

void NPC::Initialise()
{
    std::cout << m_Goal.m_Id << std::endl;
}

void NPC::Update()
{

}

void NPC::Draw()
{
    Vector2 worldPos = WorldPosToRenderPos(m_Position);
    DrawCircle(worldPos.x + centeringValue, worldPos.y + centeringValue, 4.0f, PURPLE);
}