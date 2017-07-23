#include <iostream>
#include <math.h>
#include <algorithm>
#include <string>
#include <sstream>

#include "a_star.hpp"

using namespace std;

// Calculates and returns the fcost
int AStar::getFCost(Node* currentNode) {
	// TODO Step1. Return fCost as a sum of gCost and hCost
    return currentNode->gCost + currentNode->hCost;
}

// Computes the Euclidean distance between two nodes and scales it by 10 to avoid floats
int AStar::getHDistance(Node* A, Node* B) {
	// TODO Step2. Return the Euclidian distance scaled by 10
    return (sqrt(pow((A->posX-B->posX), 2)+pow((A->posY-B->posY), 2)))*10;
}

// Iterates through the node's parents and creates a list of the shortest path to take
list<Node*> AStar::retracePath (Node* startNode, Node* endNode) {
	list<Node*> path;
	Node* currentNode = endNode;

	while (currentNode != startNode) {
		path.push_front(currentNode);
		currentNode = currentNode->parent;
	}
	return path;
}

// For the current node, cnode, discovers all walkable neighbours, and adds them to the neighbours list
list<Node*> AStar::getNeighbours(Node* cnode, Node* grid[Y_LENGTH][X_LENGTH]) {
	list<Node*> neighbours;

	// TODO Step3. Add walkable neighbours to the neighbours list
	// Step3.1 Iterate from (cnode.y - 1) to (cnode.y + 1)
	// Step3.2 Iterate from (cnode.x - 1) to (cnode.x + 1)
	// Step3.3 Add neighbours that are not outside the world bounds and that are walkable


    int X = cnode->posX, Y = cnode->posY;
    for (int y = max (Y-1, 0); y < min (Y+2, Y_LENGTH); y ++) {
        for (int x = max (X-1, 0); x < min (X+2, X_LENGTH); x ++) {
            if( (x!=X || y != Y) && grid[y][x]->walkable )
                neighbours.push_front(grid[y][x]);
        }
    }

	return neighbours;
}

bool areNeighbors (Node* A, Node* B){
	if (A == NULL || B == NULL) return false;
	else if (A->posX == B->posX && A->posY == B->posY) return false;
	else return (abs(A->posX - B->posX) <= 1 && abs(A->posY - B->posY) <= 1 ) ;
}

// Outputs the discovered path as a string
string AStar::getPathString(list<Node*> path) {
	stringstream outputStream;
	for (list<Node*>::iterator it = path.begin(); it != path.end(); ++it) {
		outputStream << "(" << (*it)->posX << "," << (*it)->posY << ")" << endl;
	}
	return outputStream.str();
}

// Outputs path distance
int AStar::getPathDistance(list<Node*> path) {
	return path.size() > 0 ? path.back() -> fCost : 0;
}

// Finds shortest path between startNode and endNode using A* algorithm
list<Node*> AStar::findPath(Node* world[Y_LENGTH][X_LENGTH], Node* startNode, Node* endNode) {
	list<Node*> openSet; // list of nodes not yet evaluated
	list<Node*> closedSet; // list of nodes already evaluated
	Node* currentNode;

	startNode->gCost = 0;
	startNode->hCost = getHDistance(startNode, endNode);
	startNode->fCost = startNode->hCost;

	openSet.push_back(startNode); // insert the starting node at the beginning of the open set
	while(!openSet.empty()) {
		// TODO Step4. Find a node in the openSet that has the smallest fCost
		// If there is a conflict, select the node with the smaller hCost
		// Use <list> iterator to iterate through the list; see sample iterator code below

        int fCost = 999, hCost = 999;
        for(list<Node*>::iterator it = openSet.begin(); it != openSet.end(); it++) {
            Node* node = (*it);
            //cout << "This Is YOU DAMN hCOST:" <<node->hCost<< ", gCost:" <<node->gCost << ", and fCost:" << node->fCost<<endl;
            if (node-> fCost < fCost){
                fCost = node-> fCost;
                hCost = node-> hCost;
                currentNode = node;
            } else if (node-> fCost == fCost && node-> hCost < hCost){
                hCost = node-> hCost;
                currentNode = node;
            }
        }


		// TODO Step5. Remove the found node from the open list and insert it into closed list
		openSet.remove (currentNode);
        closedSet.push_back (currentNode);

		// TODO Step6. Get a list of walkable neighbours for the current node
        list<Node*> neighbours = getNeighbours (currentNode, world);


		// TODO Step7. Iterate through the neighbours list and add matching neighbours to the open list
		for(list<Node*>::iterator it = neighbours.begin(); it != neighbours.end(); it++) {
			// Step7.1. Check if the current neighbour is already in the closed list; if it is, skip it
			Node* curNeibor = (*it);
            if (std::find(closedSet.begin(), closedSet.end(), curNeibor) != closedSet.end()) continue;


            // Step7.2. Compute gCost from the start node for the current neighbour


            // If that cost is less than previously computed gCost, update the neighbour's parent to the current node, and
			// update gCost, hCost, and fCost values for the neighbour to match the current node
			// Use getHDistance to get the cost from the current node to the current neighour

            int temGCost = currentNode->gCost + getHDistance(currentNode,curNeibor);

            curNeibor->parent = currentNode;
            curNeibor->hCost = getHDistance (curNeibor, endNode);
            curNeibor->gCost = temGCost;
            curNeibor->fCost = getFCost(curNeibor);

            for(list<Node*>::iterator it = closedSet.begin(); it != closedSet.end(); it++) {
				if (areNeighbors((*it),curNeibor)){
					if ((*it)->gCost < curNeibor->parent->gCost){
						curNeibor ->parent = (*it);
						curNeibor ->gCost = getHDistance (curNeibor, (*it)) + (*it)->gCost;
            			curNeibor ->fCost = getFCost(curNeibor);
					}
				}
			}

            //Something is not Right here:
			/*

			*/
			//Basically this fail test case 4 at color (4,2) sould go diagonally(directly) to (3,3). However, this is going (4,2)->(3,2) -> (3,3), which in turn result in an additional 10 m distance to be added.

            openSet.push_back (curNeibor);
 		}

 		// TODO Step8. Check if the current node is the end node; if it is, return the retraced path from start to end
        if (currentNode == endNode) return retracePath (startNode, endNode);

	}

	list<Node*> empty;
	return empty;
}
