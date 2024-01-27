#ifndef fonctionsAVL_h
#define fonctionsAVL_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure AVL
typedef struct AVL{
    int id_trajet;
    int id_etape;
    float distance;
    float min;
    float max;
    float moy;
    float diff;
    char ville[50];
    int *tab_id;
    float *tab_distance;
    int compteur_total;
    int compteur_depart;
    int compteur_distance;
    struct AVL* fg;
    struct AVL* fd;
    int equilibre;
}AVL;

int maxi(int a,int b);
int mini(int a,int b);
AVL* rotationGauche(AVL* a);
AVL* rotationDroite(AVL* a);
AVL* doubleRotationGauche(AVL* a);
AVL* doubleRotationDroite(AVL* a);
AVL* equilibrerAVL(AVL* a);
void libererMemoire(AVL* a);

#endif
