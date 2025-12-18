#ifndef RESEAAU_H
#define RESEAAU_H

#define MAX_ID 100

struct Noeud;  
struct AVLNode;
typedef struct AVLNode AVLNode;
typedef enum {
    SOURCE_USINE,
    USINE_NOEUD,
    USINE_STOCKAGE,
    STOCKAGE_JONCTION,
    JONCTION_USAGER,
    TRONCON_DISTRIBUTION,
    INCONNU
} TypeLigne;
typedef enum {
    NOEUD_SOURCE,
    NOEUD_USINE,
    NOEUD_STOCKAGE,
    NOEUD_JONCTION,
    NOEUD_RACCORDEMENT,
    NOEUD_USAGER,
    NOEUD_INCONNU
} TypeNoeud;

typedef struct Noeud Noeud;

typedef struct ChildNode {
    struct Noeud* enfant;       // pointeur vers le vrai nœud enfant
    struct ChildNode* suivant;  // maillon suivant
} ChildNode;

struct Noeud {
    char id[MAX_ID];
    TypeNoeud type;
    double volume;
    double fuite;

    ChildNode* enfants;         // liste d'enfants
} ;


// Prototypes
Noeud* creer_noeud(const char* id, TypeNoeud type);
void ajouter_enfant(Noeud* parent, Noeud* enfant);
AVLNode* charger_reseau(const char* filename);
TypeNoeud detecter_type(const char* id, int role);
#endif
