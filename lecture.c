#include "lecture.h"
#include "ens_regles.h"
#include "regle.h"
#include "util.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Pour pouvoir construire l’ensemble de règles, il nous
// faut d’abord connaître le nombre de règle. On fait donc
// un premier parcours du Makefile, où on compte le nombre
// de ':', qui est équivalent au nombre de règles.
int nombre_regles(FILE *fichier) {
    fseek(fichier, 0, SEEK_SET);
    int i = 0;
    int c;
    while ((c = fgetc(fichier)) != EOF) {
        i += (c == ':');
    }
    fseek(fichier, 0, SEEK_SET);
    return i;
}

// On lit la première ligne :
// 		on lit jusqu'aux ":" puis on lit jusqu'au premier caractère qui
// n'est pas un espace
//		on ajoute un, on lit jusqu'au prochain espace, puis jusqu'au
// prochain qui n'en est pas un
void ajouter_prochaine_regle(FILE *fichier, ens_regles *ens) {
    size_t taille_tampon = 32;
    char *ligne = check_malloc(taille_tampon * sizeof(char));

    size_t taille_ligne;

    // On saute les lignes vides
    do {
        taille_ligne = getline(&ligne, &taille_tampon, fichier);
        if (taille_ligne == -1) {
            free(ligne);
            return;
        }
    } while (ligne[0] == '\n');
    // On enlève le \n
    ligne[taille_ligne - 1] = '\0';
    taille_ligne -= 1;

    assert(ligne[0] != '\t');

    int nb_prerequis = 0;
    int curseur = 0;

    // Équivalent à la regex suivante : nom_de_fichier\s*:\s*dependances
    int pos_fin_nom = strcspn(ligne, ": ");
    curseur = pos_fin_nom + strspn(ligne + pos_fin_nom, " ");

    assert(ligne[curseur] == ':' && "Makefile mal formé");
    ligne[pos_fin_nom] = '\0';

    curseur += strspn(ligne + curseur + 1, " ");

    int debut_prerequis = curseur + 1;

    // Calcul du nombre de prerequis
    for (int i = curseur + 1; i < taille_ligne; i += strspn(ligne + i, " ")) {
        nb_prerequis += 1;
        i += strcspn(ligne + i, " ");
    }
    char *commande = check_malloc(taille_tampon);

    // Calcul du nombre de commandes
    int nb_commandes = 0;
    long debut = ftell(fichier); // On enregistre l’endroit où on est dans le
                                 // fichier, pour pouvoir y revenir après et
                                 // parcourir les commandes une seconde fois
    while (getline(&commande, &taille_tampon, fichier) != -1) {
        if (commande[0] != '\t') {
            break;
        }
        nb_commandes += 1;
    }
    fseek(fichier, debut,
          SEEK_SET); // On retourne à l’endroit du fichier enregistré dans
                     // "début", pour pouvoir faire un second parcours des
                     // commades.

    regle *r = nouvelle_regle(ligne, nb_prerequis, nb_commandes);

    char *suite_ligne = ligne + debut_prerequis;
    if (nb_prerequis > 0) {
        ajouter_prerequis(r, strtok(suite_ligne, " "));
        for (int i = 1; i < nb_prerequis; i++) {
            ajouter_prerequis(r, strtok(NULL, " "));
        }
    }

    while (getline(&commande, &taille_tampon, fichier) != -1) {
        if (commande[0] != '\t') {
            break;
        }
        ajouter_commande(
            r, commande); // on stocke le \t mais pas un pb normalement
        commande = NULL;
    }

    ajouter_regle(ens, r);
    // free(ligne);
    free(commande);
}

ens_regles *lire_fichier(const char *nom) {
    FILE *fichier;
    fichier = fopen(nom, "r");
    if (fichier == NULL) {
        perror("fopen");
        exit(1);
    }

    int nb_regles = nombre_regles(fichier);
    ens_regles *ens = nouvel_ensemble(nb_regles);

    while (!feof(fichier)) {
        ajouter_prochaine_regle(fichier, ens);
    }

    fclose(fichier);
    return ens;
}
