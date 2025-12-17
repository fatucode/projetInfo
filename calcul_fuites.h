
#ifndef CALCUL_FUITES_H 
#define CALCUL_FUITES_H 
#include <stdio.h>
#include "reseaau.h" 

// Lance le calcul à partir d’une usine racine 
double calculer_fuites(Noeud* racine); 
//Ecrit les fuites dans le fichier.dat
int ecrire_fuite_dat(const char* filename,
                     const char* id_usine,
                     double volume_perdu);


// Calcul récursif interne 
double parcourir(Noeud* n, double volume_entree);
 #endif
