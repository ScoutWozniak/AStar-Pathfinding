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
#include "systems/map_gen.h"
#include "systems/camController.h"
#include "systems/rendering.h"


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



// Bloated main function?  Why not!
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


	MapGen::CreateWorldFromArray(&curWorld, map);
	
	// Setting up the temporary goals here
	Pathfinding::Node* curGoal = curWorld.GetNodeWithId(Pathfinding::GetIDFromPos(1,1));
	Pathfinding::Node* curGoal2 = curWorld.GetNodeWithId(Pathfinding::GetIDFromPos(15, 1));
	

	CameraController camController = {};


	bool drawConnections = false;
	bool drawRawNodes = true;

	NPC npc = {&curWorld, {2,2}};
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
			Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camController.m_Cam);
			mousePos /= MAP_SCALE;
			int gridPos[2] = {mousePos.x, mousePos.y};
			if (IsInBounds(gridPos) && map[gridPos[1]][gridPos[0]] != 1) {
				curGoal = curWorld.GetNodeWithId(Pathfinding::GetIDFromPos(gridPos[0], gridPos[1]));
				npc.UpdateGoal(curGoal);
			}
		}
		
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camController.m_Cam);
			mousePos /= MAP_SCALE;
			int gridPos[2] = {mousePos.x, mousePos.y};
			if (IsInBounds(gridPos) && map[gridPos[1]][gridPos[0]] != 1) {
				curGoal2 = curWorld.GetNodeWithId(Pathfinding::GetIDFromPos(gridPos[0], gridPos[1]));
				npc2.UpdateGoal(curGoal2);
			}
		}

		

		camController.Update();

		

		// DEBUG DRAW CONTROLS ------------------------------
		if (IsKeyPressed(KEY_ONE)) Rendering::ToggleDrawNodes();
		if (IsKeyPressed(KEY_TWO)) Rendering::ToggleDrawConnections();
		
		npc.Update();
		npc2.Update();
		
		// drawing
		BeginDrawing();
		ClearBackground(GRAY);

		DrawTextureTiled(bgTexture, {0, 0, (float)bgTexture.width, (float)bgTexture.height}, 
		{-bgScrollState,-bgScrollState,(float)GetScreenWidth() + bgTexture.width, (float)GetScreenHeight() + bgTexture.height},{0,0},0,1.0f,WHITE);

		BeginMode2D(camController.m_Cam);


		// Drawing world
		

		Rendering::DrawWorld(&curWorld, map);

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
