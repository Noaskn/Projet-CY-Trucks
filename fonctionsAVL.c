#include "fonctionsAVL.h"

//Fonction pour calculer la hauteur d'un nœud
int hauteur(AVL* a){
    if(a == NULL){
        return 0;
    }
    return a->hauteur;
}

//Fonction pour calculer l'équilibre d'un nœud
int equilibre(AVL* a){
    if(a == NULL){
        return 0;
    }
    return hauteur(a->fg) - hauteur(a->fd);
}

//Fonction pour obtenir le maximum de deux entiers
int maxi(int a, int b){
    return (a > b ? b : a);
}

//Fonction pour faire une rotation simple à gauche
AVL* rotationGauche(AVL* y){
    AVL* x = y->fd;
    AVL* T2 = x->fg;
    x->fg = y;
    y->fd = T2;
    y->hauteur = 1 + maxi(hauteur(y->fg), hauteur(y->fd));
    x->hauteur = 1 + maxi(hauteur(x->fg), hauteur(x->fd));
    return x;
}

//Fonction pour faire une rotation simple à droite
AVL* rotationDroite(AVL* x){
    AVL* y = x->fg;
    AVL* T2 = y->fd;
    y->fd = x;
    x->fg = T2;
    x->hauteur = 1 + maxi(hauteur(x->fg), hauteur(x->fd));
    y->hauteur = 1 + maxi(hauteur(y->fg), hauteur(y->fd));
    return y;
}

//Fonction qui libère la mémoire de l'AVL
void libererMemoire(AVL* a){
    if(a!=NULL){
        libererMemoire(a->fg);
        libererMemoire(a->fd);
        free(a);
    }
}
