#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "usine.h"
#include "avl_usine.h"
#include "histogramme.h"
#include "csv_reader.h"


// appelée pour chaque ligne du CSV
void traiter_ligne_csv(char** champs, int nb_champs, void* data) {
    // data = pointeur vers la racine de l'AVL
    AvlUsine** racine = (AvlUsine**) data;

    // on vérifie le nombre de champs
    if (nb_champs < 4) {
        return;
    }

    const char* id = champs[0];
    double max  = atof(champs[1]);
    double src  = atof(champs[2]);
    double real = atof(champs[3]);

    // Création de l'usine
    Usine* u = creer_usine(id, max, src, real);
    if (u == NULL) {
        return;
    }

    // Insertion dans l'AVL
    *racine = inserer_usine(*racine, u);
}
int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s fichier.csv\n", argv[0]);
        return 1;
    }

    AvlUsine* racine = NULL;

    // Lecture du CSV
    int res = lire_csv(argv[1], traiter_ligne_csv, &racine);
    if (res != 0) {
        fprintf(stderr, "Erreur lecture CSV\n");
        return 1;
    }

    // Test : générer un histogramme max
    generer_histogramme(racine, "histo_test.csv", HISTO_MAX);

    liberer_avl_usine(racine);
    return 0;
}



