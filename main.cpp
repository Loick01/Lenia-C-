#include <iostream>
#include <algorithm>
#include "SDL2/SDL.h"
#include "Grid.h"

#define FPS 30

SDL_Window* createWindow(const char* window_name, const int nColonne, const int nLigne, const int dimCellule){
	if(SDL_Init(SDL_INIT_VIDEO /*| SDL_INIT_AUDIO*/) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return NULL;
    }

	// Adapte les dimensions de la fenêtre par rapport au nombre de ligne et de colonne (selon la dimension des cellules)
    SDL_Window *window = SDL_CreateWindow(window_name,
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          nColonne * dimCellule, nLigne * dimCellule,
                                          0);
                                          
	return window;
}


int main(){

	const short nLigne = 56;
	const short nColonne = 100;
	const short dimCellule = 16;
	
    SDL_Window *window = createWindow("Lenia Simulation", nColonne, nLigne, dimCellule); // 100 Colonne, 56 ligne, cellule de taille 16, donc environ 1600x900
    Grid *g = new Grid(nColonne, nLigne, dimCellule);

    if(!window)
    {
        std::cout << "Failed to create window\n";
        return -1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_Log("Erreur lors de la création du rendu : %s", SDL_GetError());
        return -1;
    }
    
    bool keep_open = true;
    SDL_Event event;
	while(keep_open)
	{
		while(SDL_PollEvent(&event) > 0)
		{
		    switch(event.type)
		    {
		        case SDL_QUIT:
		            keep_open = false;
		            break;
		    }
		}
		
		g->draw(renderer);
		SDL_Delay(1./FPS);
	}
}
