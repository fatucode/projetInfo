#ifndef CSV_READER_H
#define CSV_READER_H

#include <stdio.h> 


// Type de fonction callback appelée pour chaque ligne du fichier CSV
// col1   : première colonne (souvent un identifiant ou inutilisée)
// amont  : identifiant du nœud amont
// aval   : identifiant du nœud aval
// volume : volume associé au tronçon
// fuite  : pourcentage de fuite
// user_data : pointeur générique permettant de passer des données (ex : racine de l'AVL)
typedef void (*csv_line_cb)(
    const char* col1,
    const char* amont,
    const char* aval,
    const char* volume,
    const char* fuite,
    void* user_data
);

// Fonction principale de lecture du fichier CSV
// filename : chemin vers le fichier CSV
// cb       : fonction callback appelée pour chaque ligne lue
// user_data: données utilisateur transmises à la callback
int csv_process_file(const char* filename, csv_line_cb cb, void* user_data);

#endif
