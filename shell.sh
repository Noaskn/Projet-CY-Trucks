#!/bin/bash

#Nom des arguments
fichier_csv="$1"

#Nom de l'exécutable C
executable="projet"

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

#Traitement qui donne les 10 conducteurs avec le plus de trajets par ordre décroissant avec affichage du temps d'exécution
-d1)
    debut_temps=$(date +%s)
    awk -F';' '!seen[$6,$1]++ {print $6}' $fichier_csv | sort | uniq -c | sort -k1,1nr | head -n 10 > "$temp/traitement_d1.txt"
    fin_temps=$(date +%s)
    duree=$((fin_temps - debut_temps))
    echo "Temps d'exécution du traitement d1 : $duree secondes";;

#Traitement qui donne les 10 conducteurs qui ont la plus longue distance par ordre décroissant avec affichage du temps d'exécution
-d2)
    debut_temps=$(date +%s)
    LC_NUMERIC="C" awk -F';' '{distances[$6]+=$5} END {for (i in distances) printf "%.6f;%s\n", distances[i], i}' $fichier_csv | sort -t';' -k1,1rn | head -n 10 | awk -F';' '{printf "%s;%s\n", $2, $1}' > "$temp/traitement_d2.txt"
    fin_temps=$(date +%s)
    duree=$((fin_temps - debut_temps))
    echo "Temps d'exécution du traitement d2 : $duree secondes";;

#Traitement qui donne les 10 numéros d'identifiant des trajets qui ont la plus longue distance par ordre croissant avec affichage du temps d'exécution
-l)
    debut_temps=$(date +%s)
    LC_NUMERIC="C" awk -F';' '{distances[$1] += $5} END {for (i in distances) printf "%.6f;%d\n", distances[i], i}' $fichier_csv | sort -t';' -k1,1nr | head -n 10 | awk -F';' '{print $2";"$1}' | sort -t';' -k1,1nr > "$temp/traitement_l.txt"
    fin_temps=$(date +%s)
    duree=$((fin_temps - debut_temps))
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
    set datafile separator ";"
    set yrange [0:*]  # La plage de l'axe des y commence à 0 et s'étend jusqu'à la valeur maximale
    set ytics 500     # L'incrément sur l'axe des y est de 500
    set xtics font "Arial,8"  # Taille de la police des marques sur l'axe x
    set ytics font "Arial,8"  # Taille de la police des marques sur l'axe y
    set key off       # Désactiver la légende
    plot "./temp/traitement_l.txt" using 2:xtic(1) with boxes title "Distance"
GPLOT
   ;;
-t)
    ./projet "$fichier_csv" -t "$temp/traitement_t.txt";;

-s)
    ./projet "$fichier_csv" -s "$temp/traitement_s.txt";;

*)
    echo "Le traitement saisi n'existe pas";;
    
esac
