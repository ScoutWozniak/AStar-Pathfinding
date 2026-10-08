#include "nodes.h"
#include "global.h"


// Code related to creating nodes from the grid
namespace MapGen {


    void CreateWorldFromArray(Pathfinding::World* world, const int (mapGrid)[MAP_SIZE][MAP_SIZE]);

    // Create a node for every empty square
	// Due to memory issues we cannot set neighbors here
    void SetupNodePos(Pathfinding::World* world, const int (mapGrid)[MAP_SIZE][MAP_SIZE]);

    // Generate the neighbors for each node
	// NOTE: Idealy this can be cut down, we do far too many loops here
	// Potentially instead of storing pointers we can store IDs (as we know them from the start)
	// Then whenever the neighbours need to be accessed we can check if they are valid and pass them through via the int alone?
    void SetupNodeNeighbors(Pathfinding::World* world, const int (mapGrid)[MAP_SIZE][MAP_SIZE]);
}