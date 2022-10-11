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
	for (int i = 0; i < ens->regle_actuelle; i++) {
		detruire_regle(ens->regles[i]);
	}
	free(ens->regles);
	free(ens);
}

void ajouter_regle(ens_regles* ens, regle* r) {
	ens->regles[ens->regle_actuelle] = r;
	ens->regle_actuelle++;
}


regle* trouver_regle(ens_regles* ens, char* nom) {
	for (int i = 0; i < ens->regle_actuelle; i++) {
		if (strcmp(ens->regles[i]->nom, nom) == 0) {
			return ens->regles[i];
		}		
	}
	return NULL;
}


void afficher_ensemble(ens_regles* ens) {
	for (int i = 0; i < ens->regle_actuelle; i++) {
		afficher_regle(ens->regles[i]);
	}	
}
