#ifndef fonctionsT_h
#define fonctionsT_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AVL* creerAVL_t(int id_trajet, int id_etape, char ville[], char type[]);
AVL* ajouterAVL_t(AVL* a, char ville[], int id_trajet, int id_etape, char type[], int* h);
AVL* creerAVLtrier_t(char ville[], int compteur_total, int compteur_depart);
AVL* ajouterAVLtrier_t(AVL* a, char ville[], int compteur_total, int compteur_depart, int* h);
AVL* copier_trier_t(AVL* ancienAVL, AVL** nouvelAVL, int* h);
int comparerVilles(const void* a, const void* b);
void stockageDonnees_t(AVL* a, char* mode, FILE* fichierSortie, int* compteur, AVL** tab_id);
void tri_alphabetique(AVL* a, char* mode, FILE* fichierSortie);

#endif
