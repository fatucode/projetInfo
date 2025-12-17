#include <stdio.h>
#include "calcul_fuites.h"
 
 double parcourir(Noeud* n, double volume_entree)
 {
 if (!n || volume_entree <= 0.0)
 return 0.0;
 
 // Compter les enfants
 int nb = 0;
 ChildNode* c = n->enfants;
 while (c) {
 nb++;
 c = c->suivant;
 }
 
 if (nb == 0)
 return 0.0; // un usager final ne perd rien par lui-même
 
 double pertes_totales = 0.0;
 double volume_par_enfant = volume_entree / nb;
 
 // Recalcul sur chaque enfant
 c = n->enfants;
 while (c) {
 Noeud* enfant = c->enfant;
 
 double fuite_pct = enfant->fuite;
 if (fuite_pct < 0) fuite_pct = 0;
 if (fuite_pct > 100) fuite_pct = 100;
 
 // Perte sur ce tronçon
 double perte = volume_par_enfant * (fuite_pct / 100.0);
 double volume_sortie = volume_par_enfant - perte;
 
 pertes_totales += perte;
 
 // Propagation récursive
 pertes_totales += parcourir(enfant, volume_sortie);
 
 c = c->suivant;
 }
 
 return pertes_totales;
 }
 
 double calculer_fuites(Noeud* racine)
 {
 if (!racine) return -1.0;
 
 double initial = racine->volume;
 double pertes_km3 = parcourir(racine, initial);
 
 return pertes_km3 / 1000.0; // --> millions de m³
