
#include <stdio.h>
#include "csv_reader.h"
#include <string.h>

int csv_process_file(const char* filename, csv_line_cb cb, void* user_data) {
    FILE *f = fopen(filename, "r");
    if (!f) return 1;

    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        // on coupe la ligne en colonnes sans modifier l'original (sscanf)
        char c1[200] = "-", c2[200] = "-", c3[200] = "-", c4[200] = "-", c5[200] = "-";
        int n = sscanf(line, "%199[^;];%199[^;];%199[^;];%199[^;];%199[^\n]",
                       c1, c2, c3, c4, c5);

        // Normaliser : si une colonne lue vaut "-" on renvoie la chaîne "-".
        const char *col1 = (n >= 1) ? c1 : "-";
        const char *amont = (n >= 2) ? c2 : "-";
        const char *aval  = (n >= 3) ? c3 : "-";
        const char *vol   = (n >= 4) ? c4 : "-";
        const char *fuite = (n >= 5) ? c5 : "-";

        // Appel du callback ; l'utilisateur décide si il strdup() ou pas
        cb(col1, amont, aval, vol, fuite, user_data);
    }

    fclose(f);
    return 0;
}

