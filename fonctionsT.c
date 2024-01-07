#include "fonctionsAVL.h"
#include "fonctionsT.h"

//Fonction pour créer un nouveau nœud
AVL* creerAVL_t(int id_trajet, int id_etape, char ville[], char type[]){
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

//Fonction pour ajouter une ville à la structure AVL
AVL* ajouterAVL_t(AVL* a, char ville[],int id_trajet, int id_etape, char type[]){
    if(a == NULL){
        return creerAVL_t(id_trajet, id_etape, ville, type);
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
        a->fg = ajouterAVL_t(a->fg, ville, id_trajet, id_etape, type);
    }
    else{
        a->fd = ajouterAVL_t(a->fd, ville, id_trajet, id_etape, type);
        
    }
    a->hauteur = 1 + maxi(hauteur(a->fg), hauteur(a->fd));
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
AVL* creerAVLtrier_t(char ville[],int compteur_total,int compteur_depart){
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
AVL* ajouterAVLtrier_t(AVL* a, char ville[], int compteur_total, int compteur_depart){
    if (a == NULL) {
        return creerAVLtrier_t(ville,compteur_total,compteur_depart);
    }
    if(a->compteur_total>compteur_total){
        a->fg = ajouterAVLtrier_t(a->fg,ville,compteur_total,compteur_depart);
    }
    else if(a->compteur_total<compteur_total){
        a->fd = ajouterAVLtrier_t(a->fd,ville,compteur_total,compteur_depart);
    }
    else{
        if(strcmp(ville,a->ville)<0){
            a->fg = ajouterAVLtrier_t(a->fg,ville,compteur_total,compteur_depart);
        }
        else if(strcmp(ville,a->ville)>0){
            a->fd = ajouterAVLtrier_t(a->fd,ville,compteur_total,compteur_depart);
        }
    }
    a->hauteur = 1 + maxi(hauteur(a->fg), hauteur(a->fd));
    int eq = equilibre(a);
    // Cas de l'équilibre à gauche-gauche
    if (eq > 1 && compteur_total < a->fg->compteur_total) {
        return rotationDroite(a);
    }

    // Cas de l'équilibre à droite-droite
    if (eq < -1 && compteur_total > a->fd->compteur_total) {
        return rotationGauche(a);
    }

    // Cas de l'équilibre à gauche-droite
    if (eq > 1 && compteur_total > a->fg->compteur_total) {
        a->fg = rotationGauche(a->fg);
        return rotationDroite(a);
    }

    // Cas de l'équilibre à droite-gauche
    if (eq < -1 && compteur_total < a->fd->compteur_total) {
        a->fd = rotationDroite(a->fd);
        return rotationGauche(a);
    }
    return a;
}

//Fonction pour copier les données du premier AVL dans un second et faire un nouveau tri
AVL* copier_trier_t(AVL* ancienAVL, AVL* nouveauAVL){
    if(ancienAVL != NULL){
        nouveauAVL = ajouterAVLtrier_t(nouveauAVL, ancienAVL->ville, ancienAVL->compteur_total, ancienAVL->compteur_depart);
        nouveauAVL = copier_trier_t(ancienAVL->fg, nouveauAVL);
        nouveauAVL = copier_trier_t(ancienAVL->fd, nouveauAVL);
    }
    return nouveauAVL;
}

//Fonction pour comparer deux chaînes de caractères (utilisée pour le tri alphabétique)
int comparerVilles(const void* a, const void* b) {
    AVL* villeA = *((AVL**)a);
    AVL* villeB = *((AVL**)b);
    return strcmp(villeA->ville, villeB->ville);
}

// Fonction qui permet de stocker les 10 villes qui ont le plus de trajets
void stockageDonnees_t(AVL* a, char* mode, FILE* fichierSortie, int* compteur, AVL** tab_id) {
    if (a != NULL && *compteur < 10) {
        stockageDonnees_t(a->fd, mode, fichierSortie, compteur, tab_id);
        
        if (*compteur >= 10) {
            return;
        }

        // Stocker la ville dans le tab_idleau
        tab_id[*compteur] = a;
        (*compteur)++;

        stockageDonnees_t(a->fg, mode, fichierSortie, compteur, tab_id);
    }
}

// Fonction qui trie les données du fichier de sortie dans l'ordre alphabétique
void tri_alphabetique(AVL* a, char* mode, FILE* fichierSortie) {
    int compteur = 0;  // Initialiser le compteur
    AVL* tab_id[10];  // tableau pour stocker les 10 villes
    stockageDonnees_t(a, mode, fichierSortie, &compteur, tab_id);

    // Trier le tableau par ordre alphabétique
    qsort(tab_id, compteur, sizeof(AVL*), comparerVilles);

    // Écrire les données triées dans le fichier de sortie
    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "%s;%d;%d\n", tab_id[i]->ville, tab_id[i]->compteur_total, tab_id[i]->compteur_depart);
    }
}
