#include <stdio.h>
#include <stdlib.h>
#include "histogramme.h"

// Génère un fichier CSV à partir de l'AVL des usines
// Ce fichier servira ensuite à tracer l'histogramme
int generer_histogramme(AvlUsine* racine,
                        const char* nom_fichier,
                        int mode)
{
    // Ouverture du fichier CSV en écriture
    FILE* out = fopen(nom_fichier, "w");
    if (out == NULL) {
        // Erreur si le fichier ne peut pas être ouvert
        return -1;
    }

    // Écriture de la première ligne (en-tête du CSV)
    // Le contenu dépend du mode choisi
    if (mode == HISTO_ALL) {
        // Cas où on affiche toutes les valeurs
        fprintf(out, "id;max;src;real\n");
    } else {
        // Cas où on affiche une seule valeur
        fprintf(out, "id;valeur\n");
    }

    // Parcours de l'AVL en ordre inverse (de Z vers A)
    // Les données sont écrites directement dans le fichier
    parcours_inverse(racine, out, mode);

    // Fermeture du fichier
    fclose(out);

    // Succès
    return 0;
}
