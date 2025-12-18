
#ifndef CALCUL_FUITES_H 
#define CALCUL_FUITES_H 
#include <stdio.h>
#include "reseaau.h" 
#include "avl.h"

// Calcul récursif interne 
double parcourir(Noeud* n, double volume_entree);

// Lance le calcul à partir d’une usine racine 
double calculer_fuites_usine(AVLNode* racine, const char* id_usine);
void sauvegarder_fuites(const char* id_usine, double pertes);

 #endif
