#ifndef GRID_H
#define GRID_H

#include <vector>

struct Cellule{
	float value; // Entre 0.0 et 1.0
	int indice; // Donne la position dans la grille
};

class Grid {
private:
	int nLigne;
	int nColonne;
	int dimCellule;
	std::vector<Cellule> *grille;
	
public:    
	Grid(int nColonne, int nLigne, int dimCellule) {
    	this->nLigne = nLigne;
		this->nColonne = nColonne;
		this->dimCellule = dimCellule;
		this->grille = initialiserGrille();
    }
    
    void fillSimpleGrid(std::vector<Cellule>* g){ // Remplit la grille linéairement
		for (unsigned int i = 0 ; i < g->size() ; i++){
			Cellule c;
			c.indice = i;
			int numLigne = i / this->nColonne; // Numéro de la ligne sur laquelle se trouve la cellule
			int numColonne = i % this->nColonne;// Numéro de la colonne sur laquelle se trouve la cellule
			c.value = (1./(this->nLigne - 1) * numLigne) / 2 + (1./(this->nColonne - 1) * numColonne) / 2;
			g->at(i) = c;
		}
	}

	std::vector<Cellule>* initialiserGrille(){
		std::vector<Cellule> *g = new std::vector<Cellule>(this->nLigne * this->nColonne);
		
		// Remplir la grille de cellule selon une certaine fonction
		fillSimpleGrid(g);
		
		return g;
	}

    void draw(SDL_Renderer* renderer){
		// Effacer le renderer
		SDL_RenderClear(renderer);
    

		// Dessiner tous les rectangles sur le renderer
		for (int i = 0 ; i < this->grille->size() ; i++){
			float value = this->grille->at(i).value;
			SDL_SetRenderDrawColor(renderer, value * 174, value * 255, 0, 255); // Pour l'instant une seule couleur (jaune)
			SDL_Rect rectangle = { (i % this->nColonne) * this->dimCellule, (i / this->nColonne) * dimCellule, this->dimCellule, this->dimCellule };
			SDL_RenderFillRect(renderer, &rectangle);	
		}

		// Met à jour le renderer
		SDL_RenderPresent(renderer);
    }
    
};

#endif // GRID_H
