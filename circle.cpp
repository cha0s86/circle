#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <string>
#include <SDL2/SDL.h>
using namespace std;

struct coords {
	public:
		std::vector<float> x;
		std::vector<float> y;
};

coords createslice(coords coordsobj, SDL_Renderer* renderer, float cx, float cy, float radius, int maxslices) {

	// Calculate the angle between each slice
	float sliceangle = 2 * M_PI / maxslices;
	
	std::cout << "Creating " << maxslices << " slices with angle " << sliceangle << " radians (" << sliceangle * 180 / M_PI << " degrees)" << std::endl;
	
	// Clear screen for this generation
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	// Clear previous coordinates and create all points for this generation
	coordsobj.x.clear();
	coordsobj.y.clear();
	
	for (int sliceIndex = 0; sliceIndex < maxslices; sliceIndex++) {
		float angle = sliceIndex * sliceangle;
		
		// Calculate the coordinates for this point
		float x = cx + radius * cos(angle);	
		float y = cy + radius * sin(angle);
		
		// Add the point to our coordinates
		coordsobj.x.push_back(x);
		coordsobj.y.push_back(y);		
		
		std::cout << "Point " << sliceIndex << ": (" << x << ", " << y << ") at angle " << angle * 180 / M_PI << "°" << std::endl;
	}

	// Draw the complete polygon for this generation
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green for lines
	for (size_t i = 0; i < coordsobj.x.size(); i++) {
		size_t next = (i + 1) % coordsobj.x.size();
		SDL_RenderDrawLine(
			renderer,
			(int)coordsobj.x[i], (int)coordsobj.y[i],
			(int)coordsobj.x[next], (int)coordsobj.y[next]
		);
	}

	// Draw all points on top
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red for points
	for (size_t i = 0; i < coordsobj.x.size(); i++) {
		SDL_RenderDrawPoint(renderer, (int)coordsobj.x[i], (int)coordsobj.y[i]);
	}

	SDL_RenderPresent(renderer);
	
	SDL_Delay(50);

	return coordsobj;
}

int main(int argc, char* argv[]) {

	// Initialize variables
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 360;
	
	// Circle center coordinates
	float cx = SCREEN_WIDTH/2;  // Center x coordinate (half of window width)
	float cy = SCREEN_HEIGHT/2;  // Center y coordinate (half of window height)

	// Coordinaeobject
	coords coordsobj;
	
	// Default values
	float radius 	= 100;  // Increased radius for better visibility
	int generations = 10;

	bool flagsUsed = false;
	// Commandline argument handling (flags)
	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "--help" || arg == "-h") {
			std::cout << "Usage: circle [--generations N] [--radius R]" << std::endl;
			std::cout << "\t--generations, -g: animate from 1..N slices so you can see all generations (default 10)" << std::endl;
			std::cout << "\t--radius, -r: circle radius (default 100)" << std::endl;
			return 0;
		} else if ((arg == "--generations" || arg == "-g") && i + 1 < argc) {
			generations = stoi(argv[++i]);
			flagsUsed = true;
		} else if ((arg == "--radius" || arg == "-r") && i + 1 < argc) {
			radius = stof(argv[++i]);
			flagsUsed = true;
		}
	}

	// Legacy positional args support (only if no flags provided)
	if (!flagsUsed) {
	if (argc == 2) {
			generations = stoi(argv[1]);
	} else if (argc == 3) {
			generations = stoi(argv[1]);
		radius = stoi(argv[2]);
		}
	}

	std::cout << "Welcome to circle approximator! a fun program!" << std::endl;
	std::cout << "Initializing SDL2..." << std::endl;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Failed to initialize SDL: " << SDL_GetError() << ". Exiting!" << std::endl;
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("My circle approximator!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (!window) {
		std::cout << "Failed to create window: " << SDL_GetError() << ". Exiting!" << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		std::cout << "Failed to create renderer: " << SDL_GetError() << ". Exiting!" << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	// Show all generations: animate from 1 slice up to `generations` slices
	for (int g = 1; g <= generations; g++) {
		std::cout << "\n=== Generation " << g << " / " << generations << " ===" << std::endl;
		coordsobj = createslice(coordsobj, renderer, cx, cy, radius, g);
	}

	SDL_RenderPresent(renderer);
	
	SDL_Delay(5000);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
