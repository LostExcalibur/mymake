#include "ens_regles.h"
#include "regle.h"
#include "util.h"

#include <asm-generic/errno-base.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

ens_regles *nouvel_ensemble() {
    ens_regles *ens = check_malloc(sizeof(ens_regles));

    ens->regles = check_calloc(2, sizeof(regle *));

    ens->capacite = 2;
    ens->longueur = 0;
    ens->premiere_regle = 0;

    return ens;
}

void detruire_ensemble(ens_regles *ens) {
    for (size_t i = 0; i < ens->longueur; i++) {
        detruire_regle(ens->regles[i]);
    }
    free(ens->regles);
    free(ens);
}

void ajouter_regle(ens_regles *ens, regle *r) {
    assert(ens->longueur <= ens->capacite && "Sanity check");
    assert(0 < ens->capacite && "Sanity check");

    if (ens->longueur == ens->capacite) {
        ens->capacite *= 2;
        debug("Reallocing array to %zu elements\n", ens->capacite);

        ens->regles =
            check_realloc_array(ens->regles, ens->capacite, sizeof(char *));
    }

    ens->regles[ens->longueur] = r;

    int i = ens->longueur;
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

    ens->longueur++;
}

regle *trouver_regle(ens_regles *ens, char *nom) {
    int debut = 0, fin = ens->longueur - 1;
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

bool construire_opti(ens_regles *ens, char *nom, char *nom_parent,
                     time_t date_min) {
    regle *r = trouver_regle(ens, nom);

    // On n'a pas de règle correspondant à ce nom, c'est soit un fichier soit
    // une erreur
    if (r == NULL) {
        struct stat stat_buffer;
        if (stat(nom, &stat_buffer) == -1) {
            // Il n'y a pas de fichier qui correspond, on ne peut pas faire la
            // règle
            if (errno == ENOENT) {
                fprintf(stderr,
                        "Aucune règle pour fabriquer la cible «%s», nécessaire "
                        "pour %s\n",
                        nom, nom_parent);
                exit(1);
                // Une autre erreur s'est produite
            } else {
                perror(nom);
                exit(1);
            }
        }

        // On va comparer avec le parent, on vérifie que le parent n'est pas
        // NULL (argument passé au premier appel)
        if (nom_parent == NULL) {
            return true;
        }

        struct stat stat_parent;
        // Cas où le parent pourrait ne pas exister à cause d'un make clean par
        // exemple
        if (stat(nom_parent, &stat_parent) == -1) {
            // Ce n'est normal que si le parent n'existe pas
            if (errno == ENOENT)
                return true;

            perror("stat");
            exit(1);
        }

        // Si la cible est plus récente que son parent, on refait.
        if (stat_buffer.st_mtime > stat_parent.st_mtime) {
            return true;
        }

        // Sinon, si le makefile est plus récent que le fichie et son parent,
        // on refait
        return date_min > stat_buffer.st_mtime &&
               date_min > stat_parent.st_mtime;
    }

    // Si on a des prérequis, on veut vérifier qu'ils ont besoin d'être
    // construits. Cependant, si on n'a pas de prérequis on veut tout le temps
    // construire la règle Exemple ./mymake clean
    bool modifie = r->prerequis_actuel == 0;

    for (int i = 0; i < r->prerequis_actuel; i++) {
        if (construire_opti(ens, r->prerequis[i], nom, date_min)) {
            modifie = true;
        }
    }
    if (modifie) {
        for (int i = 0; i < r->commande_actuelle; i++) {
            printf("%s", r->commandes[i] + 1);
            // if (false)
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

    struct stat make_stat;
    if (stat("Makefile", &make_stat) == -1) {
        perror("Makefile");
        exit(1);
    }

    time_t date_min = make_stat.st_mtime;

    if (!construire_opti(ens, nom, NULL, date_min)) {
        printf("La cible %s est à jour\n", nom);
    }
}

void afficher_ensemble(ens_regles *ens) {
    for (size_t i = 0; i < ens->longueur; i++) {
        afficher_regle(ens->regles[i]);
    }
}
