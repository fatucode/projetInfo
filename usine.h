#ifndef USINE_H
#define USINE_H

// Structure représentant une usine
typedef struct {
    char id[50];   // Identifiant de l'usine
    double max;    // Capacité maximale
    double src;    // Volume capté depuis les sources
    double real;   // Volume réellement traité (après fuites)
} Usine;

// Crée et initialise une usine
Usine* creer_usine(const char* id);

// Affiche une usine (debug)
void afficher_usine(const Usine* u);

#endif