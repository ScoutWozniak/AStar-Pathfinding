#include "camController.h"
#include "raymath.h"
#include "global.h"

void CameraController::Update()
{

    // CAMERA CONTROLS --------------------------------
	if (IsKeyDown(KEY_LEFT)) m_Cam.target -= {1.0f, 0.0f};
	if (IsKeyDown(KEY_RIGHT)) m_Cam.target += {1.0f, 0.0f};
	if (IsKeyDown(KEY_UP)) m_Cam.target -= {0.0f, 1.0f};
	if (IsKeyDown(KEY_DOWN)) m_Cam.target += {0.0f, 1.0f};

    // Zooming
    if (GetMouseWheelMove() != 0) {
			float zoomLevel = (GetMouseWheelMove() * 0.25f);
			m_Cam.zoom = Clamp(m_Cam.zoom + zoomLevel, 0.1f, 2.0f);
	}
}

void CameraController::SetupCamera()
{
    Vector2 screenSize = (Vector2{(float)GetScreenWidth(), (float)GetScreenHeight()});

    m_Cam = {};
    m_Cam.target =Vector2One() * MAP_SIZE * MAP_SCALE * 0.5f;
	m_Cam.offset = screenSize * 0.5f;
	m_Cam.zoom = 1.0f;
	m_Cam.rotation = 0.0f;

}