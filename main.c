#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure AVL
typedef struct AVL{
    int id_trajet;
    int id_etape;
    float distance;
    char ville[50];
    int *tab_id;
    float *tab_distance;
    int compteur_total;
    int compteur_depart;
    int compteur_distance;
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
AVL* creerAVL(int id_trajet, int id_etape, char ville[], char type[]){
    AVL* nouveau = (AVL*)malloc(sizeof(AVL));
    if(nouveau!=NULL){
        nouveau->id_trajet=id_trajet;
        nouveau->id_etape=id_etape;
        strcpy(nouveau->ville, ville);
        nouveau->compteur_total=0;
        nouveau->compteur_depart=0;
        //Si c'est la première étape d'un trajet on augmente le compteur
        if(strcmp(type,"depart")==0 && id_etape==1){
            nouveau->compteur_depart++;
        }
        //Allouer de la mémoire pour le tableau d'identifiants de trajets
        nouveau->tab_id = (int*)malloc(sizeof(int));
        if(nouveau->tab_id != NULL){
            nouveau->tab_id[0] = id_trajet;
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

AVL* creerAVL2(int id_trajet, float distance){
    AVL* nouveau = (AVL*)malloc(sizeof(AVL));
    if(nouveau!=NULL){
        nouveau->id_trajet=id_trajet;
        nouveau->distance=distance;
        nouveau->compteur_distance=1;
        //Allouer de la mémoire pour le tableau des distances
        nouveau->tab_distance = (float*)malloc(sizeof(float));
        if(nouveau->tab_distance != NULL){
            nouveau->tab_distance[0] = distance;
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
AVL* ajouterAVL(AVL* a, char ville[],int id_trajet, int id_etape, char type[]){
    if(a == NULL){
        return creerAVL(id_trajet, id_etape, ville, type);
    }
    int compare=strcmp(ville, a->ville);
    if(compare==0){
        int i=0;
        while(a->tab_id[i]!=0){
            //Ville et identifiant déjà présents
            if(id_trajet==a->tab_id[i]){
                 //Si c'est la première étape d'un trajet on augmente le compteur
                if(strcmp(type,"depart")==0 && id_etape==1){
                    a->compteur_depart++;
                }
                return a;
            }
            i++;
        }
        //Ville déjà présente donc on met à jour le compteur total et le tableau comptenant les identifiants
        a->compteur_total++;
         //Si c'est la première étape d'un trajet on augmente le compteur
        if(strcmp(type,"depart")==0 && id_etape==1){
            a->compteur_depart++;
        }
        //Réallouer le tableau avec une taille augmentée
        int* temp = (int*)realloc(a->tab_id, a->compteur_total * sizeof(int));
        if(temp != NULL){
            a->tab_id = temp;
            a->tab_id[a->compteur_total - 1] = id_trajet;
        }
        else{
            // Gestion de l'erreur d'allocation mémoire
            fprintf(stderr, "Erreur lors de la réallocation du tableau d'identifiants de trajets.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if(compare < 0){
        a->fg = ajouterAVL(a->fg, ville, id_trajet, id_etape, type);
    }
    else{
        a->fd = ajouterAVL(a->fd, ville, id_trajet, id_etape, type);
        
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

AVL* ajouterAVL2(AVL* a, float distance,int id_trajet){
    if(a == NULL){
        return creerAVL2(id_trajet, distance);
    }
    if(a->id_trajet == id_trajet){
        a->tab_distance = (float*)realloc(a->tab_distance, sizeof(float)*(a->compteur_distance+1));
        if(a->tab_distance == NULL){
        fprintf(stderr, "Erreur d'allocation de mémoire.\n");
        exit(EXIT_FAILURE);
        }
        // Ajouter la nouvelle distance
        a->tab_distance[a->compteur_distance++] = distance;
    }
    else if(id_trajet<a->id_trajet){
        a->fg = ajouterAVL2(a->fg,distance,id_trajet);
    }
    else{
        a->fd = ajouterAVL2(a->fd,distance,id_trajet);
    }
    a->hauteur = 1 + max(hauteur(a->fg), hauteur(a->fd));
    int eq = equilibre(a);
    // Cas de l'équilibre à gauche-gauche
    if (eq > 1 && id_trajet < a->fg->id_trajet) {
        return rotationDroite(a);
    }

    // Cas de l'équilibre à droite-droite
    if (eq < -1 && id_trajet > a->fd->id_trajet) {
        return rotationGauche(a);
    }

    // Cas de l'équilibre à gauche-droite
    if (eq > 1 && id_trajet > a->fg->id_trajet) {
        a->fg = rotationGauche(a->fg);
        return rotationDroite(a);
    }

    // Cas de l'équilibre à droite-gauche
    if (eq < -1 && id_trajet < a->fd->id_trajet) {
        a->fd = rotationDroite(a->fd);
        return rotationGauche(a);
    }
    return a;
}

float moyenne(AVL* a){
    float res=0;
    for(int i=0;i<a->compteur_distance;i++){
        res+=a->tab_distance[i];
    }
    return res/a->compteur_distance;
}

float trouverMin(AVL* a){
    if(a->compteur_distance <= 0 || a->tab_distance == NULL){
        fprintf(stderr, "Tableau invalide.\n");
        return -1;  // Valeur d'erreur
    }
    float min = a->tab_distance[0];
    for(int i = 1; i < a->compteur_distance; i++){
        if(a->tab_distance[i] < min){
            min = a->tab_distance[i];
        }
    }
    return min;
}

float trouverMax(AVL* a){
    if(a->compteur_distance <= 0 || a->tab_distance == NULL){
        fprintf(stderr, "Tableau invalide.\n");
        return -1;  // Valeur d'erreur
    }
    float max = a->tab_distance[0];
    for(int i = 1; i < a->compteur_distance; i++){
        if(a->tab_distance[i] > max){
            max = a->tab_distance[i];
        }
    }
    return max;
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
void parcoursLimite(AVL* a, char* mode, FILE* fichierSortie, int* compteur, AVL** tab_id) {
    if (a != NULL && *compteur < 10) {
        parcoursLimite(a->fd, mode, fichierSortie, compteur, tab_id);
        
        if (*compteur >= 10) {
            return;
        }

        // Stocker la ville dans le tab_idleau
        tab_id[*compteur] = a;
        (*compteur)++;

        parcoursLimite(a->fg, mode, fichierSortie, compteur, tab_id);
    }
}

// Fonction qui crée un compteur avant le stockage des données
void parcours(AVL* a, char* mode, FILE* fichierSortie) {
    int compteur = 0;  // Initialiser le compteur
    AVL* tab_id[10];  // tableau pour stocker les 10 villes
    parcoursLimite(a, mode, fichierSortie, &compteur, tab_id);

    // Trier le tableau par ordre alphabétique
    qsort(tab_id, compteur, sizeof(AVL*), comparerVilles);

    // Écrire les données triées dans le fichier de sortie
    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "%s;%d;%d\n", tab_id[i]->ville, tab_id[i]->compteur_total, tab_id[i]->compteur_depart);
    }
}

void infixe(AVL* a, char* mode, FILE* fichierSortie){
    if(a!=NULL){
        infixe(a->fd,mode,fichierSortie);
        fprintf(fichierSortie, "%d;%f;%f;%f\n",a->id_trajet, trouverMin(a), moyenne(a), trouverMax(a));
        for (int i=0; i< a->compteur_distance;i++){
            fprintf(fichierSortie, "%f  ", a->tab_distance[i]);
            fprintf(fichierSortie, "\n");
        }
        infixe(a->fg,mode,fichierSortie);
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
                            printf("%d %d %s %s\n", id_trajet, id_etape, ville_depart, ville_arrivee);
                            //Ajout dans l'AVL de la ville de départ
                            a = ajouterAVL(a, ville_depart, id_trajet, id_etape, "depart");
                            //Ajout dans l'AVL de la ville d'arrivée
                            a = ajouterAVL(a, ville_arrivee, id_trajet, id_etape, "arrivee");
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
        //Fermeture des fichiers d'entrée et de sortie
        fclose(fichierEntree);
        fclose(fichierSortie);
    }
    if(strcmp(argv[2], "-s") == 0){
        char ligne[7000000];
        AVL* a=NULL;
        while(fgets(ligne,sizeof(ligne),fichierEntree)){
            int id_trajet;
            int id_etape;
            char ville_depart[100];
            char ville_arrivee[100];
            float distance;
            char *token = strtok(ligne, ";");
            if(token != NULL) {
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
                        if(token != NULL){
                            strncpy(ville_arrivee, token, sizeof(ville_arrivee));
                            ville_arrivee[sizeof(ville_arrivee) - 1] = '\0';
                            token = strtok(NULL, ";");
                            if(token != NULL){
                                if(sscanf(token, "%f", &distance) != 1){
                                    continue; // Passer à la ligne suivante
                                }
                                //Affichage de ce qui a été récupéré (FAUDRA L'ENLEVER)
                                printf("%d %f\n", id_trajet, distance);
                                a = ajouterAVL2(a, id_trajet, distance);
                            }
                        }
                    }
                }
            }
        }
        infixe(a,argv[3],fichierSortie);
        libererMemoire(a);
        //Fermeture des fichiers d'entrée et de sortie
        fclose(fichierEntree);
        fclose(fichierSortie);
    }
    
    return 0;
}
