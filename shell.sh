#!/bin/bash

#Nom des arguments
fichier_csv="$1"
traitement="$2"

#Nom de l'exécutable C
executable="projet"

#Vérification de la présence de l'exécutable
if [ ! -x "$executable" ]
then
   echo "L'exécutable n'est pas présent. Compilation en cours..."
   #Compilation du programme C
   gcc -o "$executable" projet.c
   #Vérification du déroulement de la compilation
   if [ $? -eq 0 ]
   then
       echo "Compilation réussie. L'exécutable $executable a été créé."
   else
       echo "Erreur lors de la compilation."
       exit 1
   fi
fi

#Nom des dossiers
temp="./temp"
images="./images"

#Vérification de l'existence du dossier temp
if [ ! -d "$temp" ]
then
    echo "Le dossier temp n'existe pas, création en cours..."
    mkdir -p "$temp"
#Vidage du dossier temp
else
    echo "Le dossier temporaire existe. Vidage en cours..."
    rm -rf "$temp"/*
fi

#Vérification de l'existence du dossier images
if [ ! -d "$images" ]
then
    echo "Le dossier d'images n'existe pas, création en cours..."
    mkdir -p "$images"
fi

#Analyse des traitements
case $traitement in
#Affichage du message d'aide
-h)
    cat help.txt
    ;;
#Traitement qui donne les 10 conducteurs avec le plus de trajets par ordre décroissant
-d1)
   awk -F';' '{print $6}' $fichier_csv | sort | uniq -c | sort -rn | head -n 10 > top10_conducteurs_trajets.csv
   ;;
#Traitement qui donne les 10 conducteurs qui ont la plus longue distance par ordre décroissant
-d2)
   awk -F';' '{distances[$6]+=$5} END {for (i in distances) print distances[i], i}' $fichier_csv | sort -rn | head -n 10 > top10_conducteurs_distances.csv
   ;;
-l)
#Traitement qui donne les 10 numéros d'identifiant des trajets qui ont la plus longue distance par ordre croissant
   awk -F';' '{distances[$1]+=$5} END {for (i in distances) print distances[i], i}' $fichier_csv | sort -k1,1nr | head -n 10 | sort -k2,2n > top10_trajets_distances.csv
   ;;
-t)
    ;;
-s)
    ;;
*)
    ;;
esac
