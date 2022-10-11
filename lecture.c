#include "lecture.h"
#include "ens_regles.h"
#include "regle.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


int nombre_regles(FILE *fichier){
	fseek(fichier,0, SEEK_SET);
	int i = 0;
	int c;
	while((c = fgetc(fichier)) != EOF){
		i += c == ':';
	}
	fseek(fichier,0, SEEK_SET);
	return i;
}
// \0 _ _ _ _ a
// On lit la première ligne :
// 		on lit jusqu'aux : puis on lit jusqu'au premier caractère qui n'est pas un espace
//		on ajoute un, on lit jusqu'au prochain espace, puis jusqu'au prochain qui n'en est pas un

void ajouter_prochaine_regle(FILE* fichier, ens_regles* ens) {
    size_t taille_tampon = 32;
    char* ligne = malloc(taille_tampon * sizeof(char));
    if (ligne == NULL) {
        perror("malloc");
        exit(1);
    }

	size_t taille_ligne;

	// On saute les lignes vides
	do {
    	taille_ligne = getline(&ligne, &taille_tampon, fichier);
		if (taille_ligne == -1) {
			return;
		}
	} while (ligne[0] == '\n');
	// On enlève le \n
	ligne[taille_ligne - 1] = '\0';
	taille_ligne -= 1;

	assert(ligne[0] != '\t');

	int nb_prerequis = 0;
	int pos_fin_nom = strcspn(ligne, ": ");
	// On a un ou plusieurs espaces entre le nom de la règle et les deux points
	ligne[pos_fin_nom] = '\0';
	if (ligne[pos_fin_nom] != ':') {
		pos_fin_nom += (strspn(ligne + pos_fin_nom + 1, ": "));
	}
	
	// Calcul du nombre de prerequis
	for (int i = pos_fin_nom + 1; i < taille_ligne;) {
		i += strspn(ligne + i, " ");
		i += strcspn(ligne + i, " ");
		nb_prerequis += 1;
	}
    char* commande = NULL;

	int nb_commandes = 0;
	long debut = ftell(fichier);
    while (getline(&commande, &taille_tampon, fichier) != -1) {
		if (commande[0] != '\t') {
			break;
		}
		nb_commandes += 1;
	}
	fseek(fichier, debut, SEEK_SET);

	regle* r = nouvelle_regle(ligne, nb_prerequis, nb_commandes);

	char* suite_ligne = ligne + pos_fin_nom + 1;
	if (nb_prerequis > 0) {
		ajouter_prerequis(r, strtok(suite_ligne, " "));
		for (int i = 1; i < nb_prerequis; i ++) {
			ajouter_prerequis(r, strtok(NULL, " "));
		}
	}
	
 	while (getline(&commande, &taille_tampon, fichier) != -1) {
		if (commande[0] != '\t') {
			break;
		}
		ajouter_commande(r, commande); // on stocke le \t mais pas un pb normalement
		commande = NULL;
	}

	ajouter_regle(ens, r);
	// free(ligne);
	free(commande);
}

ens_regles* lire_fichier(const char* nom) {
	FILE * fichier;
	char * ligne = NULL;
	size_t len = 0;
	fichier = fopen(nom,"r");
	if (fichier == NULL){
		perror("fopen");
		exit(1);
	}

	int nb_regles = nombre_regles(fichier);
	printf("%d\n", nb_regles);
	ens_regles * ens = nouvel_ensemble(nb_regles);

	while (!feof(fichier)) {
		ajouter_prochaine_regle(fichier, ens);
	}

	fclose(fichier);
	return ens;
}