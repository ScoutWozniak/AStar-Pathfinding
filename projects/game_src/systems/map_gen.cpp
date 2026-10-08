#include "map_gen.h"

namespace MapGen {
    const int nextdoorCheck[4][2] = {
		{0,-1}, // Up
		{0,1}, // Down
		{-1,0}, // Left
		{1,0}, // Right
	};

    void CreateWorldFromArray(Pathfinding::World* world, const int (mapGrid)[MAP_SIZE][MAP_SIZE]){
        SetupNodePos(world,mapGrid);
        SetupNodeNeighbors(world, mapGrid);
    }

    void SetupNodePos(Pathfinding::World* world, const int (mapGrid)[MAP_SIZE][MAP_SIZE]) {
        for (int x = 0; x < MAP_SIZE; x++) {
            for (int y = 0; y < MAP_SIZE; y++) {
                if(mapGrid[y][x] == 0) {
                    // Set ID to unique value based on index position so we can get it again later
                    world->CreateNode(x,y,Pathfinding::GetIDFromPos(x,y));
                }
            }
        }
    }
    void SetupNodeNeighbors(Pathfinding::World* world, const int (mapGrid)[MAP_SIZE][MAP_SIZE]) {
        for (int x = 0; x < MAP_SIZE; x++) {
            for (int y = 0; y < MAP_SIZE; y++) {
                if(mapGrid[y][x] == 0) {
                    Pathfinding::Node* curNode = world->GetNodeWithId(Pathfinding::GetIDFromPos(x, y));
                    int gridPos[2] = {curNode->m_PosX, curNode->m_PosY };
                    // Loop through all neighbors here
                    for (int i = 0; i < 4; i++) {
                        int newPos[2] = {gridPos[0] + nextdoorCheck[i][0], gridPos[1] + nextdoorCheck[i][1]};
                        if (IsInBounds(newPos) && (mapGrid[newPos[1]][newPos[0]]) != 1) {
                            int nodeId = Pathfinding::GetIDFromPos(newPos[0], newPos[1]);
                            Pathfinding::Node* connectingNode = world->GetNodeWithId(nodeId);
                            if (connectingNode) {
                                curNode->m_Neighbors.emplace_back(connectingNode);
                            }
                        }
                    }
                }
            }
        }
    }
}

