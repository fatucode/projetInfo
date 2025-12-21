#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reseaau.h"
#include "avl.h"
#include "csv_reader.h"

/* --------------------------------------------------------
   Création d’un nœud du réseau
-------------------------------------------------------- */
Noeud* creer_noeud(const char* id, TypeNoeud type) {
    // Allocation mémoire pour le nœud
    Noeud* noeud = malloc(sizeof(Noeud));
    if (!noeud) return NULL;

    // Copie sécurisée de l'identifiant
    strncpy(noeud->id, id, MAX_ID);
    noeud->id[MAX_ID - 1] = '\0';

    // Initialisation des champs
    noeud->type = type;
    noeud->volume = 0.0;
    noeud->fuite = 0.0;
    noeud->enfants = NULL;

    return noeud;
}

/* --------------------------------------------------------
   Ajouter un enfant à un parent
-------------------------------------------------------- */
void ajouter_enfant(Noeud* parent, Noeud* enfant) {
    // Vérification des pointeurs
    if (!parent || !enfant) return;

    // Création du lien parent → enfant
    ChildNode* lien = malloc(sizeof(ChildNode));
    if (!lien) return;

    // Ajout en tête de la liste chaînée
    lien->enfant = enfant;
    lien->suivant = parent->enfants;
    parent->enfants = lien;
}

/* --------------------------------------------------------
   Détection du type de ligne CSV
-------------------------------------------------------- */
TypeLigne detecter_type_ligne(
    const char* c1,
    const char* c2,
    const char* c3,
    const char* c4,
    const char* c5
) {
    // Vérification des colonnes
    if (!c1 || !c2 || !c3 || !c4 || !c5)
        return INCONNU;

    // Cas SOURCE → USINE
    if (strcmp(c1, "-") == 0 &&
        strcmp(c3, "-") != 0 &&
        strcmp(c4, "-") != 0 &&
        strcmp(c5, "-") != 0)
        return SOURCE_USINE;

    // Cas USINE seule
    if (strcmp(c1, "-") == 0 &&
        strcmp(c3, "-") == 0 &&
        strcmp(c4, "-") != 0)
        return USINE_NOEUD;

    // Cas USINE → STOCKAGE
    if (strcmp(c1, "-") == 0 &&
        strcmp(c3, "-") != 0 &&
        strcmp(c4, "-") == 0 &&
        strcmp(c5, "-") != 0)
        return USINE_STOCKAGE;

    // Cas TRONÇON DE DISTRIBUTION
    if (strcmp(c1, "-") != 0 &&
        strcmp(c3, "-") != 0 &&
        strcmp(c4, "-") == 0 &&
        strcmp(c5, "-") != 0)
        return TRONCON_DISTRIBUTION;

    // Cas non reconnu
    return INCONNU;
}

/* --------------------------------------------------------
   Traitement d’une ligne CSV
-------------------------------------------------------- */
void traiter_ligne_csv(const char* c1,
                       const char* c2,
                       const char* c3,
                       const char* c4,
                       const char* c5,
                       void* user_data)
{
    // Récupération de la racine de l’AVL
    NoeudAVL** racine = (NoeudAVL**) user_data;

    // Détection du type de ligne
    TypeLigne type_ligne = detecter_type_ligne(c1, c2, c3, c4, c5);

    // Colonnes utiles
    const char* amont  = c2;
    const char* aval   = c3;
    const char* volume = c4;
    const char* fuite  = c5;

    // Ignorer les lignes invalides
    if (!amont || !aval || strcmp(amont, "-") == 0 || strcmp(aval, "-") == 0)
        return;

    // Types des nœuds amont et aval
    TypeNoeud type_amont = NOEUD_INCONNU;
    TypeNoeud type_aval  = NOEUD_INCONNU;

    // Définition des types selon la ligne CSV
    switch (type_ligne) {
        case SOURCE_USINE:
            type_amont = NOEUD_SOURCE;
            type_aval  = NOEUD_USINE;
            break;
        case USINE_NOEUD:
            type_amont = NOEUD_USINE;
            type_aval  = NOEUD_USINE;
            break;
        case USINE_STOCKAGE:
            type_amont = NOEUD_USINE;
            type_aval  = NOEUD_STOCKAGE;
            break;
        case TRONCON_DISTRIBUTION:
            type_amont = NOEUD_RACCORDEMENT;
            type_aval  = NOEUD_USAGER;
            break;
        default:
            return;
    }

    // Recherche ou création du nœud amont
    NoeudAVL* noeud_amont = avl_recherche(*racine, amont);
    if (!noeud_amont) {
        Noeud* na = creer_noeud(amont, type_amont);
        *racine = avl_insertion(*racine, amont, na);
        noeud_amont = avl_recherche(*racine, amont);
    }

    // Recherche ou création du nœud aval
    NoeudAVL* noeud_aval = avl_recherche(*racine, aval);
    if (!noeud_aval) {
        Noeud* nb = creer_noeud(aval, type_aval);
        *racine = avl_insertion(*racine, aval, nb);
        noeud_aval = avl_recherche(*racine, aval);
    }

    // Création du lien parent → enfant
    ajouter_enfant(noeud_amont->ptr_noeud, noeud_aval->ptr_noeud);

    // Gestion du volume
    if ((type_ligne == SOURCE_USINE || type_ligne == USINE_NOEUD) &&
        volume && strcmp(volume, "-") != 0)
    {
        noeud_aval->ptr_noeud->volume = atof(volume);
    }

    // Gestion des fuites
    if (fuite && strcmp(fuite, "-") != 0)
        noeud_aval->ptr_noeud->fuite = atof(fuite);
}

/* --------------------------------------------------------
   Chargement du réseau depuis le fichier CSV
-------------------------------------------------------- */
NoeudAVL* charger_reseau(const char* filename)
{
    // Racine de l’AVL du réseau
    NoeudAVL* racine = NULL;

    // Lecture du fichier CSV
    if (csv_process_file(filename, traiter_ligne_csv, &racine) != 0) {
        fprintf(stderr, "Erreur : lecture du fichier '%s'\n", filename);
        return NULL;
    }

    // Vérification réseau non vide
    if (!racine) {
        fprintf(stderr, "Erreur : réseau vide\n");
        return NULL;
    }

    return racine;
}
