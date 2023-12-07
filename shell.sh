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


