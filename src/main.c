#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "usine.h"
#include "avl_usine.h"
#include "histogramme.h"
#include "csv_reader.h"


// appelée pour chaque ligne du CSV
void traiter_ligne_csv(const char* col1,
                       const char* amont,
                       const char* aval,
                       const char* vol,
                       const char* fuite,
                       void* data)
{
    AvlUsine** racine = (AvlUsine**) data;

    // Si pas d'usine aval, on ignore la ligne
    if (strcmp(aval, "-") == 0) {
        return;
    }

    // Conversion des valeurs
    double max = atof(vol);
    double src = max;
    double real = max;

    // Si fuite connue, on la retire
    if (strcmp(fuite, "-") != 0) {
        real = max - atof(fuite);
    }

    // Création de l'usine
    Usine* u = creer_usine(aval, max, src, real);

    // Si échec, on ne fait rien
    if (u == NULL) {
        return;
    }

    // Insertion dans l'AVL
    *racine = inserer_avl_usine(*racine, u);
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s fichier.csv\n", argv[0]);
        return 1;
    }

    AvlUsine* racine = NULL;

    int res = csv_process_file(argv[1], traiter_ligne_csv, &racine);
    if (res != 0) {
        fprintf(stderr, "Erreur lecture CSV\n");
        return 1;
    }

    generer_histogramme(racine, "histo_test.csv", HISTO_MAX);

    liberer_avl_usine(racine);
    return 0;
}
