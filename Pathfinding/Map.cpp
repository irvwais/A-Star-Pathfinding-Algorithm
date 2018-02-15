#include "Map.h"
#include <SDL.h>
#include <memory.h>


Map::Map()
	: startPos(-1, -1),
	endPos(-1, -1),
	cursorPos(0, 0),
	graph(nullptr)
{
}


Map::~Map()
{
	if (graph) delete graph;
}

void Map::ClearPathLayer()
{
	std::fill(pathLayer.begin(), pathLayer.end(), CLEAR);
}

void Map::Init(std::vector<char>& initMap, int width, int height)
{
	tiles = initMap;

	mapSize.x = width;
	mapSize.y = height;

	// Init pathLayer
	pathLayer.clear();
	pathLayer.resize(width * height);
	std::fill(pathLayer.begin(), pathLayer.end(), CLEAR);

	CreateGraph();
}

void Map::CreateGraph()
{
	if (graph) delete graph;

	graph = new Graph();


	for (int y = 0; y < mapSize.y; y++) {
		for (int x = 0; x < mapSize.x; x++) {
			if (tiles.at((y * mapSize.x) + x) != 1) {
				Node n = Node();
				n.position = Vector2(x, y);
				graph->AddNode(n);
			}
		}
	}

	for (auto& fromNode : graph->GetAllNodes()) {
		Node* toNode = (Node*)graph->GetNode(fromNode.position + Vector2(0, 1));
		if (toNode != nullptr) {
			Edge e1(toNode);
			Edge e2(&fromNode);
			fromNode.AddEdge(e1);
			toNode->AddEdge(e2);
		}
	}

	for (auto& fromNode : graph->GetAllNodes()) {
		Node* toNode = (Node*)graph->GetNode(fromNode.position + Vector2(1, 0));
		if (toNode != nullptr) {
			Edge e1(toNode);
			Edge e2(&fromNode);
			fromNode.AddEdge(e1);
			toNode->AddEdge(e2);
		}
	}
}

void Map::Render(SDL_Surface * screenSurface)
{
	// Draw tiles
	for (int y = 0; y < mapSize.y; y++)
		for (int x = 0; x < mapSize.x; x++)
		{
			auto tileColor = SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF); // White
			switch (tiles[y * mapSize.x + x])
			{
			case 0:
				break;
			case 1:
				tileColor = SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00); // Black
				break;
			}

			// Draw the path
			switch (pathLayer[y * mapSize.x + x])
			{
			case CLEAR:
				break;
			case SEARCH_IN_PROGRESS:
				tileColor = SDL_MapRGB(screenSurface->format, 0xf0, 0xf0, 0x00); // 
				break;
			case RESULT_PATH_FOUND:
				tileColor = SDL_MapRGB(screenSurface->format, 0x00, 0xF0, 0xF0); 
				break;
			}

			SDL_Rect rect;
			rect.x = x * 50;
			rect.y = y * 50;
			rect.h = rect.w = 50;
			SDL_FillRect(screenSurface, &rect, tileColor);
		}

	// Draw start/end markers
	if (startPos.x >= 0 && startPos.y >= 0)
	{
		DrawMarker(screenSurface, START, startPos);
	}

	if (endPos.x >= 0 && endPos.y >= 0)
	{
		DrawMarker(screenSurface, END, endPos);
	}

	// Draw cursor	
	DrawMarker(screenSurface, CURSOR, cursorPos);
}

void Map::SetPosition(MarkType mark, int x, int y)
{
	switch (mark)
	{
	case CURSOR:
		cursorPos.x = x;
		cursorPos.y = y;
		break;
	case START:
		startPos.x = x;
		startPos.y = y;

		endPos = Vector2(-1, -1);

		break;
	case END:
		endPos.x = x;
		endPos.y = y;
		break;
	}
}

void Map::SetPosition(MarkType mark, Vector2 pos)
{
	SetPosition(mark, pos.x, pos.y);
}

Vector2 Map::GetPosition(MarkType mark)
{
	Vector2 result = Vector2(-1, -1);
	switch (mark)
	{
	case CURSOR:
		result = cursorPos;
		break;
	case START:
		result = startPos;
		break;
	case END:
		result = endPos;
		break;
	}
	return result;
}

Vector2& Map::GetMapSize()
{
	return mapSize;
}

void Map::DrawMarker(SDL_Surface * screenSurface, MarkType mark, Vector2 pos)
{
	SDL_Rect rect;
	switch (mark)
	{
	case CURSOR:
		rect.x = pos.x * 50 + 12;
		rect.y = pos.y * 50 + 12;
		rect.h = rect.w = 25;
		SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));
		break;
	case START:
		rect.x = pos.x * 50 + 10;
		rect.y = pos.y * 50 + 10;
		rect.h = rect.w = 30;
		SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0xFF));
		break;
	case END:
		rect.x = pos.x * 50 + 10;
		rect.y = pos.y * 50 + 10;
		rect.h = rect.w = 30;
		SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0x00));
		break;
	}
}

Map::MarkingResult Map::TryToSetMarker()
{
	MarkingResult result = NONE;
	if (tiles[cursorPos.x + cursorPos.y * mapSize.x] == 0) // marking cannot be done on walls
	{	
		if ((endPos.x < 0 || endPos.y < 0) && (startPos.x >= 0 && startPos.y >= 0))
		{
			// Set end mark
			endPos = cursorPos;
			result = END_MARKED;
		}
		else
		{
			// Set start mark;
			startPos = cursorPos;
			endPos = Vector2(-1, -1);
			result = START_MARKED;
		}
	}
	return result;
}

Vector2 Map::GetMarkerPosition(MarkType mark)
{
	Vector2 ret(-1, -1);
	switch (mark)
	{
	case START:
		ret = startPos;
		break;
	case END:
		ret = endPos;
		break;	
	}

	return ret;
}

void Map::SetPathMap(Vector2 pos, int value)
{
	pathLayer[pos.x + mapSize.x * pos.y] = value;
}

Graph* Map::GetGraph()
{
	return graph;
}



