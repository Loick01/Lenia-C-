#ifndef GRID_H
#define GRID_H

#include <vector>
#include <cmath>

struct Cellule{
	float value; // Entre 0.0 et 1.0
	int indice; // Donne la position dans la grille
};

float gaussienneChooseValue(float somme){ // Détermine la nouvelle valeur d'une cellule (selon une gaussienne, on peut faire varier les paramètres nu et sigma)
	// Quand somme varie de 0 à 8, res varie doit varier de 0 à 1
	float mu = 0.25;
	float sigma = 0.025;
	float val = somme / 8; // Diviser par 8 pour avoir une valeur entre 0 et 1 (somme des valeurs des 8 cellules adjacentes entre 0 et 8)
	float res = exp(-((pow(val-mu,2))/(2 * pow(sigma,2))));
	return res;
}

// On pourrait utiliser plein d'autres courbes pour déterminer les nouvelles valeurs des cellules

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
	
	void fillRandomGrid(std::vector<Cellule>* g){
		for (unsigned int i = 0 ; i < g->size() ; i++){
			Cellule c;
			c.indice = i;
			c.value = (rand() % 100) / 100.;  ; // Valeur aléatoire pour les cellules
			g->at(i) = c;
		}
	}
	
	void fillGridWithCircle(std::vector<Cellule>* g, int horizontalCenter, int verticalCenter, int rayon){ // Remplit la grille avec un cercle (centre + rayon). Le rayon est un nombre de cellule
		// Coordonnées du centre du cercle (ramenée au centre d'une des cellules de la grille)
		int xCenter = (horizontalCenter * this->dimCellule) + (this->dimCellule / 2); 
		int yCenter = (verticalCenter * this->dimCellule) + (this->dimCellule / 2);
		
		for (unsigned int i = 0 ; i < g->size() ; i++){
			int xCell = (i / this->nColonne) * this->dimCellule + (this->dimCellule / 2); // Position x de la cellule
			int yCell = (i % this->nColonne) * this->dimCellule + (this->dimCellule / 2); // Position y de la cellule
			float dist = sqrt(pow(xCell-xCenter,2) + pow(yCell-yCenter,2));
			
			Cellule c;
			c.indice = i;
			if (dist < rayon * this->dimCellule){
				int numLigne = i / this->nColonne; // Numéro de la ligne sur laquelle se trouve la cellule
				int numColonne = i % this->nColonne;// Numéro de la colonne sur laquelle se trouve la cellule
				c.value = (1./(this->nLigne - 1) * numLigne) / 2 + (1./(this->nColonne - 1) * numColonne) / 2;
				g->at(i) = c;
			}else{
				c.value = 0.0;
				g->at(i) = c;
			}
		}
	}

	std::vector<Cellule>* initialiserGrille(){
		std::vector<Cellule> *g = new std::vector<Cellule>(this->nLigne * this->nColonne);
		
		// Remplir la grille de cellule selon une certaine fonction
		//fillSimpleGrid(g);
		fillRandomGrid(g);
		//fillGridWithCircle(g,25,50,20);
		
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
    
     void /*std::vector<Cellule>* */ newStep(){
    	std::vector<Cellule> *nextGrid = new std::vector<Cellule>(this->nLigne * this->nColonne);
    	
    	// Utiliser la grille courante pour calculer la grille suivante (plus tard il faudra fonctionner par interpolation linéaire)
    	for (int i = 0 ; i < this->grille->size() ; i++){
    		int numLigne = i / this->nColonne; // Numéro de la ligne sur laquelle se trouve la cellule
			int numColonne = i % this->nColonne;// Numéro de la colonne sur laquelle se trouve la cellule
			float somme = 0.0; // Somme des valeurs des 8 cellules adjacentes
    		for (int l = -1 ; l < 2 ; l++){ // Pour l'instant on regarde les 8 cellules adjacentes (faire plus tard avec de filtres)
    			for (int c = -1 ; c < 2 ; c++){
    				if (l != 1 && c != 1){ // Ne pas considérer la cellule courante
    					if (numLigne + l >= 0 && numLigne + l < this->nLigne && numColonne + c >= 0 && numColonne + c < this->nColonne){ // On vérifie si on ne sort pas de la grille
    						somme += this->grille->at((numLigne + l) * this->nColonne + numColonne + c).value;
    					}
    				}
    			}
    		}
    		Cellule c;
    		c.indice = i;
    		c.value = gaussienneChooseValue(somme);
    		nextGrid->at(i) = c;
    	}
    	
    	this->grille = nextGrid;
    	//return nextGrid;
    }
    
};

#endif // GRID_H
