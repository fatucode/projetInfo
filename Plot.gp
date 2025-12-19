# Terminal PNG haute résolution
set terminal pngcairo size 1800,900 enhanced font "Arial,12"

# On utilise les variables passées par l'option -e du Shell
set output out_file
set title main_title

# Format CSV
set datafile separator ";"
set xlabel "Plant IDs"
set ylabel "Volume (" . unite . ".year-1)"

# Style histogramme
set style data histograms
set style fill solid 0.7 border -1
set boxwidth 0.8
set xtics rotate by -45 right font ",10"
set grid ytics

# Vérifier si le fichier existe et contient des données
stats data_file nooutput
if (STATS_records == 0) {
    print "Erreur : aucune donnée dans " . data_file
} else {
    # On affiche la valeur au-dessus de chaque barre pour plus de clarté
    plot data_file using 2:xtic(1) title main_title lc rgb "#4682B4"
}
