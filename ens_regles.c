#include "ens_regles.h"
#include "regle.h"
#include "util.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

ens_regles *nouvel_ensemble(int nb_regles) {
    ens_regles *ens = check_malloc(sizeof(ens_regles));

    ens->regles = check_calloc(nb_regles, sizeof(regle *));

    ens->regle_actuelle = 0;
    ens->premiere_regle = 0;

    return ens;
}

void detruire_ensemble(ens_regles *ens) {
    for (int i = 0; i < ens->regle_actuelle; i++) {
        detruire_regle(ens->regles[i]);
    }
    free(ens->regles);
    free(ens);
}

void ajouter_regle(ens_regles *ens, regle *r) {
    ens->regles[ens->regle_actuelle] = r;

    int i = ens->regle_actuelle;
    // On part du principe que les règles déjà présentes sont triées par ordre
    // croissant Quand on veut rajouter une règle, on n'a donc qu'à la faire
    // redescendre jusqu'à avoir atteint le début ou une règle plus petite dans
    // l'ordre lexicographique
    while (i > 0 && strcmp(ens->regles[i]->nom, ens->regles[i - 1]->nom) < 0) {
        swap((void **)&ens->regles[i], (void **)&ens->regles[i - 1]);
        // Il faut que l'index de la première règle du fichier reste correct,
        // donc il faut le modifier si on change la règle correspondante de
        // place
        if (ens->premiere_regle == i - 1) {
            ens->premiere_regle += 1;
        }

        i -= 1;
    }

    ens->regle_actuelle++;
}

regle *trouver_regle(ens_regles *ens, char *nom) {
    int debut = 0, fin = ens->regle_actuelle - 1;
    int milieu, res;

    while (debut <= fin) {
        milieu = (debut + fin) / 2;
        if ((res = strcmp(nom, ens->regles[milieu]->nom)) == 0) {
            return ens->regles[milieu];
        }
        if (res < 0) {
            fin = milieu - 1;
        } else {
            debut = milieu + 1;
        }
    }

    return NULL;
}

bool construire_opti(ens_regles *ens, char *nom, char *nom_parent) {
    regle *r = trouver_regle(ens, nom);

    // On n'a pas de règle correspondant à ce nom, c'est soit un fichier soit
    // une erreur
    if (r == NULL) {
        // Il n'y a pas de fichier qui correspond, on ne peut pas faire la règle
        if (access(nom, F_OK) != 0) {
            fprintf(stderr,
                    "Aucune règle pour fabriquer la cible «%s», nécessaire "
                    "pour %s\n",
                    nom, nom_parent);
            exit(1);
        }

        struct stat stat_buffer;
        if (stat(nom, &stat_buffer) == -1) {
            perror("stat");
            exit(1);
        }

        // On va comparer avec le parent, on vérifie que le parent n'est pas
        // NULL (argument passé au premier appel)
        if (nom_parent == NULL) {
            return true;
        }

        struct stat stat_parent;
        // Problème avec le parent, on suppose que le fichier n'existe
        // simplement pas (à cause d'un make clean par exemple) et donc on
        // construit dans le doute.
        if (stat(nom_parent, &stat_parent) == -1) {
            return true;
        }

        // Si il n'y a pas eu de problème, on compare les temps de
        // modifications
        return stat_buffer.st_mtime > stat_parent.st_mtime;
    }

    // Si on a des prérequis, on veut vérifier qu'ils ont besoin d'être
    // construits. Cependant, si on n'a pas de prérequis on veut tout le temps
    // construire la règle Exemple ./mymake clean
    bool modifie = r->prerequis_actuel == 0;

    for (int i = 0; i < r->prerequis_actuel; i++) {
        if (construire_opti(ens, r->prerequis[i], nom)) {
            modifie = true;
        }
    }
    if (modifie) {
        for (int i = 0; i < r->commande_actuelle; i++) {
            printf("%s", r->commandes[i] + 1);
            system(r->commandes[i]);
        }
    }
    return modifie;
}

void appliquer_ens_regle(ens_regles *ens, char *nom) {
    regle *r = trouver_regle(ens, nom);
    if (r == NULL) {
        fprintf(stderr, "Aucune règle pour fabriquer la cible «%s». Arrrêt.\n",
                nom);
        return;
    }

    if (!construire_opti(ens, nom, NULL)) {
        printf("La cible %s est à jour\n", nom);
    }
}

void afficher_ensemble(ens_regles *ens) {
    for (int i = 0; i < ens->regle_actuelle; i++) {
        afficher_regle(ens->regles[i]);
    }
}
