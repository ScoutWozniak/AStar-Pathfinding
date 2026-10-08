#include "raylib.h"
#include "raymath.h"
#include "global.h"
#include "result.h"

// NPC who can follow pathfinding result
class NPC {
    public:
        NPC(Pathfinding::World* _world, Vector2 _pos) {
            m_World = _world;
            m_Position = _pos;
            m_Goal = nullptr;

            Initialise();
        }   

        Vector2 m_Position;
        
        void UpdateResult(std::vector<Pathfinding::Node*> _result) {
            m_CurrentPath = _result;
        }

        void UpdateGoal(Pathfinding::Node* _goal);

        void Initialise();

        void Update();

        void Draw();


    private:
        const float m_Speed = 3.0f;

        float m_CurrentAngle = 0.0f;
        float m_TargetAngle = 0.0f;

        Pathfinding::Node* m_Goal;

        Pathfinding::World* m_World;

        Pathfinding::PathResult m_CurrentResult;
        std::vector<Pathfinding::Node*> m_CurrentPath;

        void RecalculatePath();

};

namespace NPCUtils {
    inline void TryUpdateNPC(NPC* _npc, const MouseButton _btn, const Vector2 _mousePos, const int (_map)[MAP_SIZE][MAP_SIZE], const Camera2D* _cam, Pathfinding::World* _curWorld) {
        if (IsMouseButtonPressed(_btn)) {
            Vector2 mousePos = GetScreenToWorld2D(_mousePos, *_cam);
            mousePos /= MAP_SCALE;
            int gridPos[2] = {(int)mousePos.x, (int)mousePos.y};
            if (IsInBounds(gridPos) && _map[gridPos[1]][gridPos[0]] != 1) {
                _npc->UpdateGoal(_curWorld->GetNodeWithId(Pathfinding::GetIDFromPos(gridPos[0], gridPos[1])));
            }
        }
    }
}