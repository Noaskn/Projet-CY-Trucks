#include "fonctionsAVL.h"
#include "fonctionsT.h"
#include "fonctionsS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Fonction principale
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
    
    //Création du fichier de sortie
    FILE* fichierSortie = NULL;
    //Ouverture du fichier de sortie
    fichierSortie = fopen(argv[3], "w");
    
    //Traitement -t
    if(strcmp(argv[2], "-t") == 0){
        char ligne[7000000];
        AVL* a=NULL;
        int h = 0;
        printf("Récupération des données du fichier d'entrée en cours...\n");
        
        //Parcours du fichier d'entrée
        while(fgets(ligne,sizeof(ligne),fichierEntree)!=NULL){
            char ville_depart[100];
            char ville_arrivee[100];
            int id_trajet;
            int id_etape;
            char *token = strtok(ligne, ";");
            
            //Récupération des données nécessaires au traitement -t
            if(token != NULL){
                if(sscanf(token, "%d", &id_trajet) != 1){
                    continue;
                }
                token = strtok(NULL, ";");
                if(token != NULL){
                    if(sscanf(token, "%d", &id_etape) != 1){
                        continue;
                    }
                    token = strtok(NULL, ";");
                    if(token != NULL){
                        strncpy(ville_depart, token, sizeof(ville_depart));
                        ville_depart[sizeof(ville_depart) - 1] = '\0';
                        token = strtok(NULL, ";");
                        if (token != NULL) {
                            strncpy(ville_arrivee, token, sizeof(ville_arrivee));
                            ville_arrivee[sizeof(ville_arrivee) - 1] = '\0';
                            //Ajout dans l'AVL de la ville de départ
                            a = ajouterAVL_t(a, ville_depart, id_trajet, id_etape, "depart", &h);
                            //Ajout dans l'AVL de la ville d'arrivée
                            a = ajouterAVL_t(a, ville_arrivee, id_trajet, id_etape, "arrivee", &h);
                        }
                    }
                }
            }
        }
        printf("Tri en cours...\n");
        h = 0;
        AVL* nouvelAVL = NULL;
        //Copie de l'AVL et nouveau trie
        copier_trier_t(a, &nouvelAVL, &h);
        //Stockage des données nécessaires dans le fichier de sortie
        tri_alphabetique(nouvelAVL,argv[3],fichierSortie);
        //Libération de la mémoire de l'AVL
        libererMemoire(a);
        //Fermeture des fichiers d'entrée et de sortie
        fclose(fichierEntree);
        fclose(fichierSortie);
    }
    
    //Traitement -s
    if(strcmp(argv[2], "-s") == 0){
        char ligne[7000000];
        AVL* a=NULL;
        int compteur=0;
        int h = 0;
        printf("Récupération des données du fichier d'entrée en cours...\n");
        
        //Parcours du fichier d'entrée
        while(fgets(ligne,sizeof(ligne),fichierEntree)!=NULL){
            int id_trajet;
            int id_etape;
            char ville_depart[100];
            char ville_arrivee[100];
            float distance;
            char *token = strtok(ligne, ";");
            
            //Récupération des données nécessaires au traitement -s
            if(token != NULL) {
                if(sscanf(token, "%d", &id_trajet) != 1){
                    continue;
                }
                token = strtok(NULL, ";");
                if(token != NULL){
                    if(sscanf(token, "%d", &id_etape) != 1){
                        continue;
                    }
                    token = strtok(NULL, ";");
                    if(token != NULL){
                        strncpy(ville_depart, token, sizeof(ville_depart));
                        ville_depart[sizeof(ville_depart) - 1] = '\0';
                        token = strtok(NULL, ";");
                        if(token != NULL){
                            strncpy(ville_arrivee, token, sizeof(ville_arrivee));
                            ville_arrivee[sizeof(ville_arrivee) - 1] = '\0';
                            token = strtok(NULL, ";");
                            if(token != NULL){
                                if(sscanf(token, "%f", &distance) != 1){
                                    continue;
                                }
                                //Ajout dans l'AVL
                                a = ajouterAVL_s(a, distance, id_trajet, &h);
                            }
                        }
                    }
                }
            }
        }
        printf("Tri en cours...\n");
        h = 0;
        AVL* nouvelAVL = NULL;
        //Copie de l'AVL et nouveau trie
        copier_trier_s(a, &nouvelAVL, &h);
        //Stockage des données nécessaires dans le fichier de sortie
        stockageDonnees_s(nouvelAVL,argv[3],fichierSortie,&compteur);
        libererMemoire(a);
        //Fermeture des fichiers d'entrée et de sortie
        fclose(fichierEntree);
        fclose(fichierSortie);
    }
    return 0;
}
