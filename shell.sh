#!/bin/bash

#Nom des arguments
fichier_csv="$1"
traitement="$2"

#Nom de l'exécutable C
executable="projet"

#Vérification de la présence de l'exécutable
if [ ! -x "$executable" ]; then
   echo "L'exécutable n'est pas présent. Compilation en cours..."
   #Compilation du programme C
   make build
   #Vérification du déroulement de la compilation
   if [ $? -eq 0 ]; then
       echo "Compilation réussie. L'exécutable $executable a été créé."
   else
       echo "Erreur lors de la compilation."
       exit 1
   fi
else
    echo "L'exécutable $executable existe déjà."
fi

#Nom des dossiers
temp="./temp"
images="./images"

#Vérification de l'existence du dossier temp
if [ ! -d "$temp" ]; then
    echo "Le dossier temp n'existe pas, création en cours..."
    mkdir -p "$temp"
#Vidage du dossier temp
else
    echo "Le dossier temporaire existe. Vidage en cours..."
    rm -rf "$temp"/*
fi

#Vérification de l'existence du dossier images
if [ ! -d "$images" ]; then
    echo "Le dossier d'images n'existe pas, création en cours..."
    mkdir -p "$images"
fi

#Analyse des traitements
case $traitement in

#Affichage du message d'aide
-h)
    cat help.txt;;

#Trie en shell pour le traitement -d1
-d1)
    echo "Execution du traitement -d1 en cours..."
    debut_temps=$(date +%s)
    awk -F';' '!seen[$6,$1]++ {print $6}' $fichier_csv | sort | uniq -c | sort -k1,1nr | head -n 10 | awk '{print $2" "$3";"$1}' > "$temp/traitement_d1.txt"
    fin_temps=$(date +%s)
    duree=$((fin_temps - debut_temps)) #Calcul de la durée du traitement
    echo "Temps d'exécution du traitement d1 : $duree secondes"
    #Script Gnuplot pour générer le graphique du traitement -d1
    gnuplot << GPLOT
    set terminal png
    set output "$images/traitement_d1.png"
    set xlabel "DRIVER NAMES" rotate by 180 font "Arial,7" offset 0,-4
    set y2label "NB ROUTES" font "Arial,7" offset -2,0
    set ylabel "Option -d1 : Nb routes = f(Driver)" font "Arial,8" offset 1,0
    set style fill solid
    set boxwidth 0.5 absolute
    SkyBlue = "#87CEEB"
    set xrange [0.5:*]
    set yrange [0:250]
    set y2range [0:250]
    set y2tics 50
    set xtics nomirror rotate by 90 font "Arial,7" offset 0,-5.5
    set y2tics nomirror rotate by 90 font "Arial,7" offset 0,-1
    set origin 0,-0.01
    set format y ""
    set format x ""
    set key off
    set style data impulses
    set datafile separator ";"
    plot "./temp/traitement_d1.txt" using 2:xticlabels(1) with boxes lc rgb SkyBlue notitle axes x1y2
GPLOT
    #Rotation de l'image avec convert
    convert "$images/traitement_d1.png" -rotate 90 "$images/traitement_d1_rotated.png"
    echo "Le graphique du traitement -d1 a été créé."
    ;;

#Trie en shell pour le traitement -d2
-d2)
    echo "Execution du traitement -d2 en cours..."
    debut_temps=$(date +%s)
    LC_NUMERIC="C" awk -F';' '{distances[$6]+=$5} END {for (i in distances) printf "%.6f;%s\n", distances[i], i}' $fichier_csv | sort -t';' -k1,1rn | head -n 10 | awk -F';' '{printf "%s;%s\n", $2, $1}' > "$temp/traitement_d2.txt"
    fin_temps=$(date +%s)
    duree=$((fin_temps - debut_temps)) #Calcul de la durée du traitement
    echo "Temps d'exécution du traitement d2 : $duree secondes"
    #Script Gnuplot pour générer le graphique du traitement -d2
    gnuplot << GPLOT
    set terminal png
    set output "$images/traitement_d2.png"
    set xlabel "DRIVER NAMES" rotate by 180 font "Arial,7" offset 0,-4
    set y2label "DISTANCE (km)" font "Arial,7" offset -2,0
    set ylabel "Option -d2 : Distance = f(Driver)" font "Arial,8" offset 1,0
    set style fill solid
    set boxwidth 0.5 absolute
    SkyBlue = "#87CEEB"
    set xrange [0.5:*]
    set yrange [0:160000]
    set y2range [0:160000]
    set y2tics 20000
    set xtics nomirror rotate by 90 font "Arial,7" offset 0,-5.5
    set y2tics nomirror rotate by 90 font "Arial,7" offset 0,-1
    set origin 0,-0.01
    set format y ""
    set format x ""
    set key off
    set style data impulses
    set datafile separator ";"
    plot "./temp/traitement_d2.txt" using 2:xticlabels(1) with boxes lc rgb SkyBlue title "Distance" axes x1y2
GPLOT
    #Rotation de l'image avec convert
    convert "$images/traitement_d2.png" -rotate 90 "$images/traitement_d2_rotated.png"
    echo "Le graphique du traitement -d2 a été créé."
    ;;

#Trie en shell pour le traitement -l
-l)
    echo "Execution du traitement -l en cours..."
    debut_temps=$(date +%s)
    LC_NUMERIC="C" awk -F';' '{distances[$1] += $5} END {for (i in distances) printf "%.6f;%d\n", distances[i], i}' $fichier_csv | sort -t';' -k1,1nr | head -n 10 | awk -F';' '{print $2";"$1}' | sort -t';' -k1,1nr > "$temp/traitement_l.txt"
    fin_temps=$(date +%s)
    duree=$((fin_temps - debut_temps)) #Calcul de la durée du traitement
    echo "Temps d'exécution du traitement l : $duree secondes"
    #Script Gnuplot pour générer le graphique du traitement -l
    gnuplot << GPLOT
    set terminal png
    set output "$images/traitement_l.png"
    set xlabel "ROUTE ID" font "Arial,7"
    set ylabel "DISTANCE (Km)" font "Arial,7"
    set title "Option -l : Distance = f(Route)" font "Arial,8"
    set style data histograms
    set style fill solid
    set boxwidth 0.5
    SkyBlue = "#87CEEB"
    set datafile separator ";"
    set yrange [0:*]
    set ytics 500
    set xtics font "Arial,7"
    set ytics font "Arial,7"
    set key off
    plot "./temp/traitement_l.txt" using 2:xtic(1) with boxes lc rgb SkyBlue title "Distance"
GPLOT
    echo "Le graphique du traitement -l a été créé."
    ;;

#Trie en C pour traitement -t
-t)
    echo "Execution du traitement -t en cours..."
    ./projet "$fichier_csv" -t "$temp/traitement_t.txt"
    #Script Gnuplot pour générer le graphique du traitement -t
    gnuplot << GPLOT
    set terminal png
    set output "$images/traitement_t.png"
    set xlabel "TOWN NAMES" font "Arial,7"
    set ylabel "NB ROUTES" font "Arial,7"
    set title "Option -t : Nb routes = f(Towns)" font "Arial,8"
    set style data histograms
    set style fill solid border -1
    set boxwidth 0.5
    SkyBlue = "#87CEEB"
    DarkBlue = "#00008B"
    set datafile separator ";"
    set yrange [0:*]
    set ytics font "Arial,7"
    set xtics font "Arial,7"
    set xtics rotate by -45
    set key font "Arial,8"
    plot "./temp/traitement_t.txt" using 2:xtic(1) lc rgb SkyBlue title "Total routes", '' using 3 lc rgb DarkBlue title "First Town"
GPLOT
    echo "Le graphique du traitement -t a été créé."
    ;;
    
#Trie en C pour traitement -s
-s)
    echo "Execution du traitement -s en cours..."
    ./projet "$fichier_csv" -s "$temp/traitement_s.txt"
    #Script Gnuplot pour générer le graphique du traitement -s
    gnuplot << GPLOT
    set terminal png
    set output "$images/traitement_s.png"
    set xlabel "ROUTE ID" font "Arial,7"
    set ylabel "DISTANCE (Km)" font "Arial,7"
    set title "Option -s : Distance = f(Route)" font "Arial,8"
    set style data lines
    set boxwidth 0.5
    SkyBlue = "#87CEEB"
    DarkBlue = "#00008B"
    set datafile separator ";"
    set xrange [*:*]
    set yrange [0:*]
    set ytics font "Arial,7"
    set xtics font "Arial,6"
    set ytics 100
    set xtics rotate by -45
    set key font "Arial,8"
    plot "./temp/traitement_s.txt" using 0:3:4:xticlabels(2) with filledcurves fc rgb SkyBlue title "Distances Max/Min (Km)", \
         '' using 0:5:xticlabels(2) with lines linestyle 5 lc rgb DarkBlue title 'Distance average(Km)'
GPLOT
    echo "Le graphique du traitement -s a été créé."
    ;;

#Par défaut
*)
    echo "Le traitement saisi n'existe pas";;
    
esac
