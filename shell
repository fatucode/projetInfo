#!/bin/bash

# --- ARGUMENTS ---
CSV_FILE=$1      # Fichier CSV en entrée
ACTION=$2        # Action : histo ou leaks
PARAM=$3         # Paramètre associé

# --- CHRONO DÉBUT ---
START_TIME=$(date +%s%3N)

# --- FONCTION D'ERREUR ---
error_exit() {
    echo "Erreur : $1"
    END_TIME=$(date +%s%3N)
    echo "Durée totale : $((END_TIME - START_TIME)) ms"
    exit 1
}

# --- VÉRIFICATION DES ARGUMENTS ---
[ $# -ne 3 ] && error_exit "Usage: $0 <file> <histo|leaks> <param>"
[ ! -f "$CSV_FILE" ] && error_exit "Fichier introuvable"

# --- COMPILATION SI NÉCESSAIRE ---
if [ ! -f "./wildwater" ]; then
    make > /dev/null 2>&1 || error_exit "Compilation échouée"
fi

# --- MODE HISTOGRAMME ---
if [ "$ACTION" == "histo" ]; then

    # Vérification du paramètre
    [[ "$PARAM" != "max" && "$PARAM" != "src" && "$PARAM" != "real" ]] \
        && error_exit "Paramètre histo invalide"

    # Exécution du programme C
    ./wildwater "$CSV_FILE" -h "$PARAM" > /dev/null \
        || error_exit "Erreur programme C"

    RAW_DATA="histo_${PARAM}.dat"
    [ ! -f "$RAW_DATA" ] && error_exit "Fichier histogramme absent"

    # Top 10 valeurs
    tail -n +2 "$RAW_DATA" | sort -t';' -k2,2rn | head -n 10 > top10_${PARAM}.temp

    # Bottom 50 valeurs
    tail -n +2 "$RAW_DATA" | sort -t';' -k2,2n | head -n 50 > bottom50_${PARAM}.temp

    # Génération des graphiques
    gnuplot -e "data_file='top10_${PARAM}.temp'; out_file='top10_${PARAM}.png'; main_title='Top 10 ($PARAM)'; unite='k.m3'" histo_config.gp
    gnuplot -e "data_file='bottom50_${PARAM}.temp'; out_file='bottom50_${PARAM}.png'; main_title='Bottom 50 ($PARAM)'; unite='k.m3'" histo_config.gp

    rm -f *.temp
    echo "Histogrammes générés"

# --- MODE FUITES ---
elif [ "$ACTION" == "leaks" ]; then
    ./wildwater "$CSV_FILE" -l "$PARAM" \
        || error_exit "Erreur calcul fuites"

# --- ACTION INCONNUE ---
else
    error_exit "Action inconnue"
fi

# --- CHRONO FIN ---
END_TIME=$(date +%s%3N)
echo "Durée totale : $((END_TIME - START_TIME)) ms"
