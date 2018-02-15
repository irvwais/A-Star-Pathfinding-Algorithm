#pragma once
#include <vector>
#include "Vector2.h"
#include "Graph.h"

class SDL_Surface;
	class Map
	{
	public:
		enum MarkType { CURSOR, START, END };
		enum MarkingResult { NONE, START_MARKED, END_MARKED };
		enum MarkOnPathLayer { CLEAR, SEARCH_IN_PROGRESS, RESULT_PATH_FOUND };
	private:

		Graph* graph;
		
		Vector2 mapSize;
		std::vector<char> tiles;

		Vector2 cursorPos;
		Vector2 startPos;	// star position for pathfinding
		Vector2 endPos;		// end(goal) position for pathfinding

							// to display searching process
							//char* pathLayer;
		std::vector<char> pathLayer;

	public:
		Map();
		~Map();

		void Init(std::vector<char>& initMap, int width, int height);
		void Render(SDL_Surface* screenSurface);
		void SetPosition(MarkType mark, int x, int y);
		void SetPosition(MarkType mark, Vector2 pos);
		Vector2 GetPosition(MarkType mark);
		Vector2& GetMapSize();
		void DrawMarker(SDL_Surface * screenSurface, MarkType mark, Vector2 pos);
		MarkingResult TryToSetMarker();
		Vector2 GetMarkerPosition(MarkType mark);
		void ClearPathLayer();
		void SetPathMap(Vector2 pos, int value);
		
		// Graph
		Graph* GetGraph();
	private:
		void CreateGraph();
	};


