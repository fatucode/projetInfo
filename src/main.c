#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv_reader.h"
#include "avl_usine.h"
#include "histogramme.h"

static void traiter_ligne(const char* c1,
                          const char* amont,
                          const char* aval,
                          const char* vol,
                          const char* fuite,
                          void* data)
{
    AvlUsine** racine = data;

    if (!strstr(aval, "Plant") && !strstr(aval, "Facility complex"))
        return;

    Usine* u = chercher_usine(*racine, aval);
    if (!u) {
        u = creer_usine(aval);
        *racine = inserer_usine(*racine, u);
    }

    double volume = atof(vol);
    double perte = (strcmp(fuite, "-") == 0) ? 0.0 : atof(fuite);

    u->src += volume;
    u->real += volume * (1.0 - perte / 100.0);

    if (volume > u->max)
        u->max = volume;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s fichier.csv histo <max|src|real>\n", argv[0]);
        return 1;
    }

    AvlUsine* racine = NULL;

    if (csv_process_file(argv[1], traiter_ligne, &racine) != 0) {
        fprintf(stderr, "Erreur CSV\n");
        return 1;
    }

    if (strcmp(argv[3], "max") == 0)
        generer_histogramme(racine, "histo_max.dat", 0);
    else if (strcmp(argv[3], "src") == 0)
        generer_histogramme(racine, "histo_src.dat", 1);
    else if (strcmp(argv[3], "real") == 0)
        generer_histogramme(racine, "histo_real.dat", 2);

    liberer_avl_usine(racine);
    return 0;
}
