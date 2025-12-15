# Usage:
# gnuplot -c plot_top10.gp input.dat output.png

set terminal pngcairo size 1800,900
set output ARG2

set datafile separator ";"
set title "Plant data (10 greatest)"
set xlabel "Plant IDs"
set ylabel "Volume (M.m3)"

set style data histograms
set style histogram rowstacked
set style fill solid 1.0 border -1
set boxwidth 0.9

set xtics rotate by -45 font ",10"
set grid ytics

plot ARG1 using 4:xtic(1) title "Real volume", \
     '' using ($3-$4) title "Lost volume", \
     '' using ($2-$3) title "Unused capacity"
