//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include "Map.h"
#include "Astar.h"

//Screen dimension constants
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

int main(int argc, char* args[])
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("Game302 Assignment2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			int x = 0;

			std::vector<char> initMap = { 
				1, 0, 1, 1, 1, 1, 0, 1, 0, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
				1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
				1, 1, 1, 0, 0, 0, 0, 1, 0, 0,
				1, 0, 0, 0, 1, 1, 1, 1, 0, 1,
				1, 1, 1, 0, 0, 0, 0, 1, 0, 0,
				0, 0, 1, 1, 1, 0, 1, 0, 0, 1,
				1, 0, 1, 0, 0, 0, 1, 0, 1, 1,
				1, 0, 0, 0, 1, 0, 1, 0, 1, 0,
				1, 1, 1, 1, 1, 0, 0, 0, 0, 0
			};

			// Create a map and initialize 
			Map map;
			map.Init(initMap, 10, 10);
			Vector2 mapSize = map.GetMapSize();
			
			AStar pathFinder(&map);

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
					{
						Vector2 cursorPos = map.GetPosition(Map::CURSOR);
						//Select surfaces based on key press
						switch (e.key.keysym.sym)
						{
						case SDLK_DOWN:
							cursorPos.y++;
							if (cursorPos.y >= map.GetMapSize().y) 
								cursorPos.y = map.GetMapSize().y - 1;
							break;

						case SDLK_UP:
							cursorPos.y--;							
							if (cursorPos.y < 0)
								cursorPos.y = 0;
							break;

						case SDLK_LEFT:
							cursorPos.x--;							
							if (cursorPos.x < 0)
								cursorPos.x = 0;
							break;

						case SDLK_RIGHT:
							cursorPos.x++;
							if (cursorPos.x >= map.GetMapSize().x)
								cursorPos.x = map.GetMapSize().x - 1;
							break;

						case SDLK_SPACE:
							// you can set start or end point only when the pathfinder is not searching
							if (!pathFinder.IsSearching())
							{
								switch (map.TryToSetMarker())
								{
								case Map::START_MARKED: // A start point has been marked
									map.ClearPathLayer(); // clear the previous path result
									break;

								case Map::END_MARKED: // An end point has been marked
									{
										// Get the start node from the start mark point
										Vector2 start = map.GetMarkerPosition(Map::START);
										Node* startNode = map.GetGraph()->GetNode(start);

										// Get the goal(end) node from the end mark point
										Vector2 goal = map.GetMarkerPosition(Map::END);
										Node* goalNode = map.GetGraph()->GetNode(goal);

										// Search the path
										pathFinder.Search(startNode, goalNode);
									}									
									break;
								}
							}
							break;

						default:							
							break;
						}
						map.SetPosition(Map::CURSOR, cursorPos);
					}
				}

				//Get window surface
				screenSurface = SDL_GetWindowSurface(window);
				
				// Draw the map
				map.Render(screenSurface);

				//Update the surface
				SDL_UpdateWindowSurface(window);
								
				SDL_Delay(1);
			}
		}
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}