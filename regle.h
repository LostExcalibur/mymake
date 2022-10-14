#ifndef __REGLE_H
#define __REGLE_H

typedef struct Regle {
  char *nom;
  char **prerequis;
  char **commandes;

  int prerequis_actuel;
  int commande_actuelle;
} regle;

regle *nouvelle_regle(char *nom, int n_prerequis, int n_commandes);
void detruire_regle(regle *r);

void ajouter_prerequis(regle *r, char *prerequis);
void ajouter_commande(regle *r, char *commande);

void afficher_regle(regle *r);

#endif // __REGLE_H
