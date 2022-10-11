#include "ens_regles.h"
#include "regle.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

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

void auxilliaire(ens_regles* ens, char* nom, char* nom_parent){
	regle * r = trouver_regle(ens, nom);

	if (r == NULL){
		DIR *d;
		struct dirent *dir;
		d = opendir(".");
		if (!d) {
			fprintf(stderr, "Ne peut pas ouvrir le dossier");
			exit(1);
		}
		while ((dir = readdir(d)) != NULL) {
			if (strcmp(dir->d_name,nom) == 0) {
				return;
			}
		}
		fprintf(stderr,"Aucune règle pour fabriquer la cible %s, nécessaire pour %s",nom,nom_parent);
		return;
	}
	for (int i = 0; i < r->n_prerequis; i++){
		auxilliaire(ens, r->prerequis[i],nom);
	}
	for (int i =0; i < r->n_commandes; i++){
		printf("%s", r->commandes[i] + 1);
		system(r->commandes[i]);
	}
}

void appliquer_ens_regle(ens_regles* ens, char* nom) {
	regle * r = trouver_regle(ens, nom);
	if (r == NULL){
		fprintf(stderr,"Aucune règle pour fabriquer la cible «%s». Arrrêt.\n",nom);
		return ;
	}

	auxilliaire(ens, nom,NULL);

}



void afficher_ensemble(ens_regles* ens) {
	for (int i = 0; i < ens->regle_actuelle; i++) {
		afficher_regle(ens->regles[i]);
	}	
}
