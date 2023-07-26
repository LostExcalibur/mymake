#include <stdio.h>

#include "ens_regles.h"
#include "lecture.h"
#include "regle.h"
#include "util.h"

// clang-format off
/*
 *             ///((/////                   *&&&&&&&&              %&&&&&&&&    
 *          //(/////((//////              /&&&     *&&&          &&&&     %&&&  
 *        /(//^     ^//(//(///            &&&                   #&&             
 *      ///^,        //////////           &&&                    &&&            
 *               //////////(///            &&&&&&         *%&(    &&&&&&%       
 *             //////(///////////          &&&&&&      &&&&&&&&&&%   *&&&&&&&   
 *             ^^^////////(//^////        &&&        %&&#       &&&        &&&* 
 *             ^^^/(/////////^////        &&&        &&&        #&&         &&% 
 *          ^^^  ////////(////^^^^        (&&&     *&&&          &&&%     #&&&  
 *       ^^^^^^^^/(////((////^ ^^^^         (&&&&&&&&              &&&&&&&&&                               __     _  __   __
 *     .^^^///////////////////                                       |  __ \                              |__ \ / _ \__ \|__ \
 *     /^///////,..//////^//////^                                    | |__) | __ ___  _ __ ___   ___         ) | | | | ) |  ) |     
 * ////////////#////////    ^^^^^                                    |  ___/ '__/ _ \| '_ ` _ \ / _ \       / /| | | |/ /  / /   
 *  /////((((////////        ^^^^                                    | |   | | | (_) | | | | | | (_) |     / /_| |_| / /_ / /_ 
 *   ////////                                                        |_|   |_|  \___/|_| |_|_|_|\___/     |____|\___/____|____|
*/
// clang-format on

int main(int argc, char **argv) {
    string_da *lignes = lire_fichier("Makefile");
    ens_regles *ens = parser_lignes(lignes);

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            appliquer_ens_regle(ens, argv[i]);
        }
    } else {
        appliquer_ens_regle(ens, ens->regles[ens->premiere_regle]->nom);
    }

    string_da_free(lignes);
    detruire_ensemble(ens);

    return 0;
}
