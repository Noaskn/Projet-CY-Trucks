#include "fonctionsAVL.h"
#include "fonctionsT.h"

//Fonction pour créer un nouveau nœud
AVL* creerAVL_t(int id_trajet, int id_etape, char ville[], char type[]){
    AVL* nouveau = (AVL*)malloc(sizeof(AVL));
    if(nouveau == NULL){
        exit(1);
    }
    else{
        nouveau->id_trajet = id_trajet;
        nouveau->id_etape = id_etape;
        strcpy(nouveau->ville, ville);
        nouveau->compteur_total = 0;
        nouveau->compteur_depart = 0;
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
            //Gestion de l'erreur d'allocation mémoire
            free(nouveau);
            return NULL;
        }
        nouveau->fg = NULL;
        nouveau->fd = NULL;
        nouveau->hauteur = 1;
    }
    return nouveau;
}

//Fonction pour ajouter une ville dans l'AVL
AVL* ajouterAVL_t(AVL* a, char ville[],int id_trajet, int id_etape, char type[]){
    //AVL vide donc on crée directement le nœud
    if(a == NULL){
        return creerAVL_t(id_trajet, id_etape, ville, type);
    }
    //Comparaison entre la ville de l'AVL et la ville passée en paramètre
    int compare=strcmp(ville, a->ville);
    //Si c'est la même ville
    if(compare == 0){
        int i=0;
        //On regarde si l'identifiant est déjà présent
        while(a->tab_id[i] != 0){
            //Ville et identifiant déjà présents
            if(id_trajet == a->tab_id[i]){
                 //Si c'est la première étape d'un trajet on augmente le compteur
                if(strcmp(type,"depart") == 0 && id_etape == 1){
                    a->compteur_depart++;
                }
                return a;
            }
            i++;
        }
        //Ville déjà présente donc on met à jour le compteur total et le tableau comptenant les identifiants
        a->compteur_total++;
         //Si c'est la première étape d'un trajet on augmente le compteur
        if(strcmp(type,"depart") == 0 && id_etape == 1){
            a->compteur_depart++;
        }
        //Réallocation du tableau avec une taille augmentée
        int* temp = (int*)realloc(a->tab_id, a->compteur_total * sizeof(int));
        if(temp != NULL){
            //Ajout de l'identifiant de trajet dans le tableau
            a->tab_id = temp;
            a->tab_id[a->compteur_total - 1] = id_trajet;
        }
        else{
            //Gestion de l'erreur d'allocation mémoire
            fprintf(stderr, "Erreur lors de la réallocation du tableau d'identifiants de trajets.\n");
            exit(EXIT_FAILURE);
        }
    }
    //Ajout de la ville dans le sous arbre de gauche
    else if(compare < 0){
        a->fg = ajouterAVL_t(a->fg, ville, id_trajet, id_etape, type);
    }
    //Ajout de la ville dans le sous arbre de droite
    else{
        a->fd = ajouterAVL_t(a->fd, ville, id_trajet, id_etape, type);
        
    }
    //Mise à jour de la hauteur et de l'équilibre
    a->hauteur = 1 + maxi(hauteur(a->fg), hauteur(a->fd));
    int eq = equilibre(a);
    //Cas de l'équilibre à gauche-gauche
    if(eq > 1 && strcmp(ville, a->fg->ville) < 0){
        return rotationDroite(a);
    }
    //Cas de l'équilibre à droite-droite
    if(eq < -1 && strcmp(ville, a->fd->ville) > 0){
        return rotationGauche(a);
    }
    //Cas de l'équilibre à gauche-droite
    if(eq > 1 && strcmp(ville, a->fg->ville) > 0){
        a->fg = rotationGauche(a->fg);
        return rotationDroite(a);
    }
    //Cas de l'équilibre à droite-gauche
    if (eq < -1 && strcmp(ville, a->fd->ville) < 0) {
        a->fd = rotationDroite(a->fd);
        return rotationGauche(a);
    }
    return a;
}

//Fonction pour créer un nouveau nœud
AVL* creerAVLtrier_t(char ville[], int compteur_total, int compteur_depart){
    AVL* nouveau = (AVL*)malloc(sizeof(AVL));
    if(nouveau == NULL){
        exit(1);
    }
    else{
        strcpy(nouveau->ville, ville);
        nouveau->compteur_total = compteur_total;
        nouveau->compteur_depart = compteur_depart;
        nouveau->fg = NULL;
        nouveau->fd = NULL;
        nouveau->hauteur = 1;
    }
    return nouveau;
}

