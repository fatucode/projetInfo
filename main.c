#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reseaau.h" 
#include "csv_reader.h"
#include "avl_usine.h"   // Pour la partie Histogramme
#include "avl.h"         // Pour la partie Fuites (Réseau)
     // Pour TypeLigne et detecter_type_ligne
#include "calcul_fuites.h"
#include "histogramme.h"
#include "type_ligne.h"
// --- FONCTION DE RAPPEL (CALLBACK) POUR L'HISTOGRAMME ---
void traiter_ligne(const char* c1, const char* c2, const char* c3, 
                    const char* c4, const char* c5, void* data) 
{
    // On récupère le pointeur vers la racine de l'arbre Usine
    AvlUsine** racine = (AvlUsine**)data;
    
    // Détection du type de ligne (Usine, Source, etc.)
    TypeLigne type = detecter_type_ligne(c1, c2, c3, c4, c5);

    const char* id_usine = NULL;

    if (type == USINE_NOEUD) {
        id_usine = c2;
        Usine* u = chercher_usine(*racine, id_usine);
        if (!u) {
            u = creer_usine(id_usine);
            *racine = inserer_usine(*racine, u);
        }
        // On met à jour la capacité max
        double vol = atof(c4);
        if (vol > u->max) u->max = vol;
    } 
    else if (type == SOURCE_USINE) {
        id_usine = c3;
        Usine* u = chercher_usine(*racine, id_usine);
        if (!u) {
            u = creer_usine(id_usine);
            *racine = inserer_usine(*racine, u);
        }
        // On met à jour le volume capté et le réel (avec fuite)
        double vol = atof(c4);
        double fuite = (strcmp(c5, "-") == 0) ? 0.0 : atof(c5);
        u->src += vol;
        u->real += vol * (1.0 - (fuite / 100.0));
    }
}

// --- MAIN UNIQUE ---
int main(int argc, char* argv[]) {
    // Usage attendu par ton Shell : ./wildwater <data.csv> <-h|-l> <option>
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <fichier.csv> -h <max|src|real> OR -l <id_usine>\n", argv[0]);
        return 1;
    }

    char* filename = argv[1];
    char* mode = argv[2];
    char* param = argv[3];

    if (strcmp(mode, "-h") == 0) {
        // --- MODE HISTOGRAMME ---
        AvlUsine* racine_h = NULL;
        
        printf("Traitement du fichier CSV pour histogramme...\n");
        if (csv_process_file(filename, traiter_ligne, &racine_h) != 0) {
            fprintf(stderr, "Erreur de lecture du fichier CSV.\n");
            return 1;
        }

        int type_val = 0; // 0:max, 1:src, 2:real
        if (strcmp(param, "src") == 0) type_val = 1;
        else if (strcmp(param, "real") == 0) type_val = 2;

        char data_out[50];
        sprintf(data_out, "histo_%s.dat", param);
        
        generer_histogramme(racine_h, data_out, type_val);
        liberer_avl_usine(racine_h);
        printf("Fichier %s genere avec succes.\n", data_out);

    } 
    else if (strcmp(mode, "-l") == 0) {
        // --- MODE FUITES ---
        printf("Chargement du reseau pour calcul des fuites...\n");
        AVLNode* racine_r = charger_reseau(filename);
        
        if (!racine_r) {
            fprintf(stderr, "Erreur de chargement du reseau.\n");
            return 1;
        }

        double pertes = calculer_fuites_usine(racine_r, param);
        if (pertes < 0) {
            printf("Usine '%s' introuvable.\n", param);
        } else {
            printf("Pertes pour %s : %.3f M.m3\n", param, pertes);
            sauvegarder_fuites(param, pertes);
        }

       
        liberer_avl(racine_r);
    } 
    else {
        fprintf(stderr, "Mode inconnu : %s\n", mode);
        return 1;
    }

    return 0;
}
