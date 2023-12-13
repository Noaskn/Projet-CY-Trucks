#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    //Vérification du nombre d'arguments
    if(argc!=4){
        fprintf(stderr, "Usage : %s %s %s %s\n", argv[0], argv[1], argv[2], argv[3]);
        return 1;
    }
    //Ouverture du fichier d'entrée
    FILE *fichierEntree = fopen(argv[1], "r");
    if(fichierEntree == NULL){
        perror("Erreur lors de l'ouverture du fichier d'entrée");
        return 1;
    }

    char *ligne;
    int id_trajet;
    int id_etape;
    char *ville_depart;
    char *ville_arrivee;
    float distance;
    char *nom;


    while(fgets(ligne,sizeof(ligne),fichierEntree) != NULL){
        sscanf(ligne, "%d[^;];%d[^;];%s[^;];%s[^;];%f[^;];%s[^\n]", &id_trajet, &id_etape, ville_depart, ville_arrivee, &distance, nom);
        //Traitement des données en fonction du type
        if(strcmp(argv[3], "-t") == 0){
            //Faut envoyer dans les fichiers de tri avec ville_depart ville_arrivee nom
        }
        else if(strcmp(argv[3], "-s") == 0){
            //Faut envoyer dans les fichiers de tri avec distance et id_trajet
        }
        else{
            fprintf(stderr, "Type de traitement non reconnu : %s\n", argv[3]);
            fclose(fichierEntree);
            return 1;
        }
    }

    //Création du fichier de sortie
    FILE *fichierSortie = fopen(argv[2], "w");
    if(fichierSortie == NULL){
        perror("Erreur lors de l'ouverture du fichier de sortie");
        fclose(fichierEntree);
        return 1;
    }
    //Fermeture des fichiers
    fclose(fichierEntree);
    fclose(fichierEntree);
    return 0;
}