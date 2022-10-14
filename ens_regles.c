#include "ens_regles.h"
#include "regle.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

// TODO : check malloc
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

int auxilliaire(ens_regles* ens, char* nom, char* nom_parent){
	regle * r = trouver_regle(ens, nom);

	// On n'a pas de règle correspondant à ce nom, c'est soit un fichier soit une erreur
	if (r == NULL) {
		// Il y a un fichier qui correspond
		if (access(nom, F_OK) == 0) {
			struct stat stat_buffer;
			if (stat(nom, &stat_buffer) == -1) {
				perror("stat");
				exit(1);
			}	
			
			time_t modif = stat_buffer.st_mtime;
			struct stat stat_parent;

			// On va comparer avec le parent, on vérifie que le parent n'est pas NULL (argument passé au premier appel)
			if (nom_parent == NULL) {
				return 1;
			}
			// Problème avec le parent, on suppose que le fichier n'existe simplement pas (à cause d'un make clean par exemple) et donc on construit dans le doute.
			if (stat(nom_parent, &stat_parent) == -1) {
				return 1;
			}

			// Si il n'y a pas eu de problème, on compare les temps de modifications
			if (modif > stat_parent.st_mtime) {
				return 1;
			}

			return 0;
		}	
		fprintf(stderr,"Aucune règle pour fabriquer la cible %s, nécessaire pour %s\n", nom, nom_parent);
		exit(1);
	}

	// Si on a des prérequis, on veut vérifier qu'ils ont besoin d'être construits.
	// Cependant, si on n'a pas de prérequis on veut tout le temps construire la règle
	// Exemple ./mymake clean 
	int modifie = r->prerequis_actuel == 0;

	for (int i = 0; i < r->prerequis_actuel; i++){
		if (auxilliaire(ens, r->prerequis[i],nom)) {
			modifie = 1;
		}
	}
	if (modifie) {
		for (int i = 0; i < r->commande_actuelle; i++){
			printf("%s", r->commandes[i] + 1);
			system(r->commandes[i]);
		}
	}
	return modifie;
}

void appliquer_ens_regle(ens_regles* ens, char* nom) {
	regle * r = trouver_regle(ens, nom);
	if (r == NULL){
		fprintf(stderr,"Aucune règle pour fabriquer la cible «%s». Arrrêt.\n",nom);
		return ;
	}

	if (!auxilliaire(ens, nom,NULL)) {
		printf("La cible %s est à jour\n", nom);
	}
}



void afficher_ensemble(ens_regles* ens) {
	for (int i = 0; i < ens->regle_actuelle; i++) {
		afficher_regle(ens->regles[i]);
	}	
}
