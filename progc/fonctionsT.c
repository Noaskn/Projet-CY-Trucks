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
        nouveau->compteur_total = 1;
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
            exit(1);
        }
        nouveau->fg = NULL;
        nouveau->fd = NULL;
        nouveau->equilibre = 0;
    }
    return nouveau;
}

//Fonction pour ajouter une ville dans l'AVL
AVL* ajouterAVL_t(AVL* a, char ville[], int id_trajet, int id_etape, char type[], int* h){
    
    //AVL vide donc on crée directement le nœud
    if(a == NULL){
        *h = 1;
        a = creerAVL_t(id_trajet, id_etape, ville, type);
    }
    
    //Comparaison entre la ville de l'AVL et la ville passée en paramètre
    int compare=strcmp(ville, a->ville);
    //Si c'est la même ville
    if(compare == 0){
        *h = 0;
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
        //Si c'est la première étape d'un trajet on augmente le compteur
        if(strcmp(type,"depart") == 0 && id_etape == 1){
            a->compteur_depart++;
        }
        //Ville déjà présente donc on met à jour le compteur total et le tableau comptenant les identifiants
        //Réallocation du tableau avec une taille augmentée
        int* temp = (int*)realloc(a->tab_id, (a->compteur_total+1) * sizeof(int));
        if(temp != NULL){
            //Ajout de l'identifiant de trajet dans le tableau
            a->tab_id = temp;
            a->tab_id[a->compteur_total-1] = id_trajet;
            a->compteur_total++;
        }
        else{
            //Gestion de l'erreur d'allocation mémoire
            exit(1);
        }
    }
    
    //Ajout de la ville dans le sous arbre de gauche
    else if(compare < 0){
        a->fg = ajouterAVL_t(a->fg, ville, id_trajet, id_etape, type, h);
        *h = -*h;
    }
    
    //Ajout de la ville dans le sous arbre de droite
    else{
        a->fd = ajouterAVL_t(a->fd, ville, id_trajet, id_etape, type, h);
    }
    
    //Equilibre de l'AVL
    if(*h != 0){
        a->equilibre = a->equilibre + *h;
        a=equilibrerAVL(a);
        if(a->equilibre == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
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
        nouveau->equilibre = 0;
    }
    return nouveau;
}

//Fonction pour ajouter un nouveau compteur dans l'AVL
AVL* ajouterAVLtrier_t(AVL* a, char ville[], int compteur_total, int compteur_depart, int* h){
    
    //AVL vide donc on crée directement le nœud
    if(a == NULL){
        *h = 1;
        a = creerAVLtrier_t(ville,compteur_total,compteur_depart);
    }
    
    //Ajout du compteur dans le sous arbre de gauche
    if(a->compteur_total>compteur_total){
        a->fg = ajouterAVLtrier_t(a->fg,ville,compteur_total,compteur_depart,h);
        *h = -*h;
    }
    
    //Ajout du compteur dans le sous arbre de droite
    else if(a->compteur_total<compteur_total){
        a->fd = ajouterAVLtrier_t(a->fd,ville,compteur_total,compteur_depart,h);
    }
    
    //Le compteur existe déjà donc on trie par ville
    else{
        
        //Ajout du compteur dans le sous arbre de gauche
        if(strcmp(ville,a->ville)<0){
            a->fg = ajouterAVLtrier_t(a->fg,ville,compteur_total,compteur_depart,h);
            *h = -*h;
        }
        
        //Ajout du compteur dans le sous arbre de droite
        else if(strcmp(ville,a->ville)>0){
            a->fd = ajouterAVLtrier_t(a->fd,ville,compteur_total,compteur_depart,h);
        }
        
        else{
            *h = 0;
            return a;
        }
    }
    
    //Equilibre de l'AVL
    if(*h != 0){
        a->equilibre = a->equilibre + *h;
        a=equilibrerAVL(a);
        if(a->equilibre == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return a;
}

//Fonction pour copier les données du premier AVL dans un second et faire un nouveau tri
AVL* copier_trier_t(AVL* ancienAVL, AVL** nouvelAVL, int* h){
    if(ancienAVL != NULL){
        //Copier les données dans le nouvel AVL
        *nouvelAVL = ajouterAVLtrier_t(*nouvelAVL, ancienAVL->ville, ancienAVL->compteur_total, ancienAVL->compteur_depart, h);
        //Appeler récursivement les sous-arbres
        if(ancienAVL->fg != NULL){
            copier_trier_t(ancienAVL->fg, nouvelAVL, h);
        }
        if(ancienAVL->fd != NULL){
            copier_trier_t(ancienAVL->fd, nouvelAVL, h);
        }
    }
    return *nouvelAVL;
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
    AVL* tab_id[10];  // Tableau pour stocker les 10 villes
    stockageDonnees_t(a, mode, fichierSortie, &compteur, tab_id);
    //Trier le tableau par ordre alphabétique
    qsort(tab_id, compteur, sizeof(AVL*), comparerVilles);
    //Écrire les données triées dans le fichier de sortie
    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "%s;%d;%d\n", tab_id[i]->ville, tab_id[i]->compteur_total, tab_id[i]->compteur_depart);
    }
}
