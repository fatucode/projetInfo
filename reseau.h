#ifndef RESEAU_H
#define RESEAU_H

#define MAX_ID 50

typedef enum {
    SOURCE,
    USINE,
    STOCKAGE,
    JONCTION,
    RACCORDEMENT,
    USAGER
} TypeNoeud;

typedef struct ChildNode {
    struct Noeud* enfant;       // pointeur vers le vrai nœud enfant
    struct ChildNode* suivant;  // maillon suivant
} ChildNode;

typedef struct Noeud {
    char id[MAX_ID];
    TypeNoeud type;
    double volume;
    double fuite;

    ChildNode* enfants;         // liste d'enfants
} Noeud;

// Prototypes
Noeud* creer_noeud(const char* id, TypeNoeud type);
void ajouter_enfant(Noeud* parent, Noeud* enfant);

#endif