//Fonction pour ajouter un nouveau compteur dans l'AVL
AVL* ajouterAVLtrier_t(AVL* a, char ville[], int compteur_total, int compteur_depart){
    //AVL vide donc on crée directement le nœud
    if(a == NULL){
        return creerAVLtrier_t(ville,compteur_total,compteur_depart);
    }
    //Ajout du compteur dans le sous arbre de gauche
    if(a->compteur_total>compteur_total){
        a->fg = ajouterAVLtrier_t(a->fg,ville,compteur_total,compteur_depart);
    }
    //Ajout du compteur dans le sous arbre de droite
    else if(a->compteur_total<compteur_total){
        a->fd = ajouterAVLtrier_t(a->fd,ville,compteur_total,compteur_depart);
    }
    //Le compteur existe déjà donc on trie par ville
    else{
        //Ajout du compteur dans le sous arbre de gauche
        if(strcmp(ville,a->ville)<0){
            a->fg = ajouterAVLtrier_t(a->fg,ville,compteur_total,compteur_depart);
        }
        //Ajout du compteur dans le sous arbre de droite
        else if(strcmp(ville,a->ville)>0){
            a->fd = ajouterAVLtrier_t(a->fd,ville,compteur_total,compteur_depart);
        }
    }
    //Mise à jour de la hauteur et de l'équilibre
    a->hauteur = 1 + maxi(hauteur(a->fg), hauteur(a->fd));
    int eq = equilibre(a);
    //Cas de l'équilibre à gauche-gauche
    if(eq > 1 && compteur_total < a->fg->compteur_total){
        return rotationDroite(a);
    }
    //Cas de l'équilibre à droite-droite
    if(eq < -1 && compteur_total > a->fd->compteur_total){
        return rotationGauche(a);
    }
    //Cas de l'équilibre à gauche-droite
    if(eq > 1 && compteur_total > a->fg->compteur_total){
        a->fg = rotationGauche(a->fg);
        return rotationDroite(a);
    }
    //Cas de l'équilibre à droite-gauche
    if(eq < -1 && compteur_total < a->fd->compteur_total){
        a->fd = rotationDroite(a->fd);
        return rotationGauche(a);
    }
    return a;
}

//Fonction pour copier les données du premier AVL dans un second et faire un nouveau tri
AVL* copier_trier_t(AVL* ancienAVL, AVL* nouveauAVL){
    if(ancienAVL != NULL){
        //Copier les données dans le nouvel AVL
        nouveauAVL = ajouterAVLtrier_t(nouveauAVL, ancienAVL->ville, ancienAVL->compteur_total, ancienAVL->compteur_depart);
        //Appeler récursivement les sous-arbres
        nouveauAVL = copier_trier_t(ancienAVL->fg, nouveauAVL);
        nouveauAVL = copier_trier_t(ancienAVL->fd, nouveauAVL);
    }
    return nouveauAVL;
}

//Fonction pour comparer deux chaînes de caractères
int comparerVilles(const void* a, const void* b){
    AVL* villeA = *((AVL**)a);
    AVL* villeB = *((AVL**)b);
    return strcmp(villeA->ville, villeB->ville);
}

// Fonction pour stocker dans le fichier de sortie les 10 villes qui ont le plus de trajets
void stockageDonnees_t(AVL* a, char* mode, FILE* fichierSortie, int* compteur, AVL** tab_id){
    if(a != NULL && *compteur < 10){
        stockageDonnees_t(a->fd, mode, fichierSortie, compteur, tab_id);
        if(*compteur >= 10){
            return;
        }
        //Stocker la ville dans le tableau
        tab_id[*compteur] = a;
        (*compteur)++;
        stockageDonnees_t(a->fg, mode, fichierSortie, compteur, tab_id);
    }
}

//Fonction qui trie les données du fichier de sortie dans l'ordre alphabétique
void tri_alphabetique(AVL* a, char* mode, FILE* fichierSortie){
    if(a == NULL){
        exit(1);
    }
    int compteur = 0;  // Initialiser le compteur
    AVL* tab_id[10];  // tableau pour stocker les 10 villes
    stockageDonnees_t(a, mode, fichierSortie, &compteur, tab_id);
    //Trier le tableau par ordre alphabétique
    qsort(tab_id, compteur, sizeof(AVL*), comparerVilles);
    //Écrire les données triées dans le fichier de sortie
    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "%s;%d;%d\n", tab_id[i]->ville, tab_id[i]->compteur_total, tab_id[i]->compteur_depart);
    }
}
