#include "variable.h"
#include "util.h"
#include <stddef.h>

variables_list *variables;

variables_list *alloc_variables(void) {
    variables_list *v = check_malloc(sizeof(variables_list));
    v->items = check_calloc(2, sizeof(variable *));
    v->capacite = 2;
    v->longueur = 0;

    return v;
}

void nouvelle_variable(char *nom, size_t longueur, char *valeur,
                       enum variable_type type) {
    if (variables->longueur >= variables->capacite) {
        variables->capacite *= 2;
        variables->items = check_realloc_array(
            variables->items, variables->capacite, sizeof(variable *));
    }

    variable *v = check_malloc(sizeof(variable));
    v->nom = nom;
    v->longueur = longueur;
    v->valeur = valeur;
    v->type = type;

    variables->items[variables->longueur] = v;
    variables->longueur++;
}