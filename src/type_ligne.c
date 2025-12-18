#ifndef TYPE_LIGNE_H
#define TYPE_LIGNE_H

typedef enum {
    INCONNU ,
    SOURCE_USINE,
    USINE_NOEUD,
    USINE_STOCKAGE,
    TRONCON_DISTRIBUTION
} TypeLigne;

TypeLigne detecter_type_ligne(
    const char* c1,
    const char* c2,
    const char* c3,
    const char* c4,
    const char* c5
);

#endif
