#include "lecture.h"
#include "ens_regles.h"
#include "regle.h"
#include "util.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
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

uint32_t parse_prochaines_lignes(size_t premiere_ligne, ens_regles *ens,
                                 string_da *lignes) {
    if (premiere_ligne >= lignes->length) {
        return 0;
    }

    size_t indice_ligne = premiere_ligne;

    char *ligne = lignes->data[indice_ligne];
    // On saute les lignes vides
    while (ligne_est_vide(ligne)) {
        indice_ligne++;
        if (indice_ligne >= lignes->length) {
            return indice_ligne - premiere_ligne;
        }
        ligne = lignes->data[indice_ligne];
    }

    size_t taille_ligne;
    // On enlève le \n
    {
        taille_ligne = strlen(ligne);

        if (taille_ligne >= 2) {
            while (indice_ligne + 1 < lignes->length) {
                // Ligne normale
                if (ligne[taille_ligne - 2] != '\\') {
                    break;
                }
                // Ligne multiple
                debug("Ligne multiple détectée : %s", ligne);
                char *prochaine = lignes->data[indice_ligne + 1];
                debug("Prochaine ligne : %s", prochaine);
                size_t taille_prochaine = strlen(prochaine);
                size_t whitespace = strspn(prochaine, "\t ");
                debug("Taille de whitespace : %zu, taille de prochaine : %zu\n",
                      whitespace, taille_prochaine);
                ligne[taille_ligne - 2] = ' ';

                ligne = check_realloc(ligne, taille_ligne + taille_prochaine -
                                                 whitespace);
                memcpy(ligne + taille_ligne - 1, prochaine + whitespace,
                       taille_prochaine - whitespace + 1);

                debug("Ligne finale : %s", ligne);
                taille_ligne += taille_prochaine - whitespace - 1;
                string_da_delete(lignes, indice_ligne + 1);
            }
        }

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
    for (size_t i = curseur + 1; i < taille_ligne;
         i += strspn(ligne + i, " ")) {
        nb_prerequis += 1;
        i += strcspn(ligne + i, " ");
    }
    debug("Trouvé %d prérequis\n", nb_prerequis);

    // Calcul du nombre de commandes
    indice_ligne++;
    ligne = lignes->data[indice_ligne];
    int nb_commandes = 0;
    while (indice_ligne < lignes->length && ligne[0] == '\t') {
        nb_commandes += 1;
        indice_ligne++;
        ligne = lignes->data[indice_ligne];
    }
    // TODO : supporter les cibles sans commandes
    assert(nb_commandes > 0 && "Makefile mal formé");
    debug("Trouvé %d commandes\n", nb_commandes);

    // On revient sur la ligne du nom de la règle
    indice_ligne -= nb_commandes + 1;
    ligne = lignes->data[indice_ligne];

    regle *r = nouvelle_regle(ligne, nb_prerequis, nb_commandes);

    char *suite_ligne = ligne + debut_prerequis;
    if (nb_prerequis > 0) {
        ajouter_prerequis(r, strtok(suite_ligne, " "));
        for (int i = 1; i < nb_prerequis; i++) {
            ajouter_prerequis(r, strtok(NULL, " "));
        }
    }

    indice_ligne++;
    ligne = lignes->data[indice_ligne];

    while (indice_ligne < lignes->length && ligne[0] == '\t') {
        // on stocke le \t mais pas un pb normalement
        ajouter_commande(r, ligne);
        indice_ligne++;
        ligne = lignes->data[indice_ligne];
    }

    ajouter_regle(ens, r);

    return indice_ligne - premiere_ligne;
}

void parse_tout_fichier(ens_regles *ens, string_da *lignes) {
    size_t premiere_ligne = 0;

    while (premiere_ligne < lignes->length) {
        premiere_ligne += parse_prochaines_lignes(premiere_ligne, ens, lignes);
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

    parse_tout_fichier(ens, lignes);
    return ens;
}
