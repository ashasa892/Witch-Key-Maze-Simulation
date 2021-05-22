#include "App.h"



int main(int argc, char** argv) {

	App app;

	if (argc >= 3) {
		App::seed = std::stoi(argv[1]);
		App::method = std::stoi(argv[2]);
	}

	if (!app.getQuit()) app.init();

	while (!app.getQuit()) {
		app.handleEvents();
		app.update();
		app.render();
	}

	printf("Steps %d\n", App::steps);
	if (App::method == 2) {
		printf("Shortest path length %d\n", App::pathLen);
	}

}