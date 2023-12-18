#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Ville{
    char nom[50];
    int trajets;
    int conducteurs;
    struct Ville* gauche;
    struct Ville* droite;
    int hauteur;
}Ville;

// Fonction pour obtenir la hauteur d'un nœud
int hauteur(Ville* n){
    if (n == NULL){
        return 0;
    }
    return n->hauteur;
}

// Fonction pour obtenir le maximum de deux entiers
int max(int a, int b){
    return (a > b) ? a : b;
}

// Fonction pour créer un nouveau nœud Ville
Ville* creerVille(char nom[50]){
    Ville* ville = (Ville*)malloc(sizeof(Ville));
    strcpy(ville->nom, nom);
    ville->trajets = 1; // Nouvelle ville, donc 1 trajet
    ville->conducteurs = 1; // Nouveau conducteur, donc 1 conducteur
    ville->gauche = NULL;
    ville->droite = NULL;
    ville->hauteur = 1; // Nouveau nœud a une hauteur initiale de 1
    return ville;
}

// Fonction pour effectuer une rotation simple à droite
Ville* rotationDroite(Ville* y){
    Ville* x = y->gauche;
    Ville* T2 = x->droite;
    // Effectuer la rotation
    x->droite = y;
    y->gauche = T2;
    // Mettre à jour les hauteurs
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;
    return x;
}

// Fonction pour effectuer une rotation simple à gauche
Ville* rotationGauche(Ville* x){
    Ville* y = x->droite;
    Ville* T2 = y->gauche;
    // Effectuer la rotation
    y->gauche = x;
    x->droite = T2;
    // Mettre à jour les hauteurs
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;
    return y;
}

// Fonction pour obtenir le facteur d'équilibre d'un nœud
int facteurEquilibre(Ville* n){
    if(n == NULL){
        return 0;
    }
    return hauteur(n->gauche) - hauteur(n->droite);
}

// Fonction principale pour ajouter une ville à la structure AVL
Ville* ajouterVille(Ville* racine, char nom[50]){
    // Effectuer l'insertion normale de l'arbre binaire de recherche
    if(racine == NULL){
        return creerVille(nom);
    }
    if(strcmp(nom, racine->nom) < 0){
        racine->gauche = ajouterVille(racine->gauche, nom);
    }
    else if(strcmp(nom, racine->nom) > 0){
        racine->droite = ajouterVille(racine->droite, nom);
    }
    else{
        // La ville existe déjà, mise à jour des compteurs
        racine->trajets++;
        racine->conducteurs++;
        return racine;
    }
    // Mettre à jour la hauteur du nœud actuel
    racine->hauteur = 1 + max(hauteur(racine->gauche), hauteur(racine->droite));
    // Obtenir le facteur d'équilibre du nœud actuel
    int equilibre = facteurEquilibre(racine);
    // Effectuer les rotations nécessaires pour rééquilibrer l'arbre
    // Rotation à droite simple
    if(equilibre > 1 && strcmp(nom, racine->gauche->nom) < 0){
        return rotationDroite(racine);
    }
    // Rotation à gauche simple
    if(equilibre < -1 && strcmp(nom, racine->droite->nom) > 0){
        return rotationGauche(racine);
    }
    // Rotation à gauche-droite (double rotation)
    if(equilibre > 1 && strcmp(nom, racine->gauche->nom) > 0){
        racine->gauche = rotationGauche(racine->gauche);
        return rotationDroite(racine);
    }
    // Rotation à droite-gauche (double rotation)
    if(equilibre < -1 && strcmp(nom, racine->droite->nom) < 0){
        racine->droite = rotationDroite(racine->droite);
        return rotationGauche(racine);
    }
    // Aucune rotation nécessaire
    return racine;
}

void libererMemoire(Ville* racine){
    if(racine != NULL){
        libererMemoire(racine->gauche);
        libererMemoire(racine->droite);
        free(racine);
    }
}

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
