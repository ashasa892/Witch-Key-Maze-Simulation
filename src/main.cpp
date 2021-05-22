#include "App.h"
#include <fstream>
#include <iostream>


using namespace std;

int main(int argc, char** argv) {

	fstream  output;
	output.open("output.txt", ios::out);
	if (argc >= 3) {
		App app;
		App::seed = std::stoi(argv[1]);
		App::method = std::stoi(argv[2]);
		if (!app.getQuit()) app.init();
		while (!app.getQuit()) {
			app.handleEvents();
			app.update();
			app.render();
		}
		printf("Steps Taken = %d\n", App::steps);
		if (App::method == 2) {
			printf("Shortest path length %d\n", App::pathLen);
		}
	}
	else{
		App app[10][3];
		output << "Seed, Algo 1, Algo 2, Algo 3" << std::endl;
		for(int i = 0; i < 10; i++){
			App::seed = i*100;
			output << App::seed;
			App::method = 1;
			App::steps = 0;
			app[i][0].init();
			while (!app[i][0].getQuit()) {
				app[i][0].handleEvents();
				app[i][0].update();
				app[i][0].render();
			}
			output << ", " << App::steps;
			App::method = 2;
			App::steps = 0;
			app[i][0].~App();
			app[i][1].init();
			while (!app[i][1].getQuit()) {
				app[i][1].handleEvents();
				app[i][1].update();
				app[i][1].render();
			}
			output << ", " << App::steps;
			App::method = 3;
			App::steps = 0;
			app[i][1].~App();
			app[i][2].init();
			while (!app[i][2].getQuit()) {
				app[i][2].handleEvents();
				app[i][2].update();
				app[i][2].render();
			}
			output << ", " << App::steps << std::endl;
			app[i][2].~App();
		}
	}

}