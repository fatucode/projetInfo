#include <stdio.h>
#include <stdlib.h>
#include "histogramme.h"

/*
 * Génère un fichier CSV contenant les données pour l'histogramme
 * - racine : AVL contenant toutes les usines
 * - nom_fichier : nom du fichier CSV de sortie
 * - mode : HISTO_MAX, HISTO_SRC, HISTO_REAL ou HISTO_ALL
 *
 * Retour :
 *  0  -> succès
 * -1  -> erreur (fichier non ouvert)
 */
int generer_histogramme(AvlUsine* racine,
                        const char* nom_fichier,
                        int mode)
{
    FILE* out = fopen(nom_fichier, "w");
    if (out == NULL) {
        return -1;
    }

    /* Écriture de l'en-tête du CSV */
    if (mode == HISTO_ALL) {
        fprintf(out, "id;max;src;real\n");
    } else {
        fprintf(out, "id;valeur\n");
    }

    /* Parcours de l'AVL en ordre inverse (Z -> A) */
    parcours_inverse(racine, out, mode);

    fclose(out);
    return 0;
}
