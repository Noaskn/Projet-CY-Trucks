#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure AVL
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

//Fonction pour calculer la hauteur d'un nœud AVL
int hauteur(AVL* a){
    if(a == NULL){
        return 0;
    }
    return a->hauteur;
}

//Fonction pour calculer l'équilibre d'un nœud AVL
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
        nouveau->compteur_total=0;
        nouveau->compteur_depart=0;
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

//Fonction pour faire une rotation simple à fg
AVL* rotationGauche(AVL *y){
    AVL *x = y->fd;
    AVL *T2 = x->fg;

    x->fg = y;
    y->fd = T2;

    y->hauteur = 1 + max(hauteur(y->fg), hauteur(y->fd));
    x->hauteur = 1 + max(hauteur(x->fg), hauteur(x->fd));

    return x;
}

//Fonction pour faire une rotation simple à fd
AVL* rotationDroite(AVL *x) {
    AVL *y = x->fg;
    AVL *T2 = y->fd;

    y->fd = x;
    x->fg = T2;

    x->hauteur = 1 + max(hauteur(x->fg), hauteur(x->fd));
    y->hauteur = 1 + max(hauteur(y->fg), hauteur(y->fd));

    return y;
}

//Fonction pour ajouter une ville à la structure AVL
AVL* ajouterAVL(AVL* a, char ville[],int id_trajet){
    if(a == NULL){
        return creerAVL(id_trajet, ville);
    }
    int compare=strcmp(ville, a->ville);
    if(compare==0){
        int i=0;
        while(a->tab[i]!=0){
            //Ville et identifiant déjà présents donc on met à jour le compteur de ville de départ
            if(id_trajet==a->tab[i]){
                a->compteur_depart++;
                return a;
            }
            i++;
        }
        //Ville déjà présente donc on met à jour le compteur total et le tableau comptenant les identifiants
        a->compteur_total++;
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
        a->fg = ajouterAVL(a->fg, ville, id_trajet);
    }
    else{
        a->fd = ajouterAVL(a->fd, ville, id_trajet);
        
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

//Fonction pour créer un nouveau nœud
AVL* creerAVLtrier(char ville[],int compteur_total,int compteur_depart){
    AVL* nouveau = (AVL*)malloc(sizeof(AVL));
    if(nouveau!=NULL){
        strcpy(nouveau->ville, ville);
        nouveau->compteur_total=compteur_total;
        nouveau->compteur_depart=compteur_depart;
        nouveau->fg=NULL;
        nouveau->fd=NULL;
        nouveau->hauteur=1;
    }
    return nouveau;
}

//Fonction pour ajouter un nouveau compteur
AVL* ajouterAVLTrieParCompteurTotal(AVL* a, char ville[], int compteur_total, int compteur_depart){
    if (a == NULL) {
        return creerAVLtrier(ville,compteur_total,compteur_depart);
    }
    if(a->compteur_total>compteur_total){
        a->fg = ajouterAVLTrieParCompteurTotal(a->fg,ville,compteur_total,compteur_depart);
    }
    else if(a->compteur_total<compteur_total){
        a->fd = ajouterAVLTrieParCompteurTotal(a->fd,ville,compteur_total,compteur_depart);
    }
    else{
        if(strcmp(ville,a->ville)<0){
            a->fg = ajouterAVLTrieParCompteurTotal(a->fg,ville,compteur_total,compteur_depart);
        }
        else if(strcmp(ville,a->ville)>0){
            a->fd = ajouterAVLTrieParCompteurTotal(a->fd,ville,compteur_total,compteur_depart);
        }
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

//Fonction pour copier les données du premier AVL dans un second et faire un nouveau tri
AVL* copierEtTrierParCompteurTotal(AVL* ancienAVL, AVL* nouveauAVL){
    if(ancienAVL != NULL){
        nouveauAVL = ajouterAVLTrieParCompteurTotal(nouveauAVL, ancienAVL->ville, ancienAVL->compteur_total, ancienAVL->compteur_depart);
        nouveauAVL = copierEtTrierParCompteurTotal(ancienAVL->fg, nouveauAVL);
        nouveauAVL = copierEtTrierParCompteurTotal(ancienAVL->fd, nouveauAVL);
    }
    return nouveauAVL;
}

//Fonction pour comparer deux chaînes de caractères (utilisée pour le tri alphabétique)
int comparerVilles(const void* a, const void* b) {
    AVL* villeA = *((AVL**)a);
    AVL* villeB = *((AVL**)b);
    return strcmp(villeA->ville, villeB->ville);
}

// Fonction qui permet de stocker les 10 villes qui ont le plus de trajets (triées par ordre alphabétique)
void parcoursLimite(AVL* a, char* mode, FILE* fichierSortie, int* compteur, AVL** tableau) {
    if (a != NULL && *compteur < 10) {
        parcoursLimite(a->fd, mode, fichierSortie, compteur, tableau);
        
        if (*compteur >= 10) {
            return;
        }

        // Stocker la ville dans le tableau
        tableau[*compteur] = a;
        (*compteur)++;

        parcoursLimite(a->fg, mode, fichierSortie, compteur, tableau);
    }
}

// Fonction qui crée un compteur avant le stockage des données
void parcours(AVL* a, char* mode, FILE* fichierSortie) {
    int compteur = 0;  // Initialiser le compteur
    AVL* tableau[10];  // Tableau pour stocker les 10 villes
    parcoursLimite(a, mode, fichierSortie, &compteur, tableau);

    // Trier le tableau par ordre alphabétique
    qsort(tableau, compteur, sizeof(AVL*), comparerVilles);

    // Écrire les données triées dans le fichier de sortie
    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "%s;%d;%d\n", tableau[i]->ville, tableau[i]->compteur_total, tableau[i]->compteur_depart);
    }
}

//Fonction qui libère la mémoire de l'AVL
void libererMemoire(AVL* a){
    if(a!= NULL){
        libererMemoire(a->fg);
        libererMemoire(a->fd);
        free(a);
    }
}

int main(int argc, char *argv[]){
    //Vérification du villebre d'arguments
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
    FILE* fichierSortie = NULL;
    //Ouverture du fichier de sortie
    fichierSortie = fopen(argv[3], "w");
    //Traitement -t
    if(strcmp(argv[2], "-t") == 0){
        char ligne[7000000];
        AVL* a=NULL;
        //Parcours du fichier d'entrée
        while(fgets(ligne,sizeof(ligne),fichierEntree)){
            char ville_depart[100];
            char ville_arrivee[100];
            int id_trajet;
            int id_etape;
            float distance;
            char nom[100];
            char *token = strtok(ligne, ";");
            //Récupération des données nécessaires au traitement
            if(token != NULL){
                if(sscanf(token, "%d", &id_trajet) != 1){
                    continue; // Passer à la ligne suivante
                }
                token = strtok(NULL, ";");
                if(token != NULL){
                    if(sscanf(token, "%d", &id_etape) != 1){
                        continue; // Passer à la ligne suivante
                    }
                    token = strtok(NULL, ";");
                    if(token != NULL){
                        strncpy(ville_depart, token, sizeof(ville_depart));
                        ville_depart[sizeof(ville_depart) - 1] = '\0';
                        token = strtok(NULL, ";");
                        if (token != NULL) {
                            strncpy(ville_arrivee, token, sizeof(ville_arrivee));
                            ville_arrivee[sizeof(ville_arrivee) - 1] = '\0';
                            //Affichage de ce qui a été récupéré (FAUDRA L'ENLEVER)
                            printf("%d %s %s\n", id_trajet, ville_depart, ville_arrivee);
                            //Ajout dans l'AVL de la ville de départ
                            a = ajouterAVL(a, ville_depart, id_trajet);
                            //Ajout dans l'AVL de la ville d'arrivée
                            a = ajouterAVL(a, ville_arrivee, id_trajet);
                        }
                    }
                }
            }
        }
        //Copie de l'AVL et nouveau trie
        AVL* nouvelAVL = copierEtTrierParCompteurTotal(a, NULL);
        //Stockage des données nécessaires dans le fichier de sortie
        parcours(nouvelAVL,argv[3],fichierSortie);
        //Libération de la mémoire de l'AVL
        libererMemoire(a);
    }
    //Fermeture des fichiers d'entrée et de sortie
    fclose(fichierEntree);
    fclose(fichierSortie);
    return 0;
}
