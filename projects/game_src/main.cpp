/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"
#include "basePathfinding.h"
#include "global.h"
#include "npc.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <string>

// Basic map, 1 = Wall, 0 = Nothing
const int map[MAP_SIZE][MAP_SIZE] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
	{1,0,1,1,1,1,1,1,1,0,0,0,1,0,0,1},
	{1,0,0,0,0,1,0,1,0,0,0,0,1,0,0,1},
	{1,0,0,0,1,1,0,1,1,1,1,0,1,0,0,1},
	{1,0,0,0,0,1,0,1,0,0,0,0,1,0,0,1},
	{1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1},
	{1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1},
	{1,0,0,0,1,1,0,0,0,0,0,0,0,1,0,1},
	{1,0,0,0,1,1,0,0,0,0,0,0,0,1,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

const float CIRCLE_RADIUS = 16;

// If any are true then we are out of bounds
bool IsInBounds(int newPos[2]) {
	return !(newPos[0] >= MAP_SIZE || newPos[0] < 0 || newPos[1] >= MAP_SIZE || newPos[1] < 0);
}

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(800, 600, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");	

	Texture2D bgTexture = LoadTexture("pattern_039.png");
	Texture2D groundTex = LoadTexture("ground_06.png");
	Texture2D wallTex = LoadTexture("block_01.png");

	Pathfinding::World curWorld = {};

	curWorld.NewWorld();

	// Create a node for every empty square
	// Due to memory issues we cannot set neighbors here
	for (int x = 0; x < MAP_SIZE; x++) {
		for (int y = 0; y < MAP_SIZE; y++) {
			if(map[y][x] == 0) {
				// Set ID to unique value based on index position so we can get it again later
				curWorld.CreateNode(x,y,Pathfinding::GetIDFromPos(x,y));
			}
		}
	}

	const int nextdoorCheck[4][2] = {
		{0,-1}, // Up
		{0,1}, // Down
		{-1,0}, // Left
		{1,0}, // Right
	};

	// Generate the neighbors for each node
	// NOTE: Idealy this can be cut down, we do far too many loops here
	// Potentially instead of storing pointers we can store IDs (as we know them from the start)
	// Then whenever the neighbours need to be accessed we can check if they are valid and pass them through via the int alone?
	for (int x = 0; x < MAP_SIZE; x++) {
		for (int y = 0; y < MAP_SIZE; y++) {
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
	Pathfinding::Node* curGoal = curWorld.GetNodeWithId(Pathfinding::GetIDFromPos(1,1));
	Pathfinding::Node* curStart = curWorld.GetNodeWithId(Pathfinding::GetIDFromPos(6,4));


	Pathfinding::Node* curGoal2 = curWorld.GetNodeWithId(Pathfinding::GetIDFromPos(15, 1));

	Vector2 screenSize = (Vector2{(float)GetScreenWidth(), (float)GetScreenHeight()});

	Camera2D cam;
	cam.target =Vector2One() * MAP_SIZE * MAP_SCALE * 0.5f;
	cam.offset = screenSize * 0.5f;
	cam.zoom = 1.0f;
	cam.rotation = 0.0f;

	bool drawConnections = false;
	bool drawRawNodes = true;

	NPC npc = {&curWorld, {curStart->m_PosX, curStart->m_PosY}};
	npc.UpdateGoal(curGoal);

	NPC npc2 = {&curWorld, {3, 3}};
	npc2.UpdateGoal(curGoal2);

	float bgScrollState = 0;

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		bgScrollState = Wrap(bgScrollState + GetFrameTime() * 25.0f, 0.0f, (float)bgTexture.width);

		// GOAL SETTING CONTROLS
		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
			Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), cam);
			mousePos /= MAP_SCALE;
			int gridPos[2] = {mousePos.x, mousePos.y};
			if (IsInBounds(gridPos) && map[gridPos[1]][gridPos[0]] != 1) {
				curGoal = curWorld.GetNodeWithId(Pathfinding::GetIDFromPos(gridPos[0], gridPos[1]));
				npc.UpdateGoal(curGoal);
			}
		}

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), cam);
			mousePos /= MAP_SCALE;
			int gridPos[2] = {mousePos.x, mousePos.y};
			if (IsInBounds(gridPos) && map[gridPos[1]][gridPos[0]] != 1) {
				curGoal2 = curWorld.GetNodeWithId(Pathfinding::GetIDFromPos(gridPos[0], gridPos[1]));
				npc2.UpdateGoal(curGoal2);
			}
		}

		// CAMERA CONTROLS --------------------------------
		if (IsKeyDown(KEY_LEFT)) cam.target -= {1.0f, 0.0f};
		if (IsKeyDown(KEY_RIGHT)) cam.target += {1.0f, 0.0f};
		if (IsKeyDown(KEY_UP)) cam.target -= {0.0f, 1.0f};
		if (IsKeyDown(KEY_DOWN)) cam.target += {0.0f, 1.0f};

		if (GetMouseWheelMove() != 0) {
			float zoomLevel = (GetMouseWheelMove() * 0.25f);
			cam.zoom = Clamp(cam.zoom + zoomLevel, 0.1f, 2.0f);
		}

		// DEBUG DRAW CONTROLS ------------------------------
		if (IsKeyPressed(KEY_ONE)) drawConnections = !drawConnections;
		if (IsKeyPressed(KEY_TWO)) drawRawNodes = !drawRawNodes;
		
		npc.Update();
		npc2.Update();
		
		// drawing
		BeginDrawing();
		ClearBackground(GRAY);
		DrawTextureTiled(bgTexture, {0, 0, (float)bgTexture.width, (float)bgTexture.height}, 
		{-bgScrollState,-bgScrollState,(float)GetScreenWidth() + bgTexture.width, (float)GetScreenHeight() + bgTexture.height},{0,0},0,1.0f,WHITE);

		BeginMode2D(cam);


		// Drawing world
		for (int x = 0; x < MAP_SIZE; x++) {
			for (int y = 0; y < MAP_SIZE; y++) {
				Vector2 renderPos = WorldPosToRenderPos(x,y);
				
				
				if (map[y][x] == 1) {
					DrawRectangle(renderPos.x,renderPos.y,MAP_SCALE, MAP_SCALE, Color{82,58,121,255} );
				}
				else {
					DrawRectangle(renderPos.x,renderPos.y,MAP_SCALE, MAP_SCALE, Color{89, 156, 156,255} );
				}
			}
		}

		
		// Drawing nodes + connections
		for (auto node : curWorld.m_Nodes) {
			
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
				DrawText((std::to_string( node.m_Id).c_str() ), screenPos[0], screenPos[1], 8, BLACK);
			}
			
		}

		npc.Draw();
		npc2.Draw();

		EndMode2D();

		DrawFPS(0,0);
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}
	UnloadTexture(bgTexture);
	UnloadTexture(groundTex);
	UnloadTexture(wallTex);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
