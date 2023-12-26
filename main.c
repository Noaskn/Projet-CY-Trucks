#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AVL{
    int id_trajet;
    char ville[50];
    int *tab;
    int compteur_total;
    int compteur_depart;
    struct AVL* fg;
    struct AVL* fd;
    int hauteur;
}AVL;

// Fonction pour calculer la hauteur d'un nœud AVL
int hauteur(AVL* a){
    if(a == NULL){
        return 0;
    }
    return a->hauteur;
}

// Fonction pour calculer l'équilibre d'un nœud AVL
int equilibre(AVL* a){
    if(a == NULL){
        return 0;
    }
    return hauteur(a->fg) - hauteur(a->fd);
}

//Fonction pour obtenir le maximum de deux entiers
int max(int a, int b){
    return (a > b ? b : a);
}

//Fonction pour créer un nouveau nœud
AVL* creerAVL(int id_trajet, char ville[]){
    AVL* nouveau = (AVL*)malloc(sizeof(AVL));
    if(nouveau!=NULL){
        nouveau->id_trajet=id_trajet;
        strcpy(nouveau->ville, ville);
        nouveau->compteur_total=1;
        nouveau->compteur_depart=1;
        //Allouer de la mémoire pour le tableau d'identifiants de trajets
        nouveau->tab = (int*)malloc(sizeof(int));
        if(nouveau->tab != NULL){
            nouveau->tab[0] = id_trajet;
        }
        else{
            // Gestion de l'erreur d'allocation mémoire
            free(nouveau);
            return NULL;
        }
        nouveau->fg=NULL;
        nouveau->fd=NULL;
        nouveau->hauteur=1;
    }
    return nouveau;
}

// Fonction pour faire une rotation simple à fg
AVL* rotationGauche(AVL *y){
    AVL *x = y->fd;
    AVL *T2 = x->fg;

    x->fg = y;
    y->fd = T2;

    y->hauteur = 1 + max(hauteur(y->fg), hauteur(y->fd));
    x->hauteur = 1 + max(hauteur(x->fg), hauteur(x->fd));

    return x;
}

// Fonction pour faire une rotation simple à fde
AVL* rotationDroite(AVL *x) {
    AVL *y = x->fg;
    AVL *T2 = y->fd;

    y->fd = x;
    x->fg = T2;

    x->hauteur = 1 + max(hauteur(x->fg), hauteur(x->fd));
    y->hauteur = 1 + max(hauteur(y->fg), hauteur(y->fd));

    return y;
}


// Fonction principale pour ajouter une ville à la structure AVL
AVL* ajouterAVL(AVL* a, char ville[],int id_trajet, char type[]){
    if(a == NULL){
        return creerAVL(id_trajet, ville);
    }
    int compare=strcmp(ville, a->ville);
    if(compare==0){
        int i=0;
        while(a->tab[i]!=0){
            if(id_trajet==a->tab[i]){
                return a;
            }
            i++;
        }
        //Ville déjà présente donc on met à jour les informations nécessaires
        a->compteur_total++;
        if(strcmp(type,"depart")==0){
            a->compteur_depart++;
        }
        //Réallouer le tableau avec une taille augmentée
        int* temp = (int*)realloc(a->tab, a->compteur_total * sizeof(int));
        if(temp != NULL){
            a->tab = temp;
            a->tab[a->compteur_total - 1] = id_trajet;
        }
        else{
            // Gestion de l'erreur d'allocation mémoire
            fprintf(stderr, "Erreur lors de la réallocation du tableau d'identifiants de trajets.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if(compare < 0){
        a->fg = ajouterAVL(a->fg, ville, id_trajet, type);
    }
    else{
        a->fd = ajouterAVL(a->fd, ville, id_trajet, type);
        
    }
    a->hauteur = 1 + max(hauteur(a->fg), hauteur(a->fd));
    int eq = equilibre(a);
    // Cas de l'équilibre à gauche-gauche
    if (eq > 1 && strcmp(ville, a->fg->ville) < 0) {
        return rotationDroite(a);
    }

    // Cas de l'équilibre à droite-droite
    if (eq < -1 && strcmp(ville, a->fd->ville) > 0) {
        return rotationGauche(a);
    }

    // Cas de l'équilibre à gauche-droite
    if (eq > 1 && strcmp(ville, a->fg->ville) > 0) {
        a->fg = rotationGauche(a->fg);
        return rotationDroite(a);
    }

    // Cas de l'équilibre à droite-gauche
    if (eq < -1 && strcmp(ville, a->fd->ville) < 0) {
        a->fd = rotationDroite(a->fd);
        return rotationGauche(a);
    }
    return a;
}

void infixe(AVL* a){
    if(a!=NULL){
        infixe(a->fd);
        if(a->compteur_total>1000){
            printf("%s %d %d\n",a->ville,a->compteur_total,a->compteur_depart);
        }
        infixe(a->fg);
    }
}

void libererMemoire(AVL* a){
    if(a!= NULL){
        libererMemoire(a->fg);
        libererMemoire(a->fd);
        free(a);
    }
}

int main(int argc, char *argv[]){
    //Vérification du villebre d'arguments
    if(argc!=3){
        fprintf(stderr, "Usage : %s %s %s\n", argv[0], argv[1], argv[2]);
        return 1;
    }
    //Ouverture du fichier d'entrée
    FILE *fichierEntree = fopen(argv[1], "r");
    if(fichierEntree == NULL){
        perror("Erreur lors de l'ouverture du fichier d'entrée");
        return 1;
    }
    if(strcmp(argv[2], "-t") == 0){
        char ligne[7000000];
        AVL* a=NULL;
        while(fgets(ligne,sizeof(ligne),fichierEntree)){
            char ville_depart[100];
            char ville_arrivee[100];
            int id_trajet;
            int id_etape;
            float distance;
            char nom[100];
            char *token = strtok(ligne, ";");

    if (token != NULL) {
        if (sscanf(token, "%d", &id_trajet) != 1) {
            fprintf(stderr, "Erreur de lecture de id_trajet\n");
            continue; // Passer à la ligne suivante
        }

        token = strtok(NULL, ";");

        if (token != NULL) {
            if (sscanf(token, "%d", &id_etape) != 1) {
                fprintf(stderr, "Erreur de lecture de id_etape\n");
                continue; // Passer à la ligne suivante
            }

            token = strtok(NULL, ";");

            if (token != NULL) {
                strncpy(ville_depart, token, sizeof(ville_depart));
                ville_depart[sizeof(ville_depart) - 1] = '\0';

                token = strtok(NULL, ";");

                if (token != NULL) {
                    strncpy(ville_arrivee, token, sizeof(ville_arrivee));
                    ville_arrivee[sizeof(ville_arrivee) - 1] = '\0';

                    token = strtok(NULL, ";");

                    if (token != NULL) {
                        if (sscanf(token, "%f", &distance) != 1) {
                            fprintf(stderr, "Erreur de lecture de distance\n");
                            continue; // Passer à la ligne suivante
                        }

                        token = strtok(NULL, ";");

                        if (token != NULL) {
                            strncpy(nom, token, sizeof(nom));
                            nom[sizeof(nom) - 1] = '\0';

                            printf("%d %s %s\n", id_trajet, ville_depart, ville_arrivee);
                            a = ajouterAVL(a, ville_depart, id_trajet, "depart");
                            a = ajouterAVL(a, ville_arrivee, id_trajet, "arrivee");
                        }
                    }
                }
            }
        }
    }
        }
        fclose(fichierEntree);
        infixe(a);
    }
    return 0;
}
