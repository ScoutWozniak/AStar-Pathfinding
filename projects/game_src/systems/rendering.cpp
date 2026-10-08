#include "rendering.h"

namespace Rendering {

    bool drawConnections = false;
    bool drawRawNodes = false;

    void DrawNodes(Pathfinding::World* _world) {
        for (auto node : _world->m_Nodes) {
                
            float screenPos[2] = {node.m_PosX * MAP_SCALE, node.m_PosY * MAP_SCALE};
            screenPos[0] = screenPos[0] + centeringValue;
            screenPos[1] = screenPos[1] + centeringValue;

                //Draw connections between nodes
            if (drawConnections) {
                for (auto connection : node.m_Neighbors) {
                    DrawLine(screenPos[0], screenPos[1],
                    (connection->m_PosX * MAP_SCALE) + centeringValue, (connection->m_PosY * MAP_SCALE) + centeringValue, YELLOW);
                }
            }
                
            if (drawRawNodes) {
                DrawCircle(screenPos[0], screenPos[1], MAP_SCALE * 0.25f, RED);
            }
                
        }
    }

    void DrawMap(const int (_mapGrid)[MAP_SIZE][MAP_SIZE]) {
        for (int x = 0; x < MAP_SIZE; x++) {
			for (int y = 0; y < MAP_SIZE; y++) {
				Vector2 renderPos = WorldPosToRenderPos(x,y);
				
				
				if (_mapGrid[y][x] == 1) {
					DrawRectangle(renderPos.x,renderPos.y,MAP_SCALE, MAP_SCALE, Color{82,58,121,255} );
				}
				else {
					DrawRectangle(renderPos.x,renderPos.y,MAP_SCALE, MAP_SCALE, Color{89, 156, 156,255} );
				}
			}
		}
    }

    void DrawWorld(Pathfinding::World* _world, const int (_mapGrid)[MAP_SIZE][MAP_SIZE])
    {
        DrawMap(_mapGrid);
        DrawNodes(_world);
    }

    

    void ToggleDrawNodes() {
        drawRawNodes = !drawRawNodes;
    }

    void ToggleDrawConnections() {
        drawConnections = !drawConnections;
    }
}

