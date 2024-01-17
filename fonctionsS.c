#include "fonctionsAVL.h"
#include "fonctionsS.h"

//Fonction pour créer un nouveau nœud
AVL* creerAVL_s(int id_trajet, float distance){
    AVL* nouveau = (AVL*)malloc(sizeof(AVL));
    if(nouveau == NULL){
        exit(1);
    }
    else{
        nouveau->id_trajet = id_trajet;
        nouveau->distance = distance;
        nouveau->min = distance;
        nouveau->max = distance;
        nouveau->moy = distance;
        nouveau->diff = 0;
        nouveau->compteur_distance = 1;
        //Allouer de la mémoire pour le tableau des distances
        nouveau->tab_distance = (float*)malloc(sizeof(float));
        if(nouveau->tab_distance != NULL){
            nouveau->tab_distance[0] = distance;
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

//Fonction qui fait la moyenne des valeurs dans un tableau
float moyenne(AVL* a){
    if(a == NULL){
        exit(1);
    }
    float res = 0;
    for(int i=0;i<a->compteur_distance;i++){
        res += a->tab_distance[i];
    }
    return res/a->compteur_distance;
}

//Fonction pour ajouter un identifiant de trajet dans l'AVL
AVL* ajouterAVL_s(AVL* a, float distance, int id_trajet){
    //AVL vide donc on crée directement le nœud
    if(a == NULL){
        return creerAVL_s(id_trajet, distance);
    }
    //L'identifiant de trajet a été trouvé dans l'AVL
    if(a->id_trajet == id_trajet){
        //Mise à jour du compteur de distance car on va en ajouter une
        a->compteur_distance++;
        //Réallocation du tableau avec une taille augmentée
        float* temp = (float*)realloc(a->tab_distance, a->compteur_distance * sizeof(float));
        if(temp != NULL){
            //Ajout de la distance dans le tableau
            a->tab_distance = temp;
            a->tab_distance[a->compteur_distance - 1] = distance;
            //Mise à jour du maximum, du minimum, de la moyenne et de la différence max-min
            float nouvelle_distance = distance;
            a->min = (nouvelle_distance < a->min) ? nouvelle_distance : a->min;
            a->max = (nouvelle_distance > a->max) ? nouvelle_distance : a->max;
            a->moy = moyenne(a);
            a->diff = a->max - a->min;
        }
        else{
            //Gestion de l'erreur d'allocation mémoire
            fprintf(stderr, "Erreur lors de la réallocation du tableau d'identifiants de trajets.\n");
            exit(EXIT_FAILURE);
        }
    }
    //Ajout de l'identifiant de trajet dans le sous arbre de gauche
    else if(id_trajet<a->id_trajet){
        a->fg = ajouterAVL_s(a->fg,distance,id_trajet);
    }
    //Ajout de l'identifiant de trajet dans le sous arbre de droite
    else{
        a->fd = ajouterAVL_s(a->fd,distance,id_trajet);
    }
    //Mise à jour de la hauteur et de l'équilibre
    a->hauteur = 1 + maxi(hauteur(a->fg), hauteur(a->fd));
    int eq = equilibre(a);
    //Cas de l'équilibre à gauche-gauche
    if(eq > 1 && id_trajet < a->fg->id_trajet){
        return rotationDroite(a);
    }
    //Cas de l'équilibre à droite-droite
    if(eq < -1 && id_trajet > a->fd->id_trajet){
        return rotationGauche(a);
    }
    //Cas de l'équilibre à gauche-droite
    if(eq > 1 && id_trajet > a->fg->id_trajet){
        a->fg = rotationGauche(a->fg);
        return rotationDroite(a);
    }
    //Cas de l'équilibre à droite-gauche
    if(eq < -1 && id_trajet < a->fd->id_trajet){
        a->fd = rotationDroite(a->fd);
        return rotationGauche(a);
    }
    return a;
}

//Fonction pour créer un nouveau nœud
AVL* creerAVLtrier_s(float min, float max, float moy, int id_trajet){
    AVL* nouveau = (AVL*)malloc(sizeof(AVL));
    if(nouveau == NULL){
        exit(1);
    }
    else{
        nouveau->min = min;
        nouveau->max = max;
        nouveau->moy = moy;
        nouveau->diff = max - min;
        nouveau->id_trajet = id_trajet;
        nouveau->fg = NULL;
        nouveau->fd = NULL;
        nouveau->hauteur = 1;
    }
    return nouveau;
}

//Fonction pour ajouter une nouvelle différence de distance dans l'AVL
AVL* ajouterAVLtrier_s(AVL* a, float min, float max, float moy, int id_trajet){
    float diff=max-min;
    //AVL vide donc on crée directement le nœud
    if(a == NULL){
        return creerAVLtrier_s(min,max,moy,id_trajet);
    }
    //Ajout de la différence dans le sous arbre de gauche
    if(a->diff>diff){
        a->fg = ajouterAVLtrier_s(a->fg,min,max,moy,id_trajet);
    }
    //Ajout de la différence dans le sous arbre de droite
    else if(a->diff<diff){
        a->fd = ajouterAVLtrier_s(a->fd,min,max,moy,id_trajet);
    }
    //La différence existe déjà donc on trie par identifiant de trajet
    else{
        //Ajout de la différence dans le sous arbre de gauche
        if(a->id_trajet>id_trajet){
            a->fg = ajouterAVLtrier_s(a->fg,min,max,moy,id_trajet);
        }
        //Ajout de la différence dans le sous arbre de droite
        else if(a->id_trajet<id_trajet){
            a->fd = ajouterAVLtrier_s(a->fd,min,max,moy,id_trajet);
        }
    }
    //Mise à jour de la hauteur et de l'équilibre
    a->hauteur = 1 + maxi(hauteur(a->fg), hauteur(a->fd));
    int eq = equilibre(a);
    //Cas de l'équilibre à gauche-gauche
    if(eq > 1 && diff < a->fg->diff){
        return rotationDroite(a);
    }
    //Cas de l'équilibre à droite-droite
    if(eq < -1 && diff > a->fd->diff){
        return rotationGauche(a);
    }
    //Cas de l'équilibre à gauche-droite
    if(eq > 1 && diff > a->fg->diff){
        a->fg = rotationGauche(a->fg);
        return rotationDroite(a);
    }
    //Cas de l'équilibre à droite-gauche
    if (eq < -1 && diff < a->fd->diff) {
        a->fd = rotationDroite(a->fd);
        return rotationGauche(a);
    }
    return a;
}

//Fonction pour copier les données du premier AVL dans un second et faire un nouveau tri
AVL* copier_trier_s(AVL* ancienAVL, AVL* nouveauAVL){
    if(ancienAVL != NULL){
        //Copier les données dans le nouvel AVL
        nouveauAVL = ajouterAVLtrier_s(nouveauAVL, ancienAVL->min, ancienAVL->max, ancienAVL->moy, ancienAVL->id_trajet);
        //Appeler récursivement les sous-arbres
        nouveauAVL = copier_trier_s(ancienAVL->fg, nouveauAVL);
        nouveauAVL = copier_trier_s(ancienAVL->fd, nouveauAVL);
    }
    return nouveauAVL;
}

//Fonction pour stocker dans le fichier de sortie les 50 plus grandes différences de distances max-min
void stockageDonnees_s(AVL* a, char* mode, FILE* fichierSortie, int* compteur) {
    if(a != NULL && *compteur < 50){
        stockageDonnees_s(a->fd, mode, fichierSortie, compteur);
        if (*compteur >= 50) {
            return;
        }
        fprintf(fichierSortie, "%d;%d;%.3f;%.3f;%.3f;%.3f\n", *compteur+1, a->id_trajet, a->min, a->max, a->moy, a->diff);
        (*compteur)++;
        stockageDonnees_s(a->fg, mode, fichierSortie, compteur);
    }
}
