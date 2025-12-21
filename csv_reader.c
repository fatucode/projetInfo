#include <stdio.h>
#include "csv_reader.h"
#include <string.h>

// Fonction principale de lecture du fichier CSV
// - filename : nom du fichier CSV à lire
// - cb : fonction callback appelée pour chaque ligne
// - user_data : données utilisateur transmises au callback
int csv_process_file(const char* filename, csv_line_cb cb, void* user_data)
{
    // Ouverture du fichier en mode lecture
    FILE *f = fopen(filename, "r");
    if (!f) return 1; // Erreur si le fichier n'existe pas ou ne peut pas être ouvert

    char line[1024]; // Buffer pour stocker une ligne du fichier

    // Lecture et saut de la première ligne (en-tête du CSV)
    if (!fgets(line, sizeof(line), f)) {
        fclose(f);
        return 1; // Erreur si le fichier est vide
    }

    // Lecture ligne par ligne du fichier CSV
    while (fgets(line, sizeof(line), f)) {

        // Suppression des caractères de fin de ligne (\n ou \r\n)
        line[strcspn(line, "\r\n")] = 0;

        // Pointeur utilisé pour parcourir la ligne
        char *ptr = line;

        // Tableau pour stocker les 5 colonnes du CSV
        char *cols[5] = {NULL, NULL, NULL, NULL, NULL};

        // Découpage de la ligne en colonnes séparées par ';'
        for (int i = 0; i < 5; i++) {
            cols[i] = ptr;              // Début de la colonne courante
            char *sep = strchr(ptr, ';'); // Recherche du séparateur ';'
            if (sep) {
                *sep = '\0';            // Remplace ';' par fin de chaîne
                ptr = sep + 1;          // Avance vers la colonne suivante
            }
        }

        // Appel de la fonction callback avec les colonnes lues
        cb(cols[0], cols[1], cols[2], cols[3], cols[4], user_data);
    }

    // Fermeture du fichier après lecture complète
    fclose(f);
    return 0; // Succès
}
