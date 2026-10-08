#include "map_gen.h"

namespace MapGen {
    const int nextdoorCheck[4][2] = {
		{0,-1}, // Up
		{0,1}, // Down
		{-1,0}, // Left
		{1,0}, // Right
	};

    
    // Create a node for every empty square
	// Due to memory issues we cannot set neighbors here
    void SetupNodePos(Pathfinding::World* _world, const int (_mapGrid)[MAP_SIZE][MAP_SIZE]) {
        for (int x = 0; x < MAP_SIZE; x++) {
            for (int y = 0; y < MAP_SIZE; y++) {
                if(_mapGrid[y][x] == 0) {
                    // Set ID to unique value based on index position so we can get it again later
                    _world->CreateNode(x,y,Pathfinding::GetIDFromPos(x,y));
                }
            }
        }
    }

    // Generate the neighbors for each node
	// NOTE: Idealy this can be cut down, we do far too many loops here
	// Potentially instead of storing pointers we can store IDs (as we know them from the start)
	// Then whenever the neighbours need to be accessed we can check if they are valid and pass them through via the int alone?
    void SetupNodeNeighbors(Pathfinding::World* _world, const int (_mapGrid)[MAP_SIZE][MAP_SIZE]) {
        for (int x = 0; x < MAP_SIZE; x++) {
            for (int y = 0; y < MAP_SIZE; y++) {
                if(_mapGrid[y][x] == 0) {
                    Pathfinding::Node* curNode = _world->GetNodeWithId(Pathfinding::GetIDFromPos(x, y));
                    int gridPos[2] = {curNode->m_PosX, curNode->m_PosY };
                    // Loop through all neighbors here
                    for (int i = 0; i < 4; i++) {
                        int newPos[2] = {gridPos[0] + nextdoorCheck[i][0], gridPos[1] + nextdoorCheck[i][1]};
                        if (IsInBounds(newPos) && (_mapGrid[newPos[1]][newPos[0]]) != 1) {
                            int nodeId = Pathfinding::GetIDFromPos(newPos[0], newPos[1]);
                            Pathfinding::Node* connectingNode = _world->GetNodeWithId(nodeId);
                            if (connectingNode) {
                                curNode->m_Neighbors.emplace_back(connectingNode);
                            }
                        }
                    }
                }
            }
        }
    }

    void CreateWorldFromArray(Pathfinding::World* _world, const int (_mapGrid)[MAP_SIZE][MAP_SIZE]){
        SetupNodePos(_world,_mapGrid);
        SetupNodeNeighbors(_world, _mapGrid);
    }
}

