#include "nodes.h"
#include "global.h"


// Code related to creating nodes from the grid
namespace MapGen {
    void CreateWorldFromArray(Pathfinding::World* world, const int (mapGrid)[MAP_SIZE][MAP_SIZE]);
}