#ifndef T_H
#define T_H

struct Cellule{
	float value; // Entre 0.0 et 1.0
	int indice; // Donne la position dans la grille
};

struct EnvData{ // Bon là cette structure est juste adapté pour l'utilisation que je veux en faire pour cette appli, c'est pour ça que c'est peut être pas une très bonne solution
	bool* isPaused; // Pour le bouton de pause et de lecture
	std::vector<Cellule>* g; // Pour remplir la grille
	int horizontalCenter; // Pour remplir la grille avec un cercle
	int verticalCenter;
	int rayon;
	/* Si on fait le placement libre du cercle, on aura besoin de ça
	int clickX;
	int clickY;
	*/
};

#endif // T_H