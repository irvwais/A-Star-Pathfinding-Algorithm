#pragma once
#include "SDL_thread.h"
#include "Map.h"
#include "Graph.h"
#include <vector>
#include <map>

class AStar
{
	Map* map;
	Graph* graph;
	SDL_Thread* thread;

	bool isSearching;
	
	std::vector<Node*> visited;
	std::vector<Node*> unvisited;
	

	std::map<Node*, Node*> predecessorDict;
	std::map<Node*, float> distanceDict;
	std::map<Node*, float> actualDistanceDict;

	Node* startNode;
	Node* goalNode;
	std::vector<Node*> pathFound;

public:
	AStar(Map* map);
	~AStar();

	bool IsSearching();
	void Search(Node* start, Node* goal);

	void OnSearchDone(); // callback when the search has been done
	static int SearchThread(void* data); // thread function
private:
	Node* GetClosestFromUnvisited();
	void ValidateDistanceDict(Node* n);
};

