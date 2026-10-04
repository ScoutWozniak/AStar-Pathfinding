#include "raylib.h"
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

        Pathfinding::Node* m_Goal;

        Pathfinding::World* m_World;

        Pathfinding::PathResult m_CurrentResult;
        std::vector<Pathfinding::Node*> m_CurrentPath;

        void RecalculatePath();

};