#ifndef RESEAAU_H
#define RESEAAU_H

// Taille maximale pour les identifiants (id)
#define MAX_ID 100

// Déclarations anticipées (forward declarations)
struct Noeud;  
struct NoeudAVL;

// Alias de type pour NoeudAVL
typedef struct NoeudAVL NoeudAVL;

// Types de lignes détectées dans le fichier CSV
typedef enum {
    SOURCE_USINE,          // Lien entre une source et une usine
    USINE_NOEUD,           // Ligne décrivant une usine
    USINE_STOCKAGE,        // Lien usine -> stockage
    STOCKAGE_JONCTION,     // Lien stockage -> jonction
    JONCTION_USAGER,       // Lien jonction -> usager
    TRONCON_DISTRIBUTION,  // Tronçon de distribution
    INCONNU                // Type non reconnu
} TypeLigne;

// Types de nœuds du réseau
typedef enum {
    NOEUD_SOURCE,          // Source d'eau
    NOEUD_USINE,           // Usine
    NOEUD_STOCKAGE,        // Stockage
    NOEUD_JONCTION,        // Jonction
    NOEUD_RACCORDEMENT,    // Raccordement
    NOEUD_USAGER,          // Usager final
    NOEUD_INCONNU          // Type inconnu
} TypeNoeud;

// Alias de type pour la structure Noeud
typedef struct Noeud Noeud;

// Liste chaînée représentant les enfants d’un nœud
typedef struct ChildNode {
    struct Noeud* enfant;       // Pointeur vers le nœud enfant
    struct ChildNode* suivant;  // Enfant suivant dans la liste
} ChildNode;

// Structure représentant un nœud du réseau
struct Noeud {
    char id[MAX_ID];       // Identifiant du nœud
    TypeNoeud type;        // Type du nœud
    double volume;         // Volume d’eau associé
    double fuite;          // Pourcentage de fuite
    ChildNode* enfants;   // Liste des nœuds enfants
};

// --- Prototypes des fonctions ---

// Crée un nouveau nœud du réseau
Noeud* creer_noeud(const char* id, TypeNoeud type);

// Ajoute un enfant à un nœud parent
void ajouter_enfant(Noeud* parent, Noeud* enfant);

// Charge le réseau depuis un fichier CSV et retourne la racine de l’AVL
NoeudAVL* charger_reseau(const char* filename);

// Détecte le type de ligne du CSV à partir des colonnes
TypeLigne detecter_type_ligne(const char* c1, const char* c2, const char* c3,
                              const char* c4, const char* c5);

#endif
