#include "AStar.h"
#include "Map.h"
#include "SDL.h"

AStar::AStar(Map* m)
	:map(m),
	isSearching(false)
{
	graph = m->GetGraph();
}

AStar::~AStar()
{
}

bool AStar::IsSearching()
{
	return isSearching;
}

void AStar::Search(Node* start, Node* goal)
{
	isSearching = true;
	startNode = start;
	goalNode = goal;
	
	thread = SDL_CreateThread(SearchThread, "", this);
}

void AStar::OnSearchDone()
{
	isSearching = false;

	// Draw the shortest path
	for (auto p : pathFound)
	{
		map->SetPathMap(p->position, Map::RESULT_PATH_FOUND); 
	}
	
}

int AStar::SearchThread(void * data)
{
	AStar* astar = static_cast<AStar*>(data);

	if (!astar->startNode || !astar->goalNode)
	{
		astar->OnSearchDone();
		return 0;
	}

	std::vector<Node> keys = astar->map->GetGraph()->GetAllNodes();

	for (Node& key : keys) {
		astar->unvisited.push_back(astar->graph->GetNode(key.position));
		astar->distanceDict[astar->graph->GetNode(key.position)] = std::numeric_limits<float>::max();
		astar->actualDistanceDict[astar->graph->GetNode(key.position)] = std::numeric_limits<float>::max();
	}

	astar->distanceDict[astar->startNode] = 0;
	astar->actualDistanceDict[astar->startNode] = 0;
	astar->visited.clear();
	astar->predecessorDict.clear();
	astar->pathFound.clear();

	while (astar->unvisited.size() > 0) {
		Node* u = astar->GetClosestFromUnvisited();

		astar->map->SetPathMap(u->position, Map::SEARCH_IN_PROGRESS);
		SDL_Delay(200);

		if (u == nullptr) break;
		if (u == astar->goalNode) break;

		astar->visited.push_back(u);

		float shortest = std::numeric_limits<float>::max();

		for (Node* v : astar->map->GetGraph()->GetAdjacentNodes(u)) {
			if (std::find(astar->visited.begin(), astar->visited.end(), v) != astar->visited.end())
				continue;

			if (astar->distanceDict[v] > astar->actualDistanceDict[u] + astar->graph->GetDistance(u, v) + astar->graph->GetDistance(v, astar->goalNode)) {
				astar->actualDistanceDict[v] = astar->actualDistanceDict[u] + astar->graph->GetDistance(u, v);
				astar->distanceDict[v] = astar->actualDistanceDict[u] + astar->graph->GetDistance(u, v) + astar->graph->GetDistance(v, astar->goalNode);
			}

			if (shortest >= astar->graph->GetDistance(u, v)) {
				shortest = astar->graph->GetDistance(u, v);
				astar->predecessorDict[v] = u;
			}
		}
	}

	Node* p = astar->goalNode;
	do {
		astar->pathFound.push_back(p);
		p = astar->predecessorDict[p];
	} 
	
	while (p != astar->startNode);
	astar->pathFound.reserve(astar->pathFound.size());

	astar->OnSearchDone();
	return 0;
}

Node * AStar::GetClosestFromUnvisited()
{

	float shortest = std::numeric_limits<float>::max();
	Node* shortestNode = nullptr;

	for (Node* node : unvisited) {
		if (shortest > distanceDict[node]) {
			shortest = distanceDict[node];
			shortestNode = node;
		}
	}

	std::vector<Node*>::iterator it;
	for (it = unvisited.begin(); it != unvisited.end(); ) {
		if ((*it) == shortestNode) {
			it = unvisited.erase(it);
		} else {
			++it;
		}
	}

	return shortestNode;
}

void AStar::ValidateDistanceDict(Node * n)
{
	float max = std::numeric_limits<float>::max();
	if (distanceDict.find(n) == distanceDict.end())
	{
		distanceDict[n] = max;
	}
	if (actualDistanceDict.find(n) == actualDistanceDict.end())
	{
		actualDistanceDict[n] = max;
	}
}
