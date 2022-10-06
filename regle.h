#ifndef __REGLE_H
#define __REGLE_H

typedef struct Regle {
	char* nom;
	char** prerequis;
	char** commandes;

	int n_prerequis;
	int n_commandes;
} regle;


#endif // __REGLE_H
