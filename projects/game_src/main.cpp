/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "MazeMap.h"

#define SCREEN_STRIPES 32

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(800, 600, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	Vector2 renderResolution = {128,128};
	int horizontalStripes = 128;

	float playerAngle = 0.0f;
	Vector2 playerPos = {4,4};
	Vector2 cameraPlane = {0,0.66f};
	float FOV = 90.0f;

	bool perspective = false;

	float cameraPlaneScale = 0.66f;

	float playerSpeed = 1;
	float turnSpeed = 180;

	float rayStartAngle = -(FOV * 0.5f);
	float singleRayAngle = FOV / float(horizontalStripes);

	double time = 0;
	double oldTime = 0;
	
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{

		Vector2 playerForward = Vector2Rotate(Vector2UnitY, DEG2RAD * playerAngle);
		Vector2 playerRight = Vector2Rotate(Vector2UnitY, DEG2RAD * (playerAngle + 90));

		float angleAdd = 0;
		if (IsKeyDown(KEY_LEFT)) angleAdd -= turnSpeed;
		if (IsKeyDown(KEY_RIGHT)) angleAdd += turnSpeed;
		playerAngle = Wrap(playerAngle + angleAdd * GetFrameTime(), 0.0f, 360.0f);

		if (IsKeyDown(KEY_UP)) playerPos += playerForward * (playerSpeed * GetFrameTime());
		if (IsKeyDown(KEY_DOWN)) playerPos -= playerForward * (playerSpeed * GetFrameTime());


		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);
		for (int x = 0; x < horizontalStripes; x++) {			
			

			Vector2 rayDir;
			// Camera plane trumps perspective as it looks a lot better!
			if (perspective) {
				float rayAngle = rayStartAngle + (singleRayAngle * x);
				rayDir = Vector2Normalize(Vector2Rotate(playerForward, DEG2RAD * rayAngle));
			}
			else {

				float cameraX = 2 * x / float(horizontalStripes) - 1;
				rayDir = playerForward + ((playerRight * cameraPlaneScale) * cameraX);
			}
			

			Vector2 mapPos = {int(playerPos.x), int(playerPos.y)};
			Vector2 sideDist;
			Vector2 deltaDist;
			deltaDist.x = (rayDir.x == 0) ? 1e30 : abs(1 / rayDir.x);
			deltaDist.y = (rayDir.y == 0) ? 1e30 : abs(1 / rayDir.y);
			float perpWallDist;

			Vector2 step;

			int hit = 0;
			int side;
			if (rayDir.x < 0) {
				step.x = -1;
				sideDist.x = (playerPos.x - mapPos.x) * deltaDist.x;
			}
			else {
				step.x = 1;
				sideDist.x = (mapPos.x + 1 - playerPos.x) * deltaDist.x;
			}
			if (rayDir.y < 0) {
				step.y = -1;
				sideDist.y = (playerPos.y - mapPos.y) * deltaDist.y;
			}
			else {
				step.y = 1;
				sideDist.y = (mapPos.y + 1 - playerPos.y) * deltaDist.y;
			}

			while (hit == 0) {
				if (sideDist.x < sideDist.y) {
					sideDist.x += deltaDist.x;
					mapPos.x += step.x;
					side = 0;
				}
				else {
					sideDist.y += deltaDist.y;
					mapPos.y += step.y;
					side = 1;
				}
				if (worldMap[int(mapPos.x)][int(mapPos.y)] != 0) { hit = 1;}
			}

			if (side == 0) perpWallDist = (sideDist.x	- deltaDist.x);
			else perpWallDist = (sideDist.y) - deltaDist.y;

			float lineHeight = GetScreenHeight() / perpWallDist;

			float drawStart = -lineHeight * 0.5f + GetScreenHeight() * 0.5f;
			if (drawStart < 0) drawStart = 0;
			float drawEnd = lineHeight * 0.5f + GetScreenHeight() * 0.5f;
			if (drawEnd >= GetScreenHeight()) drawEnd = GetScreenHeight() - 1;
			float rectSize = float(GetScreenWidth()) / float(horizontalStripes);
			float screenPos = x * rectSize;
			

			DrawRectangle(screenPos, drawStart, rectSize, drawEnd - drawStart, RED);
		}

		// Draw Debug View
		Vector2 debugSize = {256,256};
		float size = debugSize.x / MAP_WIDTH;
		DrawRectangle(0,0,debugSize.x,debugSize.y, GRAY);
		for (int x = 0; x < MAP_WIDTH; x++) {
			for (int y = 0; y < MAP_HEIGHT; y++) {
				if (worldMap[x][y] != 0) {
					DrawRectangle(x * size, y * size, size, size, BLUE);
				}
			}
		}
		Vector2 playerScreenPos = playerPos * size;

		for (int strip = 0; strip < horizontalStripes; strip++) {
			DrawLine(playerScreenPos.x, playerScreenPos.y, playerScreenPos.x + (playerForward.x * size), playerScreenPos.y + (playerForward.y * size), YELLOW);
		}
		DrawCircle(playerScreenPos.x, playerScreenPos.y, size * 0.5f, GREEN);
		
		

		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}


	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
