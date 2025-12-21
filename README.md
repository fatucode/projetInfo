# projetInfo
1. Présentation
Le projet Wild Water est un outil d'analyse de données hydrauliques massives. Il permet d'analyser l'infrastructure d'un réseau de distribution d'eau à travers des fichiers CSV (jusqu'à 500 Mo) pour générer des statistiques visuelles et des calculs de fuites.

2. Installation et Compilation
L'application nécessite GCC et Gnuplot. Commande de compilation : make Cette commande compile les fichiers C et génère l'exécutable wildwater.

3. Guide d'utilisation
L'outil s'utilise via le script wildwater.sh avec deux modes principaux :

A. Mode Analyse Statistique (histo)
Ce mode nécessite un argument supplémentaire pour choisir le type de données à traiter :

max : Pour les capacités maximales.

real : Pour les volumes réellement traités.

src : Pour les débits aux sources.

Commande : ./wildwater.sh [fichier.csv] histo [max|real|src] Le programme génère alors les fichiers de données et les graphiques PNG correspondants.

B. Mode Calcul des Pertes (leaks)
Ce mode calcule les fuites d'eau en millions de mètres cubes (M.m³) pour une unité précise. Commande : ./wildwater.sh [fichier.csv] leaks "Nom de l'unité" Note : Utilisez des guillemets pour les noms d'unités contenant des espaces.

4. Architecture et Algorithmique
Le cœur du programme repose sur un Arbre AVL (Arbre binaire de recherche équilibré) développé en C.

Efficacité : L'AVL permet de maintenir une hauteur d'arbre minimale, garantissant des recherches rapides même sur des millions de lignes.

Flux de données : Le script Shell effectue un premier tri, le programme C traite les données dans l'arbre, et Gnuplot assure la mise en image finale.

5. Composition de l'équipe
Sarah : Développement C - Modules Histogrammes et logique AVL.

Fatumata : Développement C - Algorithmes de rendement et calcul des fuites.

Maria : Script Shell - Automatisation, filtrage et interface Gnuplot.
