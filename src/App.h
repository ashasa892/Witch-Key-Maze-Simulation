#pragma once
#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <utility>
#include <stack>
#include <queue>

		

#define GAME_NAME "Maze Simulator" 
#define SCREEN_WIDTH 1280 // height of the game window  (change in Map.h)
#define SCREEN_HEIGHT 640  // width of the game window

#define XSIZE SCREEN_WIDTH/32
#define YSIZE SCREEN_HEIGHT/32
#define SCALE 32


struct component {
	int xpos, ypos, status, parx=-1, pary;
	SDL_Texture* tex;
	component() {}
	component(int x, int y, int s): xpos(x), ypos(y), status(s) {}
};

class App 
{
public:
	App();
	~App();

	void init(); //init sdl and window
	void handleEvents();
	void update();
	void loadTexture(component* c);
	void render(component* c);
	void render();
	void mapGenerate(int seed);
	void dfsUpdate();
	void bfsUpdate();
	void smartDfsUpdate();
	bool checkBounds(int x, int y);
	

	static int seed;
	static int pathLen;
	static int method;
	static int steps;
    inline bool getQuit() const { return quit; }
    void setQuit( bool q ) { quit = q; }

private:
	
	SDL_Event event;
	bool quit;
	SDL_Window* window;
	SDL_Renderer* renderer;


};