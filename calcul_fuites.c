#include <stdio.h>
#include "calcul_fuites.h"

// Fonction récursive qui parcourt le réseau à partir d’un nœud
// volume_entree : volume d’eau arrivant dans ce nœud
double parcourir(Noeud* n, double volume_entree)
{
    // Cas d’arrêt : nœud nul ou plus de volume à transmettre
    if (!n || volume_entree <= 0.0)
        return 0.0;

    // Comptage du nombre d’enfants du nœud courant
    int nb = 0;
    ChildNode* c = n->enfants;
    while (c) {
        nb++;
        c = c->suivant;
    }

    // S’il n’y a pas d’enfants, c’est un usager final :
    // il ne génère pas de pertes supplémentaires
    if (nb == 0)
        return 0.0;

    double pertes_totales = 0.0;

    // Répartition du volume entrant équitablement entre les enfants
    double volume_par_enfant = volume_entree / nb;

    // Parcours de chaque enfant
    c = n->enfants;
    while (c) {
        Noeud* enfant = c->enfant;

        // Récupération et sécurisation du pourcentage de fuite
        double fuite_pct = enfant->fuite;
        if (fuite_pct < 0) fuite_pct = 0;
        if (fuite_pct > 100) fuite_pct = 100;

        // Calcul de la perte sur ce tronçon
        double perte = volume_par_enfant * (fuite_pct / 100.0);

        // Volume restant après la fuite
        double volume_sortie = volume_par_enfant - perte;

        // Ajout de la perte locale
        pertes_totales += perte;

        // Propagation récursive vers les enfants
        pertes_totales += parcourir(enfant, volume_sortie);

        c = c->suivant;
    }

    // Retour du total des pertes calculées pour ce sous-réseau
    return pertes_totales;
}

// Fonction principale : calcule les fuites totales pour une usine donnée
double calculer_fuites_usine(NoeudAVL* racine, const char* id_usine)
{
    // Arbre AVL vide
    if (!racine) return -1.0;

    Noeud* n = NULL;

    if (id_usine) {
        // Recherche de l’usine dans l’AVL à partir de son identifiant
        NoeudAVL* n_avl = avl_recherche(racine, id_usine);
        if (!n_avl) return -1.0; // Usine non trouvée
        n = n_avl->ptr_noeud;
    } else {
        // Si aucun identifiant n’est fourni, on utilise la racine
        n = racine->ptr_noeud;
    }

    // Volume total entrant dans l’usine
    double volume_entree = n->volume;

    // Calcul récursif des pertes
    double pertes = parcourir(n, volume_entree);

    // Conversion en millions de mètres cubes
    return pertes / 1000.0;
}

// Fonction pour sauvegarder les résultats des fuites dans un fichier
void sauvegarder_fuites(const char* id_usine, double pertes)
{
    // Ouverture du fichier en mode ajout
    FILE* f = fopen("fuites.dat", "a");
    if (!f) {
        perror("Erreur ouverture fichier fuites.dat");
        return;
    }

    // Écriture de l’identifiant de l’usine et des pertes associées
    fprintf(f, "%s;%.3f M.m3\n", id_usine, pertes);

    // Fermeture du fichier
    fclose(f);
}
