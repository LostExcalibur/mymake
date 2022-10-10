#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "regle.h"
#include "ens_regles.h"

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

int nombre_regles(FILE *fichier){
	int i = 0;
	int c;
	while((c = fgetc(fichier)) != EOF){
		i += c == ':';
	}
	fseek(fichier,0, SEEK_SET);
	return i;
}


ens_regles * lire(void){
	FILE * fichier;
	char * ligne = NULL;
	size_t len = 0;
	fichier = fopen("Makefile","r");
	if (fichier == NULL){
		fprintf(stderr,"Le fichier est introuvable");
	}

	int nb_regles = nombre_regles(fichier);
	ens_regles * ens = nouvel_ensemble(nb_regles);
	long debut = ftell(fichier);

	while(getline(&ligne, &len, fichier) != -1){
		if (ligne[0] != '\t'){
			char * nom_regle  = strtok(ligne, ":");

			char *prerequis;
			int nb_prerequis = 0;
			while((prerequis = strtok(NULL, " ")) != 0){
				nb_prerequis ++;
			}

			int nb_commandes = 0;
			while(getline(&ligne, &len, fichier) != -1){
				if(ligne[0] != '\t'){
					break;
				}
				nb_commandes += 1;
			}

			fseek(fichier,debut,SEEK_SET);
			regle * r = nouvelle_regle(nom_regle,nb_prerequis,nb_commandes);


			while((prerequis = strtok(NULL, " ")) != 0){
				ajouter_prerequis(r, prerequis);
			}


			while(getline(&ligne, &len, fichier) != -1){
				if(ligne[0] != '\t'){
					break;
				}
				ligne += 1;
				ajouter_commande(r,ligne);
			}

			ajouter_regle(ens, r);
			

		}
		debut = ftell(fichier);
	};
	fclose(fichier);
	return ens;
}

int main(void) {
	ens_regles* ens = lire();
	
	afficher_ensemble(ens);
	detruire_ensemble(ens);

	return 0;
}
