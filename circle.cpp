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

coords createslice(coords coordsobj, float cx, float cy, float radius, int maxslices) {

	// Clear any existing coordinates
	coordsobj.x.clear();
	coordsobj.y.clear();

	// Calculate the angle between each slice
	float sliceangle = 2 * M_PI / maxslices;
	
	std::cout << "Creating " << maxslices << " slices with angle " << sliceangle << " radians (" << sliceangle * 180 / M_PI << " degrees)" << std::endl;

	// Create points around the circle
	for (int i = 0; i < maxslices; i++) {
		float angle = i * sliceangle;
		
		// Calculate the coordinates for this point
		float x = cx + radius * cos(angle);
		float y = cy + radius * sin(angle);
		
		// Add the point to our coordinates
		coordsobj.x.push_back(x);
		coordsobj.y.push_back(y);
		
		std::cout << "Point " << i << ": (" << x << ", " << y << ") at angle " << angle * 180 / M_PI << "°" << std::endl;
	}

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
	int maxslices = 10;
	float radius = 100;  // Increased radius for better visibility

	// Commandline argument handling
	if (argc == 2) {
		maxslices = stoi(argv[1]);
	} else if (argc == 3) {
		maxslices = stoi(argv[1]);
		radius = stoi(argv[2]);
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

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	
	coordsobj = createslice(coordsobj, cx, cy, radius, maxslices);

	/* Check the logic for this for loop later, write it down and examine it*/
	// Draw the polygon
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Blue color for lines
	
	// Draw lines connecting consecutive points to form the octagon
	for (size_t i = 0; i < coordsobj.x.size(); i++) {
		size_t next = (i + 1) % coordsobj.x.size();  // Connect last point to first point
		SDL_RenderDrawLine(renderer, 
			(int)coordsobj.x[i], (int)coordsobj.y[i],
			(int)coordsobj.x[next], (int)coordsobj.y[next]);
	}
	
	// Draw points
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color for points
	for (size_t i = 0; i < coordsobj.x.size(); i++) {
		SDL_RenderDrawPoint(renderer, (int)coordsobj.x[i], (int)coordsobj.y[i]);
	}
	
	SDL_RenderPresent(renderer);
	
	SDL_Delay(5000);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
