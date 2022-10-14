#ifndef __ENS_REGLES_H
#define __ENS_REGLES_H

#include "regle.h"

typedef struct Ens_Regles {
  regle **regles;

  int regle_actuelle;
  int premiere_regle;
} ens_regles;

ens_regles *nouvel_ensemble(int nb_regles);
void detruire_ensemble(ens_regles *ens);

void ajouter_regle(ens_regles *ens, regle *r);

// Renvoit un pointeur vers la règle ayant le nom correspondant.
// Renvoit NULL si elle n'existe pas dans l'ensemble
regle *trouver_regle(ens_regles *ens, char *nom);

void afficher_ensemble(ens_regles *ens);
void appliquer_ens_regle(ens_regles *ens, char *nom);

#endif // __ENS_REGLES_H
