#ifndef __VARIABLE_H
#define __VARIABLE_H

#include <stddef.h>
enum variable_type {
    INVALIDE,
    RECURSIVE,
    SIMPLE,
    IMMEDIAT,
    APPEND,
    CONDITIONNEL,
    SHELL,
    COUNT_VAR_TYPES
};

typedef struct {
    // nom n'est pas null-terminated, donc on stocke la longueur du nom;
    char *nom;
    size_t longueur;

    char *valeur;
    enum variable_type type;
} variable;

typedef struct {
    variable **items;
    size_t longueur;
    size_t capacite;
} variables_list;

variables_list *alloc_variables(void);
void nouvelle_variable(char *nom, size_t longueur, char *valeur,
                       enum variable_type type);

#endif // __VARIABLE_H