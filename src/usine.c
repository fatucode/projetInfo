#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usine.h"

Usine* creer_usine(const char* id, double max, double src, double real) {
    Usine* u = malloc(sizeof(Usine));
    if (u == NULL) {
        return NULL;
    }

    strncpy(u->id, id, sizeof(u->id) - 1);
u->id[sizeof(u->id) - 1] = '\0';

    u->max = max;
    u->src = src;
    u->real = real;

    return u;
}

// Affiche une usine pour debug
void afficher_usine(const Usine* u) {
    if (u == NULL) {
        return;
    }

    printf("Usine %s | max=%.2f | src=%.2f | real=%.2f\n",
           u->id, u->max, u->src, u->real);
}

