/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"
#include "basePathfinding.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <string>

#define SCREEN_STRIPES 32

// Basic map, 1 = Wall, 0 = Nothing
const int map[8][8] = {
	{1,1,1,1,1,1,1,1},
	{1,0,0,0,1,1,0,1},
	{1,0,1,0,1,0,0,1},
	{1,0,1,0,1,0,0,1},
	{1,0,1,0,1,1,0,1},
	{1,1,1,0,0,0,0,1},
	{1,1,1,1,1,0,0,1},
	{1,1,1,1,1,1,1,1},
};

const int MAP_SCALE = 64;
const float CIRCLE_RADIUS = 16;

// If any are true then we are out of bounds
bool IsInBounds(int newPos[2]) {
	return !(newPos[0] >= 8 || newPos[0] < 0 || newPos[1] >= 8 || newPos[1] < 0);
}

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(800, 600, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");	

	Pathfinding::World curWorld = {};

	curWorld.NewWorld();

	Pathfinding::Node* curGoal;
	Pathfinding::Node* curStart;


	// Create a node for every empty square
	// Due to memory issues we cannot set neighbors here
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if(map[y][x] == 0) {
				// Set ID to unique value based on index position so we can get it again later
				curWorld.CreateNode(x,y,Pathfinding::GetIDFromPos(x,y));
			}
		}
	}

	int nextdoorCheck[4][2] = {
		{0,-1}, // Up
		{0,1}, // Down
		{-1,0}, // Left
		{1,0}, // Right
	};

	// Generate the neighbors for each node
	// NOTE: Idealy this can be cut down, we do far too many loops here
	// Potentially instead of storing pointers we can store IDs (as we know them from the start)
	// Then whenever the neighbours need to be accessed we can check if they are valid and pass them through via the int alone?
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if(map[y][x] == 0) {
				Pathfinding::Node* curNode = curWorld.GetNodeWithId(Pathfinding::GetIDFromPos(x, y));
				int gridPos[2] = {curNode->m_PosX, curNode->m_PosY };
				// Loop through all neighbors here
				for (int i = 0; i < 4; i++) {
					int newPos[2] = {gridPos[0] + nextdoorCheck[i][0], gridPos[1] + nextdoorCheck[i][1]};
					if (IsInBounds(newPos) && map[newPos[1]][newPos[0]] != 1) {
						int nodeId = Pathfinding::GetIDFromPos(newPos[0], newPos[1]);
						Pathfinding::Node* connectingNode = curWorld.GetNodeWithId(nodeId);
						if (connectingNode) {
							curNode->m_Neighbors.emplace_back(connectingNode);
						}
					}
				}
			}
		}
	}

	// Setting up the temporary goal here
	curStart = curWorld.GetNodeWithId(Pathfinding::GetIDFromPos(1,1));
	curGoal = curWorld.GetNodeWithId(Pathfinding::GetIDFromPos(6,4));

	Pathfinding::AStar pathfinder = {};
	Pathfinding::PathResult result = pathfinder.ResolvePath(curStart, curGoal);
	std::vector<Pathfinding::Node*> path = Pathfinding::ReconstructPath(curStart, curGoal, result);

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();
		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(GRAY);

		
		for (auto node : curWorld.m_Nodes) {
			float centeringValue = MAP_SCALE * 0.5f;
			float screenPos[2] = {node.m_PosX * MAP_SCALE, node.m_PosY * MAP_SCALE};
			screenPos[0] = screenPos[0] + centeringValue;
			screenPos[1] = screenPos[1] + centeringValue;

			for (auto connection : node.m_Neighbors) {
				DrawLine(screenPos[0], screenPos[1],
					 (connection->m_PosX * MAP_SCALE) + centeringValue, (connection->m_PosY * MAP_SCALE) + centeringValue, YELLOW);
				
			}
			Color nodeColor = node.m_Id == curStart->m_Id ? BLUE : RED;
			nodeColor = node.m_Id == curGoal->m_Id ? GREEN : nodeColor;

			DrawCircle(screenPos[0], screenPos[1], CIRCLE_RADIUS, nodeColor);
			DrawText((std::to_string( node.m_Id).c_str() ), screenPos[0], screenPos[1], 16, BLACK);
		}

		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				if (map[y][x] == 1) {
					DrawRectangle(x*MAP_SCALE, y*MAP_SCALE, MAP_SCALE, MAP_SCALE, BLACK);
				}
			}
		}

		if (!result.results.empty()) {
		 	for(auto node : path) {
				DrawCircle(node->m_PosX * MAP_SCALE, node->m_PosY * MAP_SCALE, 8.0f, BLUE);
			}
		}

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}


	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
