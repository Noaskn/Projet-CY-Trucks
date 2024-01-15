#!/bin/bash

#Nom des arguments
fichier_csv="$1"

#Nom de l'exécutable C
executable="projet"

#Choix du traitement par l'utilisateur
read -p "Choisir un traitement (-d1,-d2,-l,-t,-s,-h) : " traitement

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
    debut_temps=$(date +%s)
    awk -F';' '!seen[$6,$1]++ {print $6}' $fichier_csv | sort | uniq -c | sort -k1,1nr | head -n 10 > "$temp/traitement_d1.txt"
    fin_temps=$(date +%s)
    duree=$((fin_temps - debut_temps)) #Calcul de la durée du traitement
    echo "Temps d'exécution du traitement d1 : $duree secondes";;

#Trie en shell pour le traitement -d2
-d2)
    debut_temps=$(date +%s)
    LC_NUMERIC="C" awk -F';' '{distances[$6]+=$5} END {for (i in distances) printf "%.6f;%s\n", distances[i], i}' $fichier_csv | sort -t';' -k1,1rn | head -n 10 | awk -F';' '{printf "%s;%s\n", $2, $1}' > "$temp/traitement_d2.txt"
    fin_temps=$(date +%s)
    duree=$((fin_temps - debut_temps)) #Calcul de la durée du traitement
    echo "Temps d'exécution du traitement d2 : $duree secondes"
    # Script Gnuplot pour générer le graphique du traitement -d2
    gnuplot << GPLOT
    set terminal png
    set output "$images/traitement_d2.png"
    set xlabel "DRIVER NAMES"
    set title "Option -d2 : Distance = f(Driver)"
    set style data histograms
    set style fill solid
    set boxwidth 0.5
    set datafile separator ";"
    set yrange [0:*]
    set ytics font "Arial,8"
    set xtics font "Arial,8"
    set key off
    set y2range [0:*]
    set y2tics 20000 nomirror font "Arial,8"
    set y2label "DISTANCE (Km)"
    plot "./temp/traitement_d2.txt" using 2:xticlabels(1) with boxes title "Distance"
GPLOT
    # Rotation de l'image avec convert
    convert "$images/traitement_d2.png" -rotate 90 "$images/traitement_d2_rotated.png"
    ;;

#Trie en shell pour le traitement -l
-l)
    debut_temps=$(date +%s)
    LC_NUMERIC="C" awk -F';' '{distances[$1] += $5} END {for (i in distances) printf "%.6f;%d\n", distances[i], i}' $fichier_csv | sort -t';' -k1,1nr | head -n 10 | awk -F';' '{print $2";"$1}' | sort -t';' -k1,1nr > "$temp/traitement_l.txt"
    fin_temps=$(date +%s)
    duree=$((fin_temps - debut_temps)) #Calcul de la durée du traitement
    echo "Temps d'exécution du traitement l : $duree secondes"
    # Script Gnuplot pour générer le graphique du traitement -l
    gnuplot << GPLOT
    set terminal png
    set output "$images/traitement_l.png"
    set xlabel "ROUTE ID"
    set ylabel "DISTANCE (Km)"
    set title "Option -l : Distance = f(Route)"
    set style data histograms
    set style fill solid
    set boxwidth 0.5
    SkyBlue = "#87CEEB"
    set datafile separator ";"
    set yrange [0:*]  # La plage de l'axe des y commence à 0 et s'étend jusqu'à la valeur maximale
    set ytics 500     # L'incrément sur l'axe des y est de 500
    set xtics font "Arial,8"  # Taille de la police des marques sur l'axe x
    set ytics font "Arial,8"  # Taille de la police des marques sur l'axe y
    set key off       # Désactiver la légende
    plot "./temp/traitement_l.txt" using 2:xtic(1) with boxes lc rgb SkyBlue title "Distance"
GPLOT
    ;;

#Trie en C pour traitement -t
-t)
    ./projet "$fichier_csv" -t "$temp/traitement_t.txt"
    # Script Gnuplot pour générer le graphique du traitement -t
    gnuplot << GPLOT
    set terminal png
    set output "$images/traitement_t.png"
    set xlabel "TOWN NAMES"
    set ylabel "NB ROUTES"
    set title "Option -t : Nb routes = f(Towns)"
    set style data histograms
    set style fill solid border -1
    set boxwidth 0.5
    SkyBlue = "#87CEEB"
    DarkBlue = "#00008B"
    set datafile separator ";"
    set yrange [0:*]
    set ytics font "Arial,8"
    set xtics font "Arial,8"
    set xtics rotate by -45  # Incliner les étiquettes sur l'axe x de 45 degrés
    set key font "Arial,8"  # Changer la taille de la légende
    plot "./temp/traitement_t.txt" using 2:xtic(1) lc rgb SkyBlue title "Total routes", '' using 3 lc rgb DarkBlue title "First Town"
GPLOT
    ;;
    
#Trie en C pour traitement -s
-s)
    ./projet "$fichier_csv" -s "$temp/traitement_s.txt"
    # Script Gnuplot pour générer le graphique du traitement -s
    gnuplot << GPLOT
    set terminal png
    set output "$images/traitement_s.png"
    set xlabel "ROUTE ID"
    set ylabel "DISTANCE (Km)"
    set title "Option -s : Distance = f(Route)"
    set style data lines
    set boxwidth 0.5
    SkyBlue = "#87CEEB"
    DarkBlue = "#00008B"
    set datafile separator ";"
    set xrange [*:*]
    set yrange [0:*]
    set ytics font "Arial,8"
    set xtics font "Arial,6"
    set ytics 100
    set xtics rotate by -45  # Incliner les étiquettes sur l'axe x de 45 degrés
    set key font "Arial,8"  # Changer la taille de la légende
    plot "./temp/traitement_s.txt" using 0:3:4:xticlabels(2) with filledcurves fc rgb SkyBlue title "Distances Max/Min (Km)", \
         '' using 0:5:xticlabels(2) with lines linestyle 5 lc rgb DarkBlue title 'Distance average(Km)'
GPLOT
    ;;

#Par défaut
*)
    echo "Le traitement saisi n'existe pas";;
    
esac
