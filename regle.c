#include "regle.h"

#include <stdio.h>
#include <stdlib.h>

void applique_regle(regle* r) {
	for (int i = 0; i < r->n_commandes; i++) {
		printf("%s\n", r->commandes[i]);
		system(r->commandes[i]);
	}
}

regle* nouvelle_regle(char* nom, int n_prerequis, int n_commandes) {
	regle* r = malloc(sizeof(regle));
	r->prerequis = malloc(sizeof(char*) * n_prerequis);
	r->commandes = malloc(sizeof(char*) * n_commandes);
	
	r->nom = nom;
	r->n_commandes = n_commandes;
	r->n_prerequis = n_prerequis;

	r->prerequis_actuel = 0;
	r->commande_actuelle = 0;
	return r;
}



void detruire_regle(regle* r) {
	free(r->commandes);
	free(r->prerequis);

	free(r);
}

void ajouter_prerequis(regle* r, char* prerequis) {
	r->prerequis[r->prerequis_actuel] = prerequis;

	r->prerequis_actuel++;
}

void ajouter_commande(regle* r, char* commande) {
	r->commandes[r->commande_actuelle] = commande;

	r->commande_actuelle++;
}
