#ifndef __REGLE_H
#define __REGLE_H

typedef struct Regle {
	char* nom;
	char** prerequis;
	char** commandes;

	int n_prerequis;
	int n_commandes;
} regle;

void applique_regle(regle* r);
regle* nouvelle_regle(char* nom);
void detruire_regle(regle* r);


#endif // __REGLE_H
