#ifndef HISTOGRAMME_H
#define HISTOGRAMME_H

#include "avl_usine.h"

// Modes d'histogramme
#define HISTO_MAX  0
#define HISTO_SRC  1
#define HISTO_REAL 2
#define HISTO_ALL  3

// Générer le CSV de l'histogramme
int generer_histogramme(AvlUsine* racine, const char* nom_fichier, int mode);

#endif

