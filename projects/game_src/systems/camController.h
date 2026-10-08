#include "raylib.h"

class CameraController {
    public:
    CameraController() {
        SetupCamera();
    }

    Camera2D m_Cam;

    void Update();

    private:
    
    void SetupCamera();
};