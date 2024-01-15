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
    int hauteur;
}AVL;

int hauteur(AVL* a);
int equilibre(AVL* a);
int maxi(int a, int b);
AVL* rotationGauche(AVL* y);
AVL* rotationDroite(AVL* x);
void libererMemoire(AVL* a);

#endif
