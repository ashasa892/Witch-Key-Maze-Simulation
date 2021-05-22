#include "App.h"

const char *PLAYER = "../res/player.png", // status = 4;
*MOVE = "../res/move.png",  // status = 0
*MOVE1 = "../res/move1.png", // status = 1
*MOVE2 = "../res/move2.png",	// status = 2
*BLOCK = "../res/block.png"; // status = 3


component player;
component maze[XSIZE][YSIZE];

using namespace std;

int curx=1, cury=1, goalx=XSIZE-5, goaly=YSIZE-5;

stack<pair<int,int> > dfsStack;
queue<pair<int,int> > bfsQueue;
int pathx = goalx, pathy = goaly;

int App::steps =0;
int App::method = 2;
int App::seed = 6996;
int App::pathLen = 0;


void App::init() {
	if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
		throw "Error while initializing SDL (SRC - init() in App.cpp)";
	}
	printf("SDL systems initialized\n");
	
	window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	mapGenerate(seed);

	player.xpos = 1, player.ypos = 1, player.status = 4;
	loadTexture(&player);

}

void App::mapGenerate(int seed) {
	srand(seed);

	for (auto x=0; x < XSIZE; ++x) {
		for (auto y=0; y < YSIZE; ++y) {
			maze[x][y].xpos = x, maze[x][y].ypos = y;
			if((x == goalx || x == 1) && (y == 1 || y == goaly)){
				maze[x][y].status = 0;
			}
			else if(x == 0 || x == XSIZE-1 || y == 0 || y == YSIZE - 1) maze[x][y].status = 3;
			else maze[x][y].status = (rand()%3)?0:3;

			loadTexture(*(maze+x) + y);
		}
	}
}


void App::handleEvents() {
	// SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type) {
		case SDL_QUIT:
			setQuit(true);
		default:
		break;

	}
}

bool App::checkBounds(int x, int y) {
	if (x<0 || y<0 || x >= XSIZE || y >= YSIZE) return false;
	if (maze[x][y].status == 0) return true;
	else return false;
}

void App::dfsUpdate() {
	if (maze[curx][cury].status == 0) maze[curx][cury].status = 1, dfsStack.push({curx, cury});

	if (checkBounds(curx+1, cury)) ++curx;
	else if (checkBounds(curx, cury+1)) ++cury;
	else if (checkBounds(curx-1, cury)) --curx;
	else if (checkBounds(curx, cury-1)) --cury;
	else if (!dfsStack.empty()) {
		pair<int,int> p = dfsStack.top();
		dfsStack.pop();
		curx = p.first, cury = p.second;
		maze[curx][cury].status = 2;
	}
}

void App::bfsUpdate() {
	maze[curx][cury].status = 1;
	if (checkBounds(curx+1, cury)) {
		maze[curx+1][cury].status = 1;
		maze[curx+1][cury].parx = curx, maze[curx+1][cury].pary = cury;
		bfsQueue.push({curx+1, cury});
	}
	if (checkBounds(curx, cury+1)) {
		maze[curx][cury+1].status = 1;
		maze[curx][cury+1].parx = curx, maze[curx][cury+1].pary = cury;
		bfsQueue.push({curx, cury+1});
	}
	if (checkBounds(curx-1, cury)) {
		maze[curx-1][cury].status = 1;
		maze[curx-1][cury].parx = curx, maze[curx-1][cury].pary = cury;
		bfsQueue.push({curx-1, cury});
	}
	if (checkBounds(curx, cury-1)) {
		maze[curx][cury-1].status = 1;
		maze[curx][cury-1].parx = curx, maze[curx][cury-1].pary = cury;
		bfsQueue.push({curx, cury-1});
	}
	if (!bfsQueue.empty()) {
		pair<int,int> p = bfsQueue.front();
		bfsQueue.pop();
		curx = p.first, cury = p.second;
	}
}


void App::smartDfsUpdate() {
	if (maze[curx][cury].status == 0) maze[curx][cury].status = 1, dfsStack.push({curx, cury});
	int cntR = 0, cntD = 0, cntL = 0, cntU = 0, tempx = curx+1, tempy = cury;
	bool r = false, d = false, l = false, u = false;
	while (checkBounds(tempx, tempy)) r = r || (tempx == goalx && tempy == goaly), tempx++, cntR++;
	tempx = curx, tempy++;
	while (checkBounds(tempx, tempy)) d = d || (tempx == goalx && tempy == goaly), tempy++, cntD++;
	tempx = curx-1, tempy = cury;
	while (checkBounds(tempx, tempy)) l = l || (tempx == goalx && tempy == goaly), tempx--, cntL++;
	tempx = curx, tempy = cury-1;
	while (checkBounds(tempx, tempy)) u = u || (tempx == goalx && tempy == goaly), tempy--, cntU++;

	
	if ((cntR && (cntD<cntR || !cntD) && !(d || l || u)) || r) ++curx;
	else if ((cntD && !(l || u)) || d) ++cury;
	else if ((cntL && !(u)) || l) --curx;
	else if (cntU) --cury;
	else if (!dfsStack.empty()) {
		pair<int,int> p = dfsStack.top();
		dfsStack.pop();
		curx = p.first, cury = p.second;
		maze[curx][cury].status = 2;
	}

}


void App::update() {
	if (curx != goalx || cury != goaly) {
		int tempx = curx, tempy = cury;
		if (App::method == 1) dfsUpdate();
		else if (App::method == 2) bfsUpdate();
		else if (App::method == 3) smartDfsUpdate();
		loadTexture(*(maze+tempx) + tempy);
		++steps;
	}
	else if (method == 2 && (pathx != 1 || pathy != 1)) { //bfs
		int tempx = pathx, tempy = pathy;
		pathx = maze[tempx][tempy].parx, pathy = maze[tempx][tempy].pary;
		maze[pathx][pathy].status = 2;
		loadTexture(*(maze+pathx) + pathy);
		pathLen++;
	}
	else{
		
	}
	player.xpos = curx, player.ypos = cury;
}

void App::loadTexture(component *c) {
	SDL_Surface* surface;
	if (c->status == 0) surface = IMG_Load(MOVE);
	else if (c->status == 1) surface = IMG_Load(MOVE1);
	else if (c->status == 2) surface = IMG_Load(MOVE2);
	else if (c->status == 3) surface = IMG_Load(BLOCK);
	else if (c->status == 4) surface = IMG_Load(PLAYER);
	else return;

	c->tex = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

void App::render(component *c) {
	SDL_Rect dest{c->xpos * SCALE, c->ypos * SCALE, SCALE, SCALE};
	SDL_RenderCopy(renderer, c->tex, NULL, &dest);
}

void App::render() {
	
	for (int i=0; i<XSIZE; i++) {
		for (int j=0; j<YSIZE; j++) {
			render(*(maze+i) + j);
		}
	}
	render(&player);
	SDL_RenderPresent(renderer);
	SDL_Delay(60);
}


App::App() {

}

App::~App(void) {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "SDL systems shut down" << std::endl;
}

