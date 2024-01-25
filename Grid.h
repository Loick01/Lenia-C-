#ifndef GRID_H
#define GRID_H

#include <vector>
#include <cmath>
#include "T.h"

#define nbCelluleFiltre 8

float gaussienneChooseValue(float somme){ // Détermine la nouvelle valeur d'une cellule (selon une gaussienne, on peut faire varier les paramètres nu et sigma)
	// Quand somme varie de 0 à nbCelluleFiltre, res varie doit varier de 0 à 1
	float mu = 0.15;
	float sigma = 0.015;
	float val = somme / nbCelluleFiltre; // Diviser par nbCelluleFiltre pour avoir une valeur entre 0 et 1 (somme des valeurs des nbCelluleFiltre cellules voisine entre 0 et nbCelluleFiltre)
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
    }

	std::vector<Cellule>* getGrille(){
		return this->grille;
	}
    
    void fillSimpleGrid(EnvData e){ // Remplit la grille linéairement
		std::vector<Cellule>* g = e.g; // Récupère les données nécessaire dans l'objet EnvData

		for (unsigned int i = 0 ; i < g->size() ; i++){
			Cellule c;
			c.indice = i;
			int numLigne = i / this->nColonne; // Numéro de la ligne sur laquelle se trouve la cellule
			int numColonne = i % this->nColonne;// Numéro de la colonne sur laquelle se trouve la cellule
			c.value = (1./(this->nLigne - 1) * numLigne) / 2 + (1./(this->nColonne - 1) * numColonne) / 2;
			g->at(i) = c;
		}
		this->grille = g;
	}
	
	void fillRandomGrid(EnvData e){
		std::vector<Cellule>* g = e.g; // Récupère les données nécessaire dans l'objet EnvData

		for (unsigned int i = 0 ; i < g->size() ; i++){
			Cellule c;
			c.indice = i;
			c.value = (rand() % 100) / 100.;  ; // Valeur aléatoire pour les cellules
			g->at(i) = c;
		}
		this->grille = g;
	}
	
	void fillGridWithCircle(EnvData e){ // Remplit la grille avec un cercle (centre + rayon). Le rayon est un nombre de cellule

		// Pour l'instant tout les cercles se placent au même endroit et ont tous le même rayon (par la suite on pourra choisir directement su la fenêtre)
		e.horizontalCenter = 25;
		e.verticalCenter = 50;
		e.rayon = 20;
		
		std::vector<Cellule>* g = e.g; // Récupère les données nécessaire dans l'objet EnvData
		int horizontalCenter = e.horizontalCenter;
		int verticalCenter = e.verticalCenter;
		int rayon = e.rayon;
		
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
		this->grille = g;
	}

	void initialiserGrille(EnvData e){
		e.g = new std::vector<Cellule>(this->nLigne * this->nColonne);
		// Remplir la grille de cellule selon une certaine fonction
		//fillSimpleGrid(e);
		//fillRandomGrid(e);
		fillGridWithCircle(e);
	}

    void draw(SDL_Renderer* renderer){
		// Dessiner tous les rectangles sur le renderer
		for (int i = 0 ; i < this->grille->size() ; i++){
			float value = this->grille->at(i).value;
			SDL_SetRenderDrawColor(renderer, value * 10, value * 155, value * 255, 255); 
			SDL_Rect rectangle = { (i % this->nColonne) * this->dimCellule, (i / this->nColonne) * dimCellule, this->dimCellule, this->dimCellule };
			SDL_RenderFillRect(renderer, &rectangle);	
		}
    }
    
     std::vector<Cellule>* newStep(){
    	std::vector<Cellule> *nextGrid = new std::vector<Cellule>(this->nLigne * this->nColonne);
    	
    	// Utiliser la grille courante pour calculer la grille suivante (plus tard il faudra fonctionner par interpolation linéaire)
    	for (int i = 0 ; i < this->grille->size() ; i++){
    		int numLigne = i / this->nColonne; // Numéro de la ligne sur laquelle se trouve la cellule
			int numColonne = i % this->nColonne;// Numéro de la colonne sur laquelle se trouve la cellule
			float somme = 0.0; // Somme des valeurs des nbCelluleFiltre cellules voisines
    		for (int l = -1 ; l <= 1 ; l++){
    			for (int c = -1 ; c <= 1 ; c++){
    				if (l != 0 || c != 0){ // Ne pas considérer la cellule courante
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
    	
    	return nextGrid;
    }
    
    void update(std::vector<float> listePas){
    	for (int i = 0 ; i < listePas.size() ; i++){
    		float newValue = this->grille->at(i).value + listePas[i];
    		if (newValue >= 0. && newValue <= 1.){ // Attention à bien borner les valeurs des cellules entre 0 et 1 
    			this->grille->at(i).value = newValue;
    		}
    	}
    }
    
};

#endif // GRID_H
