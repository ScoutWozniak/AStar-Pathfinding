/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"

#include "nodes.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

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


	// Create a node for every empty square
	// Due to memory issues we cannot set neighbors here
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if(map[x][y] == 0) {
				// Set ID to unique value based on index position so we can get it again later
				curWorld.CreateNode(x,y,(y*8)*x);
			}
		}
	}

	// Generate the neighbors for each node
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if(map[x][y] == 0) {
				Pathfinding::Node* curNode = curWorld.GetNodeWithId((y*8)*x);
				// Loop through all neighbors here

			}
		}
	}

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();
		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		
		for (auto node : curWorld.m_Nodes) {
			DrawCircle(node.m_PosX * MAP_SCALE, node.m_PosY * MAP_SCALE, CIRCLE_RADIUS, RED);
		}

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}


	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
