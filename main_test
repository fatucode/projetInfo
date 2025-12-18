#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "reseaau.h"
#include "avl.h"
#include "calcul_fuites.h"

int main(void) {
    // 1. Charger le réseau depuis un fichier CSV
    AVLNode* racine = charger_reseau("c-wildwater_v0.csv");
    if (!racine) {
        printf("Erreur : impossible de charger le réseau.\n");
        return 1;
    }

    // 2. Demander l'identifiant de l'usine
    char id_usine[100];
    printf("Entrez l'identifiant de l'usine : ");
    if (!fgets(id_usine, sizeof(id_usine), stdin)) {
        printf("Erreur lecture identifiant.\n");
        return 1;
    }
    // Enlever le retour à la ligne si présent
    char* p = strchr(id_usine, '\n');
    if (p) *p = '\0';

    // 3. Calculer les fuites pour cette usine
    double pertes = calculer_fuites_usine(racine, id_usine);
    if (pertes < 0) {
        printf("Usine '%s' non trouvée dans le réseau.\n", id_usine);
        return 1;
    }

    printf("Volume d'eau perdue pour l'usine '%s' : %.3f M.m3\n", id_usine, pertes);

    // 4. Sauvegarder le résultat dans le fichier .dat
    sauvegarder_fuites(id_usine, pertes);
    printf("Résultat ajouté dans 'fuites.dat'.\n");

    return 0;
}
