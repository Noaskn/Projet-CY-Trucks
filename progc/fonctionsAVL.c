#include "fonctionsAVL.h"

//Fonction qui donne le maximum entre deux entiers
int maxi(int a,int b){
    if(a>b){
        return a;
    }
    else{
        return b;
    }
}

//Fonction qui donne le minimum entre deux entiers
int mini(int a,int b){
    if(a>b){
        return b;
    }
    else{
        return a;
    }
}

//Fonction qui fait une rotation vers la gauche
AVL* rotationGauche(AVL* a){
    if(a==NULL){
        exit(1);
    }
    AVL* pivot;
    int eq_a,eq_p;
    pivot=a->fd;
    a->fd=pivot->fg;
    pivot->fg=a;
    eq_a=a->equilibre;
    eq_p=pivot->equilibre;
    a->equilibre=eq_a-maxi(eq_p,0)-1;
    pivot->equilibre=mini(mini(eq_a-2,eq_a+eq_p-2),eq_p-1);
    a=pivot;
    return a;
}

//Fonction qui fait une rotation vers la droite
AVL* rotationDroite(AVL* a){
    if(a==NULL){
        exit(1);
    }
    AVL* pivot;
    int eq_a,eq_p;
    pivot=a->fg;
    a->fg=pivot->fd;
    pivot->fd=a;
    eq_a=a->equilibre;
    eq_p=pivot->equilibre;
    a->equilibre=eq_a-mini(eq_p,0)+1;
    pivot->equilibre=maxi(maxi(eq_a+2,eq_a+eq_p+2),eq_p+1);
    a=pivot;
    return a;
}

//Fonction qui fait une double rotation vers la gauche
AVL* doubleRotationGauche(AVL* a){
    if(a==NULL){
        exit(1);
    }
    a->fd=rotationDroite(a->fd);
    return rotationGauche(a);
}

//Fonction qui fait une double rotation vers la droite
AVL* doubleRotationDroite(AVL* a){
    if(a==NULL){
        exit(1);
    }
    a->fg=rotationGauche(a->fg);
    return rotationDroite(a);
}

//Fonction qui equilibre l'AVL en fonction du facteur d'équilibre du noeud
AVL* equilibrerAVL(AVL* a){
    if(a==NULL){
        exit(1);
    }
    if(a->equilibre>=2){
        if(a->fd->equilibre>=0){
            return rotationGauche(a);
        }
        else{
            return doubleRotationGauche(a);
        }
    }
    else if(a->equilibre<=-2){
        if(a->fg->equilibre<=0){
            return rotationDroite(a);
        }
        else{
            return doubleRotationDroite(a);
        }
    }
    return a;
}

//Fonction qui libère la mémoire de l'AVL
void libererMemoire(AVL* a){
    if(a!=NULL){
        libererMemoire(a->fg);
        libererMemoire(a->fd);
        free(a);
    }
}
