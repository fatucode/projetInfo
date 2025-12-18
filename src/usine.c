#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usine.h"

Usine* creer_usine(const char* id) {
    Usine* u = malloc(sizeof(Usine));
    if (u == NULL) {
        return NULL;
    }

    strncpy(u->id, id, sizeof(u->id) - 1);
u->id[sizeof(u->id) - 1] = '\0';

    u->max = 0.0;
    u->src = 0.0;
    u->real = 0.0;

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

