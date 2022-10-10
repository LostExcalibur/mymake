#include "ens_regles.h"
#include "regle.h"

#include <stdlib.h>
#include <string.h>

ens_regles* nouvel_ensemble(int nb_regles) {
	ens_regles* ens = malloc(sizeof(ens_regles));

	ens->regles = malloc(nb_regles * sizeof(regle *));

	ens->regle_actuelle = 0;

	return ens;
}

void detruire_ensemble(ens_regles* ens) {
	free(ens->regles);

	free(ens);
}

void ajouter_regle(ens_regles* ens, regle* r) {
	ens->regles[ens->regle_actuelle] = r;
	ens->regle_actuelle++;
}


// Renvoit un pointeur vers la règle ayant le nom correspondant.
// Renvoit NULL si elle n'existe pas dans l'ensemble
regle* trouver_regle(ens_regles* ens, char* nom) {
	for (int i = 0; i < ens->regle_actuelle; i++) {
		if (strcmp(ens->regles[i]->nom, nom) == 0) {
			return ens->regles[i];
		}		
	}
	return NULL;
}	
