set terminal pngcairo size 1800,900
set output ARG2

set datafile separator ";"
set title "Source volume per plant"
set xlabel "Plant IDs"
set ylabel "Volume (M.m3)"

set style data histograms
set style fill solid 1.0 border -1
set boxwidth 0.9

set xtics rotate by -90 font ",8"
set grid ytics

plot ARG1 using 2:xtic(1) title "Source volume"
