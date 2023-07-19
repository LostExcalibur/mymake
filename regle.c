#include "regle.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

regle *nouvelle_regle(char *nom, int n_prerequis, int n_commandes) {
    regle *r = check_malloc(sizeof(regle));
    r->prerequis = check_malloc(sizeof(char *) * n_prerequis);
    r->commandes = check_malloc(sizeof(char *) * n_commandes);

    r->nom = nom;

    r->prerequis_actuel = 0;
    r->commande_actuelle = 0;
    return r;
}

void detruire_regle(regle *r) {
    /* for (int i = 0; i < r->prerequis_actuel; i++) {
            free(r->prerequis[i]);
    } */
    for (int i = 0; i < r->commande_actuelle; i++) {
        free(r->commandes[i]);
    }
    free(r->nom);
    free(r->commandes);
    free(r->prerequis);

    free(r);
}

void ajouter_prerequis(regle *r, char *prerequis) {
    r->prerequis[r->prerequis_actuel] = prerequis;

    r->prerequis_actuel++;
}

void ajouter_commande(regle *r, char *commande) {
    r->commandes[r->commande_actuelle] = commande;

    r->commande_actuelle++;
}

void afficher_regle(regle *r) {
    printf("%s : ", r->nom);
    for (int i = 0; i < r->prerequis_actuel; i++) {
        printf("%s ", r->prerequis[i]);
    }
    printf("\n");
    for (int i = 0; i < r->commande_actuelle; i++) {
        printf("%s", r->commandes[i]);
    }
    printf("\n");
}
