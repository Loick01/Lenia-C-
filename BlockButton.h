#ifndef BLOCKBUTTON_H
#define BLOCKBUTTON_H

#include "T.h"

class BlockButton {
private:
	// Coordonnées du point en haut à gauche de la zone de case
	short xPoint;
	short yPoint;
	
	short nbLigne; // Pour l'instant on considère que toute les lignes ont 'nbColonne' colonnes
	short nbColonne;
	short widthCase; // Toutes les cases ont les mêmes dimensions (je ne pense pas changer ça)
	short heightCase;
	short separation; // Ecart (en pixel) entre 2 cases
	
	std::vector<void (*)(EnvData)> functionsForButton; // Toutes ces fonctions dans ce tableau prendront un unique paramètre de type EnvData
	/*
	Tableau de pointeur de fonction, représentant une fonctionnalité pour chaque bouton. Attention --> toutes les fonctions doivent avoir les mêmes types de retour et les mêmes types de paramètres
	Du coup soit on crée un nouveau tableau pour chaque configuration, sauf que je souhaite avoir un unique tableau par BlockButton.
	Donc on fera en sorte que toutes les fonctions soit de type void
	Et pour les paramètres on utilisera la structure EnvData pour ne pas se prendre la tête avec problème (à voir quand même si on ne pourrait pas faire mieux)
	*/
	
public:
	BlockButton(short xPoint, short yPoint,short nbLigne, short nbColonne, short widthCase, short heightCase, short separation){
		this->xPoint = xPoint;
		this->yPoint = yPoint;
    	this->nbLigne = nbLigne;
		this->nbColonne = nbColonne;
		this->widthCase = widthCase; // Longueur d'une case, à laquelle on enlèvera la séparation (une fois seulement)
		this->heightCase = heightCase;
		this->separation = separation;
    }

	void addFunction(void (*f)(EnvData)){ // Il faudra ajouter à la main les fonctions pour les boutons, il n'y a aucune vérification s'il y a autant de fonctions que de boutons
		this->functionsForButton.push_back(f);
	}
    
    void draw(SDL_Renderer* renderer){
    	// Attention à ne pas faire SDL_RenderClear ici, c'est déjà fait dans draw de Grid.h (donc attention à l'ordre d'appel de ces fonctions)
    	SDL_SetRenderDrawColor(renderer, 19, 87, 37, 255); // Pour l'instant toutes les cases ont la même couleur
    	
    	for (int i = 0 ; i < nbLigne ; i++){
    		for (int j = 0 ; j < nbColonne ; j++){
    			SDL_Rect rectangle = { this->xPoint + j * (this->separation + this->widthCase),
    								   this->yPoint + i * (this->separation + this->heightCase), 
    								   this->widthCase - this->separation, 
    								   this->heightCase - this->separation};
				SDL_RenderFillRect(renderer, &rectangle);
    		}
    	}
    }
    
    bool isClicked(int clickX, int clickY){ // Coordonnées de la souris en paramètre
    	return (clickX >= this->xPoint && clickX <= this->xPoint + this->nbColonne * this->widthCase &&
    			clickY >= this->yPoint && clickY <= this->yPoint + this->nbLigne * this->heightCase);
    }
    
    int getIndCase(int clickX, int clickY){ // Retourne l'indice (Commence à 0, de gauche à droite, de haut en bas) de la case qui est cliquée
    	int nL = (clickY - this->yPoint) / this->heightCase;
    	int nC = (clickX - this->xPoint) / this->widthCase;
    	return this->nbColonne * nL + nC;
    }

	void execute(int ind, EnvData e){
		this->functionsForButton[ind](e);
	}
};

#endif // BLOCKBUTTON_H
