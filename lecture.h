#ifndef __LECTURE_H
#define __LECTURE_H

#include "ens_regles.h"
#include "util.h"

string_da *lire_fichier(const char *nom);
ens_regles *parser_lignes(string_da* ens);

#endif // __LECTURE_H