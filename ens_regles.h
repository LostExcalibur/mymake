#ifndef __ENS_REGLES_H
#define __ENS_REGLES_H

#include "regle.h"
#include <stddef.h>

typedef struct Ens_Regles {
    regle **regles;
    size_t capacite;
    size_t longueur;

    int premiere_regle;
} ens_regles;

ens_regles *nouvel_ensemble();
void detruire_ensemble(ens_regles *ens);

void ajouter_regle(ens_regles *ens, regle *r);

// Renvoit un pointeur vers la règle ayant le nom correspondant.
// Renvoit NULL si elle n'existe pas dans l'ensemble
regle *trouver_regle(ens_regles *ens, char *nom);

void afficher_ensemble(ens_regles *ens);
void appliquer_ens_regle(ens_regles *ens, char *nom);

#endif // __ENS_REGLES_H
