#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reseaau.h"
#include "avl.h"
#include "csv_reader.h"

// --------------------------------------------------------
// Création d’un nœud
// --------------------------------------------------------
Noeud* creer_noeud(const char* id, TypeNoeud type) {
    Noeud* n = malloc(sizeof(Noeud));
    if (!n) return NULL;

    strncpy(n->id, id, MAX_ID);
    n->id[MAX_ID - 1] = '\0';

    n->type = type;
    n->volume = 0.0;
    n->fuite = 0.0;
    n->enfants = NULL;

    return n;
}

// --------------------------------------------------------
// Ajouter un enfant à un parent
// --------------------------------------------------------
void ajouter_enfant(Noeud* parent, Noeud* enfant) {
    if (!parent || !enfant) return;

    ChildNode* c = malloc(sizeof(ChildNode));
    c->enfant = enfant;
    c->suivant = parent->enfants;
    parent->enfants = c;
}

// --------------------------------------------------------
// Détecter le type d’un nœud selon son contenu (nom)
// --------------------------------------------------------
TypeLigne detecter_type_ligne(
    const char* c1,
    const char* c2,
    const char* c3,
    const char* c4,
    const char* c5
) {
    // Sécurité
    if (!c2) return INCONNU;

    // SOURCE -> USINE (captage)
    if (strcmp(c1, "-") == 0 &&
        strcmp(c3, "-") != 0 &&
        strcmp(c4, "-") != 0 &&
        strcmp(c5, "-") != 0)
        return SOURCE_USINE;

    // USINE seule (capacité)
    if (strcmp(c1, "-") == 0 &&
        strcmp(c3, "-") == 0 &&
        strcmp(c4, "-") != 0)
        return USINE_NOEUD;

    // USINE -> STOCKAGE
    if (strcmp(c1, "-") == 0 &&
        strcmp(c3, "-") != 0 &&
        strcmp(c4, "-") == 0 &&
        strcmp(c5, "-") != 0)
        return USINE_STOCKAGE;

    // MONDE 2 : DISTRIBUTION
    // STOCKAGE -> JONCTION
    // JONCTION -> RACCORDEMENT
    // RACCORDEMENT -> USAGER
    if (strcmp(c1, "-") != 0 &&
        strcmp(c3, "-") != 0 &&
        strcmp(c4, "-") == 0 &&
        strcmp(c5, "-") != 0)
        return TRONCON_DISTRIBUTION;

    return INCONNU;
}

// --------------------------------------------------------
// Construction du réseau complet depuis le CSV
// --------------------------------------------------------

void traiter_ligne_csv(const char* c1,
                       const char* c2,
                       const char* c3,
                       const char* c4,
                       const char* c5,
                       void* user_data)
{
    AVLNode** racine = (AVLNode**) user_data;

    // Détection du type de ligne
    TypeLigne type_ligne = detecter_type_ligne((char*)c1, (char*)c2, (char*)c3, (char*)c4, (char*)c5);

    const char *amont = c2;
    const char *aval  = c3;
    const char *volume = c4;
    const char *fuite  = c5;


    if (!amont || !aval || strcmp(amont, "-") == 0 || strcmp(aval, "-") == 0)
        return;

    // Déterminer le type de noeud selon le type de ligne et la position
    TypeNoeud type_amont = NOEUD_INCONNU;
    TypeNoeud type_aval  = NOEUD_INCONNU;

    switch (type_ligne) {
    case SOURCE_USINE:
        type_amont = NOEUD_SOURCE;
        type_aval  =  NOEUD_USINE;
        break;
    case USINE_NOEUD:
        type_amont =  NOEUD_USINE;
        type_aval  =  NOEUD_USINE; // noeud seul
        break;
    case USINE_STOCKAGE:
        type_amont =  NOEUD_USINE;
        type_aval  =  NOEUD_STOCKAGE;
        break;
    case STOCKAGE_JONCTION:
        type_amont =  NOEUD_STOCKAGE;
        type_aval  =  NOEUD_JONCTION;
        break;
    case JONCTION_USAGER:   // ici pour distribution secondaire / branchement
        type_amont =  NOEUD_JONCTION;
        type_aval  =  NOEUD_USAGER;
        break;
    case TRONCON_DISTRIBUTION: // ligne de distribution secondaire
        type_amont =  NOEUD_RACCORDEMENT;
        type_aval  =  NOEUD_USAGER;
        break;
    default:
        type_amont = NOEUD_INCONNU;
        type_aval  = NOEUD_INCONNU;
        break;
}

    // Recherche ou création des noeuds
    AVLNode* nA = avl_search(*racine, amont);
    if (!nA) {
        Noeud* na = creer_noeud(amont, type_amont);
        *racine = avl_insert(*racine, amont, na);
        nA = avl_search(*racine, amont);
    }

    AVLNode* nB = avl_search(*racine, aval);
    if (!nB) {
        Noeud* nb = creer_noeud(aval, type_aval);
        *racine = avl_insert(*racine, aval, nb);
        nB = avl_search(*racine, aval);
    }

    // Création du lien parent -> enfant
    ajouter_enfant(nA->ptr_noeud, nB->ptr_noeud);

    // Volume : uniquement pour Source → Usine ou Usine seule
    if ((type_ligne == SOURCE_USINE || type_ligne == USINE_NOEUD) &&
        volume && strcmp(volume, "-") != 0)
    {
        nB->ptr_noeud->volume = atof(volume);
    }

    // Fuite : pour toutes les lignes où elle est définie
    if (fuite && strcmp(fuite, "-") != 0)
        nB->ptr_noeud->fuite = atof(fuite);
}

AVLNode* charger_reseau(const char* filename)
{
    AVLNode* racine = NULL;

    int statut = csv_process_file(filename, traiter_ligne_csv, &racine);
    if (statut != 0) {
        fprintf(stderr, "Erreur : impossible de lire le fichier '%s'\n", filename);
        return NULL;
    }

    if (!racine) {
        fprintf(stderr, "Erreur : aucun noeud n'a été chargé depuis '%s'\n", filename);
        return NULL;
    }

    return racine;
}
