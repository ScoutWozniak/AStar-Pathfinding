# AI & Programming - A* Pathfinding
Implemenation of the A* pathfinding algorithm.  Hooked into raylib for displaying + demo-ing using a moving NPC.


## Repo projects
### Pathfinding
Pathfinding algorithims and such only using the standard libraries.  Currently contains the working A* algorithm and a basic node system for 2D worlds.
Only references standard c++ libraries so it can be used elsewhere as a generic library for pathfinding.
The API is set up to allow for other types of pathfinding to be implemented however they are out of the scope of this project currently so only A* is implemented.

### Game
Implementation of the pathfinding algorithm using Raylib.  Generates a map from a grid and allows the user to dynamically choose what nodes are the goal and the start.


## Pathfinding Algorithm
Algorithim heavily inspired from these sources:
https://youtu.be/icZj67PTFhc?si=R5opCVVgvSemC4HF
https://www.redblobgames.com/pathfinding/a-star/implementation.html#cpp-astar
https://youtu.be/-L-WgKMFuhE?si=sP0Cw-npwzDKJoeE

### Map + Nodes
A "map" in A* is represented as a series of nodes that link to each other (as seen in the Node class).  These nodes hold a position in 2D space as well as pointers to all connecting neighbors.  These are then held together in the World class which helps with creating nodes and finding nodes.  Each node has an ID, while not explicitly part of A*, this is implemented purely to make assembling the map via a grid easier.
While this implementation is not constrained to the grid (all calculations are done using real vector math so any position will work), this implementation is using a grid for ease of showcasing.

### Heuristic
By default the heuristic is calculated via a straight line from the starting position to the goal.  This can end up being innacurate in some cases when dealing with more complex conditions, but for now this works fine as at most nodes will only be a distance of 1 apart from each other (diagonal movement would work too, but in this case I have only connected neighbors directly above, bellow or to the side).

### Calculating Path
A* works by calculating the cost of a node from both the start and the goal.  Cost is simply the value of traveling from one node to the other.  In this implementation cost is simply the distance between the nodes (normally just 1), however in theory special nodes can be assigned a much higher cost than others (adding onto distance) so whilst not completely blocked off from the pathfinding it is less likely to go through there for the sake of efficiency.  A* iterates through a queue of nodes starting from the first node.  It then calculates the cost of each neighbour of that original node and if it has not been calculated yet (or has a different cost than first calculated), it is added to the queue to check later.  The node with the next priority in the queue is chosen and looked at until ultimately the queue is empty.  Whilst this is occuring, each neighbor is placed into an unordered map that links to the node that checked its values.  At the end this unordered map acts as the result.  It can be reconstructed into an ordered list by working backwards from the goal node.
