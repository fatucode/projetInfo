#ifndef USINE_H
#define USINE_H

typedef struct {
    char id[50];      // Id de l'usine
    double max;       // Capacité max
    double src;       // Volume capté
    double real;      // Volume réellement traité
} Usine;


Usine* creer_usine(const char* id, double max, double src, double real);


void afficher_usine(const Usine* u);

#endif

