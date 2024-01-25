#include <iostream>
#include <algorithm>
#include "SDL2/SDL.h"
#include "Grid.h"
#include "BlockButton.h"

#define FPS 30 // Nombre de frame entre 2 étapes d'évolution (ATTENTION : Ce n'est pas le nombre de frame par secondes, sauf si SECONDE_ENTRE_EVOLUTION = 1)

#define SECONDE_ENTRE_EVOLUTION 0.05 // Nombre de seconde entre 2 étapes d'évolution (permet donc de modifier la vitesse d'évolution des cellules)
// !!! En théorie seulement, en réalité ce n'est pas exactement cette valeur !!!


#define WINDOW_WIDTH 1600 // Dimensions de la fenêtre
#define WINDOW_HEIGHT 900

#define AREA_WIDTH 1400 // Dimensions de la zone pour les cellules
#define AREA_HEIGHT 787

void pause(EnvData e){ // Je fait pas de vérification si c'est déjà en pause ou non
	*(e.isPaused) = true;
}

void play(EnvData e){ // Je fait pas de vérification si c'est déjà en lecture ou non
	*(e.isPaused) = false;
}

SDL_Window* createWindow(const char* window_name){
	if(SDL_Init(SDL_INIT_VIDEO /*| SDL_INIT_AUDIO*/) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return NULL;
    }

    SDL_Window *window = SDL_CreateWindow(window_name,
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          0);
                                          
	return window;
}


int main(){

	const short nLigne = 56;
	const short nColonne = 100;
	const short dimCellule = std::min(AREA_WIDTH / nColonne, AREA_HEIGHT / nLigne); // On définit au mieux la dimension des cellules
	
    SDL_Window *window = createWindow("Lenia Simulation");
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
    
    
    short nbLigne = 4; // Nombre de lignes de case
    short nbColonne = 2; // Nombre de colonnes de case
    BlockButton *buttons = new BlockButton(AREA_WIDTH, 0, nbLigne, nbColonne, (WINDOW_WIDTH - AREA_WIDTH) / nbColonne, AREA_HEIGHT / nbLigne , 3); // 4 lignes, 2 colonnes
	buttons->addFunction(pause); // Pour l'instant il faut ajouter les fonctions à la main
	// Je n'arrive pas encore à ajouter les fonctions provennant de l'objet g (fillSimpleGrid, fillRandomGrid et fillGridWithCircle). On verra ça plus tard
	buttons->addFunction(play);
	buttons->addFunction(pause);
	buttons->addFunction(play);
	buttons->addFunction(pause);
	buttons->addFunction(play);
	buttons->addFunction(pause);
	buttons->addFunction(play);


    
    std::vector<Cellule> *nextGrid; // Sert à conserver les nouveaux états souhaités pour les cellules
    std::vector<float> listePasDesCellules;
    int compteur = 1;
    
    int* xClick = new int; // Pour les coordonnées de la souris, il faut des pointeurs 
	int* yClick = new int;

	bool* isPaused = new bool;
	*isPaused = false;

	EnvData e;
	e.isPaused = isPaused;
	e.g = g->getGrille();

	g->initialiserGrille(e);

	while(keep_open)
	{
		while(SDL_PollEvent(&event) > 0)
		{
		    switch(event.type)
		    {
		        case SDL_QUIT:
		            keep_open = false;
		            
		            break;
		        case SDL_MOUSEBUTTONDOWN:
		        	SDL_GetMouseState(xClick,yClick);
		        	if (buttons->isClicked(*xClick,*yClick)){
		        		int indCase = buttons->getIndCase(*xClick,*yClick);	
		        		std::cout << "Indice = "<< indCase << "\n";
						buttons->execute(indCase,e); // Exécute la fonction dans le tableau de fonction de BlockButton, à l'indice indCase
		        	}
		    }
		}

		std::cout << "Marche 1\n";
		
		if (!(*isPaused)){
			// Effacer le renderer (met la couleur de fond en blanc)
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
			SDL_RenderClear(renderer);
			g->draw(renderer);
			buttons->draw(renderer); // Dessine les cases à droite de la zone des cellules
			
			SDL_Delay((1./FPS) * (SECONDE_ENTRE_EVOLUTION * 1000)); // Valeur en milliseconde
			if (compteur == 0){ 
				nextGrid = g->newStep();
				for (int i = 0 ; i < nextGrid->size() ; i++){
					listePasDesCellules.push_back(nextGrid->at(i).value / FPS);// On détermine pour chaque cellule le pas entre 2 états.
				}
			}else if(compteur == (FPS + 1)){
				compteur = -1;
				listePasDesCellules.clear();
			}else{
				g->update(listePasDesCellules); // Passe les états des cellules à l'état suivant (ajoute à la valeur de chaque cellule le pas)
			}
			compteur++;
			
			// Met à jour le renderer
			SDL_RenderPresent(renderer);
		}
	}
	
	delete xClick;
	delete yClick;
	delete isPaused;
}




