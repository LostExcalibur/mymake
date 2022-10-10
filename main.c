#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "regle.h"

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

void lire(void){
	FILE * fichier;
	char * ligne = NULL;
	size_t len = 0;
	ssize_t nread;
	fichier = fopen("Makefile","r");
	if (fichier == NULL){
		fprintf(stderr,"Le fichier est introuvable");
	}
	while((nread = getline(&ligne, &len, fichier))!= -1){
		if (ligne[0] != '\t'){
			char *coupe = strtok(ligne, " ");
			int nb_mots = strlen(coupe);
			for (int i=0; i < nb_mots ; i++){
				printf("%c",coupe[i]);
			}
		};
	};
	free(ligne);
	fclose(fichier);
}

int main(void) {
	printf("Hello, World !\n");
	lire();

	return 0;
}
