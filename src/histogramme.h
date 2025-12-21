#ifndef HISTOGRAMME_H
#define HISTOGRAMME_H

#include "avl_usine.h"   // Définition de la structure AvlUsine

// Modes possibles pour l'histogramme
#define HISTO_MAX  0     // Histogramme des valeurs maximales
#define HISTO_SRC  1     // Histogramme des volumes sources
#define HISTO_REAL 2     // Histogramme des volumes réels
#define HISTO_ALL  3     // Histogramme avec toutes les valeurs

// Génère un fichier CSV pour l'histogramme
// - racine : AVL contenant les usines
// - nom_fichier : nom du fichier CSV à créer
// - mode : type d'histogramme à générer
int generer_histogramme(AvlUsine* racine, const char* nom_fichier, int mode);

#endif
