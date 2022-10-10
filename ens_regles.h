#ifndef __ENS_REGLES_H
#define __ENS_REGLES_H

#include "regle.h"

typedef struct Ens_Regles {
	regle** regles;

	int regle_actuelle;
} ens_regles;


ens_regles* nouvel_ensemble(int nb_regles);
void detruire_ensemble(ens_regles* ens);

void ajouter_regle(ens_regles* ens, regle* r);
regle* trouver_regle(ens_regles* ens, char* nom);

void afficher_ensemble(ens_regles* ens);

#endif // __ENS_REGLES_H
