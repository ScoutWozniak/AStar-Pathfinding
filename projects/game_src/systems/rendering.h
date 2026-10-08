#include "nodes.h"
#include "global.h"

namespace Rendering {
    void DrawWorld(Pathfinding::World* _world, const int (_mapGrid)[MAP_SIZE][MAP_SIZE]);

    void ToggleDrawNodes();
    void ToggleDrawConnections();
}