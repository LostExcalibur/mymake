#include "lecture.h"
#include "ens_regles.h"
#include "regle.h"
#include "util.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void eliminer_commentaires(string_da *lignes) {
    for (size_t i = 0; i < lignes->length; i++) {
        char *ligne = lignes->data[i];

        if (ligne[strspn(ligne, " \t")] == '#') {
            string_da_delete(lignes, i);
            i -= 1;
        }
    }
}

bool ligne_est_vide(const char *ligne) {
    int premier_char = strspn(ligne, " \t");

    return ligne[premier_char] == '\n';
}

uint32_t ajouter_prochaine_regle(size_t premiere_ligne, ens_regles *ens,
                                 string_da *lignes) {
    if (premiere_ligne > lignes->length) {
        return 0;
    }
    char **ligne_iter = lignes->data + premiere_ligne;
    char *ligne = *ligne_iter;
    // On saute les lignes vides
    while (ligne_est_vide(ligne)) {
        ligne_iter++;
        if (ligne_iter >= lignes->data + lignes->length) {
            return (ligne_iter - lignes->data);
        }
        ligne = *ligne_iter;
    }

    size_t taille_ligne;
    // On enlève le \n
    {
        taille_ligne = strlen(ligne);
        ligne[taille_ligne - 1] = '\0';
        taille_ligne -= 1;

        // assert(ligne[0] != '\t');
    }

    int nb_prerequis = 0;
    int curseur = 0;
    // Équivalent à la regex suivante : nom_de_fichier\s*:\s*dependances
    {
        int pos_fin_nom = strcspn(ligne, ": ");
        curseur = pos_fin_nom + strspn(ligne + pos_fin_nom, " ");
        debug("Ligne actuelle : %s\n", ligne);
        assert(ligne[curseur] == ':' && "Makefile mal formé");
        ligne[pos_fin_nom] = '\0';

        curseur += strspn(ligne + curseur + 1, " ");
    }

    int debut_prerequis = curseur + 1;
    // Calcul du nombre de prerequis
    for (size_t i = curseur + 1; i < taille_ligne; i += strspn(ligne + i, " ")) {
        nb_prerequis += 1;
        i += strcspn(ligne + i, " ");
    }
    debug("Trouvé %d prérequis\n", nb_prerequis);

    // Calcul du nombre de commandes
    ligne_iter++;
    ligne = *ligne_iter;
    int nb_commandes = 0;
    while (ligne_iter < lignes->data + lignes->length && ligne[0] == '\t') {
        nb_commandes += 1;
        ligne_iter++;
        ligne = *ligne_iter;
    }
    assert(nb_commandes > 0 && "Makefile mal formé");
    debug("Trouvé %d commandes\n", nb_commandes);

    // On revient sur la ligne du nom de la règle
    ligne_iter -= nb_commandes + 1;
    ligne = *ligne_iter;

    regle *r = nouvelle_regle(ligne, nb_prerequis, nb_commandes);

    char *suite_ligne = ligne + debut_prerequis;
    if (nb_prerequis > 0) {
        ajouter_prerequis(r, strtok(suite_ligne, " "));
        for (int i = 1; i < nb_prerequis; i++) {
            ajouter_prerequis(r, strtok(NULL, " "));
        }
    }

    ligne_iter++;
    ligne = *ligne_iter;

    while (ligne_iter < lignes->data + lignes->length && ligne[0] == '\t') {
        // on stocke le \t mais pas un pb normalement
        ajouter_commande(r, ligne);
        ligne_iter++;
        ligne = *ligne_iter;
    }

    ajouter_regle(ens, r);

    return ligne_iter - (lignes->data + premiere_ligne);
}

void parse_toutes_regles(ens_regles *ens, string_da *lignes) {
    size_t premiere_ligne = 0;

    while (premiere_ligne < lignes->length) {
        debug("Ligne : %s\n", lignes->data[premiere_ligne]);
        premiere_ligne += ajouter_prochaine_regle(premiere_ligne, ens, lignes);
    }
}

string_da *lire_fichier(const char *nom) {
    FILE *fichier;
    fichier = fopen(nom, "r");
    if (fichier == NULL) {
        perror("fopen");
        exit(1);
    }

    string_da *lignes = string_da_alloc();

    size_t n;
    char *ligne = NULL;

    while (getline(&ligne, &n, fichier) != -1) {
        string_da_append(lignes, ligne);
        ligne = NULL;
    }
    fclose(fichier);
    free(ligne);

    eliminer_commentaires(lignes);
    return lignes;
}

ens_regles *parser_lignes(string_da *lignes) {
    // int nb_regles = nombre_regles(lignes);
    ens_regles *ens = nouvel_ensemble();

    parse_toutes_regles(ens, lignes);
    return ens;
}
