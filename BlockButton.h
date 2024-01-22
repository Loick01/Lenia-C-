#ifndef BLOCKBUTTON_H
#define BLOCKBUTTON_H

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
};

#endif // BLOCKBUTTON_H
