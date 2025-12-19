#include <stdio.h>
#include "calcul_fuites.h"

double parcourir(Noeud* n, double volume_entree)
{
    if (!n || volume_entree <= 0.0)
        return 0.0;

    // Compter les enfants
    int nb = 0;
    ChildNode* c = n->enfants;
    while (c) {
        nb++;
        c = c->suivant;
    }

    if (nb == 0)
        return 0.0; // un usager final ne perd rien par lui-même

    double pertes_totales = 0.0;
    double volume_par_enfant = volume_entree / nb;

    // Recalcul sur chaque enfant
    c = n->enfants;
    while (c) {
        Noeud* enfant = c->enfant;

        double fuite_pct = enfant->fuite;
        if (fuite_pct < 0) fuite_pct = 0;
        if (fuite_pct > 100) fuite_pct = 100;

        // Perte sur ce tronçon
        double perte = volume_par_enfant * (fuite_pct / 100.0);
        double volume_sortie = volume_par_enfant - perte;

        pertes_totales += perte;

        // Propagation récursive
        pertes_totales += parcourir(enfant, volume_sortie);

        c = c->suivant;
    }

    return pertes_totales;
}

//Fonction principale : calcul des fuites pour une usine
double calculer_fuites_usine(AVLNode* racine, const char* id_usine)
{
    if (!racine) return -1.0;

    Noeud* n = NULL;

    if (id_usine) {
        // Chercher le nœud correspondant à l'usine
        AVLNode* n_avl = avl_search(racine, id_usine);
        if (!n_avl) return -1.0; // Usine non trouvée
        n = n_avl->ptr_noeud;
    } else {
        // Aucun id fourni : utiliser la racine
        n = racine->ptr_noeud;
    }

    double volume_entree = n->volume;
    double pertes = parcourir(n, volume_entree);

    return pertes / 1000.0; // Conversion en millions de m³
}

// Fonction pour écrire ou ajouter les résultats dans un fichier .dat
void sauvegarder_fuites(const char* id_usine, double pertes)
{
    FILE* f = fopen("fuites.dat", "a");
    if (!f) {
        perror("Erreur ouverture fichier fuites.dat");
        return;
    }

    fprintf(f, "%s;%.3f M.m3\n", id_usine, pertes);
    fclose(f);
}
