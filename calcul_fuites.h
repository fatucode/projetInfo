#ifndef CALCUL_FUITES_H 
#define CALCUL_FUITES_H 

#include <stdio.h>
#include "reseaau.h" 
#include "avl.h"

// =====================================================
// Fonction récursive interne
// Parcourt le réseau à partir d’un nœud donné et calcule
// les pertes en fonction du volume entrant.
// - n : nœud courant du réseau
// - volume_entree : volume arrivant sur ce nœud
// Retourne la somme des pertes sur tous les sous-nœuds
// =====================================================
double parcourir(Noeud* n, double volume_entree);


// =====================================================
// Fonction principale de calcul des fuites
// Lance le calcul à partir d’une usine identifiée par son id.
// - racine : racine de l’AVL contenant tous les nœuds du réseau
// - id_usine : identifiant de l’usine à analyser
// Retourne les pertes totales (en millions de m³)
// =====================================================
double calculer_fuites_usine(NoeudAVL* racine, const char* id_usine);

// =====================================================
// Sauvegarde des résultats des fuites dans un fichier .dat
// - id_usine : identifiant de l’usine
// - pertes : valeur des pertes calculées
// Les résultats sont ajoutés à la fin du fichier
// =====================================================
void sauvegarder_fuites(const char* id_usine, double pertes);
#endif
