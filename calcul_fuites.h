
#ifndef CALCUL_FUITES_H 
#define CALCUL_FUITES_H 
#include <stdio.h>
#include "reseaau.h" 

// Lance le calcul à partir d’une usine racine 
double calculer_fuites(Noeud* racine); 

// Calcul récursif interne 
double parcourir(Noeud* n, double volume_entree);
 #endif
