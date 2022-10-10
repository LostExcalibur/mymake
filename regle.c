#include "regle.h"

#include <stdio.h>
#include <stdlib.h>

void applique_regle(regle* r) {
	for (int i = 0; i < r->n_commandes; i++) {
		printf("%s\n", r->commandes[i]);
		system(r->commandes[i]);
	}
}

regle* nouvelle_regle(char* nom) {
	regle* r = malloc(sizeof(regle));

	r->nom = nom;

	return r;
}

void detruire_regle(regle* r) {
	free(r->commandes);
	free(r->prerequis);

	free(r);
}


