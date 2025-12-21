#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reseaau.h" 
#include "csv_reader.h"
#include "avl_usine.h"   
#include "avl.h"         
#include "calcul_fuites.h"
#include "histogramme.h"

// --- FONCTION DE RAPPEL (CALLBACK) POUR L'HISTOGRAMME ---
// Cette fonction est appelée pour chaque ligne du fichier CSV
void traiter_ligne(const char* c1, const char* c2, const char* c3, 
                   const char* c4, const char* c5, void* data) 
{
    // Récupération de la racine de l'AVL des usines
    AvlUsine** racine = (AvlUsine**)data;
    
    // Détection du type de ligne (usine, source → usine, etc.)
    TypeLigne type = detecter_type_ligne(c1, c2, c3, c4, c5);
    const char* id_usine = NULL;

    // Cas d'une ligne décrivant une usine
    if (type == USINE_NOEUD) {
        id_usine = c2;

        // Recherche de l'usine dans l'AVL
        Usine* u = chercher_usine(*racine, id_usine);

        // Si l'usine n'existe pas encore, on la crée
        if (!u) {
            u = creer_usine(id_usine);
            *racine = inserer_usine(*racine, u);
        }

        // Mise à jour de la capacité maximale de l'usine
        double vol = atof(c4);
        if (vol > u->max) u->max = vol;
    } 
    // Cas d'une ligne source → usine
    else if (type == SOURCE_USINE) {
        id_usine = c3;

        // Recherche ou création de l'usine
        Usine* u = chercher_usine(*racine, id_usine);
        if (!u) {
            u = creer_usine(id_usine);
            *racine = inserer_usine(*racine, u);
        }

        // Mise à jour des volumes source et réel (avec prise en compte des fuites)
        double vol = atof(c4);
        double fuite = (strcmp(c5, "-") == 0) ? 0.0 : atof(c5);
        u->src += vol;
        u->real += vol * (1.0 - (fuite / 100.0));
    }
}

// --- MAIN ---
int main(int argc, char* argv[])
{
    // Usage attendu :
    // ./wildwater <fichier.csv> -h <max|src|real>
    // ./wildwater <fichier.csv> -l <id_usine>
    if (argc < 4) {
        fprintf(stderr,
                "Usage: %s <fichier.csv> -h <max|src|real> OR -l <id_usine>\n",
                argv[0]);
        return 1;
    }

    char* filename = argv[1]; // Fichier CSV
    char* mode = argv[2];     // Mode (-h ou -l)
    char* param = argv[3];    // Paramètre (type d'histo ou id usine)

    // --- MODE HISTOGRAMME ---
    if (strcmp(mode, "-h") == 0) {
        AvlUsine* racine_h = NULL;

        printf("Traitement du fichier CSV pour histogramme...\n");

        // Lecture du CSV avec callback
        if (csv_process_file(filename, traiter_ligne, &racine_h) != 0) {
            fprintf(stderr, "Erreur de lecture du fichier CSV.\n");
            return 1;
        }

        // Sélection du type de valeur à afficher
        int type_val = 0; // 0 = max, 1 = src, 2 = real
        if (strcmp(param, "src") == 0) type_val = 1;
        else if (strcmp(param, "real") == 0) type_val = 2;

        // Nom du fichier de sortie
        char data_out[50];
        sprintf(data_out, "histo_%s.dat", param);

        // Génération du fichier pour gnuplot
        generer_histogramme(racine_h, data_out, type_val);

        // Libération de la mémoire
        liberer_avl_usine(racine_h);

        printf("Fichier %s genere avec succes.\n", data_out);
    }
    // --- MODE CALCUL DES FUITES ---
    else if (strcmp(mode, "-l") == 0) {
        printf("Chargement du reseau pour calcul des fuites...\n");

        // Construction du réseau à partir du CSV
        NoeudAVL* racine_r = charger_reseau(filename);
        if (!racine_r) {
            fprintf(stderr, "Erreur de chargement du reseau.\n");
            return 1;
        }

        // Calcul des fuites pour l'usine demandée
        double pertes = calculer_fuites_usine(racine_r, param);
        if (pertes < 0) {
            printf("Usine '%s' introuvable.\n", param);
        } else {
            printf("Pertes pour %s : %.3f M.m3\n", param, pertes);
            sauvegarder_fuites(param, pertes);
        }

        // Libération de la mémoire
        liberer_avl(racine_r);
    }
    // --- MODE INVALIDE ---
    else {
        fprintf(stderr, "Mode inconnu : %s\n", mode);
        return 1;
    }

    return 0;
}
