#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct AVL{
    int id_trajet;
    char ville[50];
    int *tab;
    int compteur_total;
    int compteur_depart;
    struct AVL* fg;
    struct AVL* fd;
    int equilibre;
}AVL;

//Fonction pour créer un nouveau nœud
AVL* creerNoeud(int id_trajet, char ville[]){
    AVL* nouveau = (AVL*)malloc(sizeof(AVL));
    if(nouveau!=NULL){
        nouveau->id_trajet=id_trajet;
        strcpy(nouveau->ville, ville);
        nouveau->compteur_total=1;
        nouveau->compteur_depart=1;
        //Allouer de la mémoire pour le tableau d'identifiants de trajets
        nouveau->tab = (int*)malloc(sizeof(int));
        if(nouveau->tab != NULL){
            nouveau->tab[0] = id_trajet;
        }
        else{
            // Gestion de l'erreur d'allocation mémoire
            free(nouveau);
            return NULL;
        }
        nouveau->tab=NULL;
        nouveau->gauche=NULL;
        nouveau->droite=NULL;
        nouveau->equilibre=0;
    }
    return nouveau;
}

//Fonction pour obtenir le maximum de deux entiers
int max(int a, int b){
    return (a > b) ? a : b;
}

//Fonction pour obtenir le minimum de deux entiers
int min(int a, int b){
    return (a < b) ? a : b;
}

//Fonction pour effectuer une rotation simple à droite
AVL* rotationDroite(AVL* a){
    AVL* pivot;
    int eq_a,eq_p;
    pivot=a->fg;
    a->fg=pivot->fd;
    pivot->fd=a;
    eq_a=a->equilibre;
    eq_p=pivot->equilibre;
    a->equilibre=eq_a-min(eq_p,0)+1;
    pivot->equilibre=max(max(eq_a+2,eq_a+eq_p+2),eq_p+1);
    a=pivot;
    return a;
}

//Fonction pour effectuer une rotation simple à gauche
AVL* rotationGauche(AVL* a){
    AVL* pivot;
    int eq_a,eq_p;
    pivot=a->fd;
    a->fd=pivot->fg;
    pivot->fg=a;
    eq_a=a->equilibre;
    eq_p=pivot->equilibre;
    a->equilibre=eq_a-max(eq_p,0)-1;
    pivot->equilibre=min(min(eq_a-2,eq_a+eq_p-2),eq_p-1);
    a=pivot;
    return a;
}

//Fonction pour effectuer une rotation double à gauche
AVL* douleRotationGauche(AVL* a){
    a->fd=rotationDroite(a->fd);
    return rotationGauche(a);
}

//Fonction pour effectuer une rotation double à droite
AVL* douleRotationDroite(AVL* a){
    a->fg=rotationGauche(a->fg);
    return rotationDroite(a);
}

//Fonction pour équilibrer l'arbre
AVL* equilibreAVL(AVL* a){
    if(a->equilibre>=2){
        if(a->fd->equilibre>=0){
            return rotationGauche(a);
        }
        else{
            return douleRotationGauche(a);
        }
    }
    else if(a->equilibre<=0){
        if(a->fg->equilibre<=0){
            return rotationDroite(a);
        }
        else{
            return douleRotationDroite(a);
        }
    }
    return a;
}

// Fonction principale pour ajouter une ville à la structure AVL
AVL* ajouterAVL(AVL* a, char ville[],int id_trajet, int *h,char type[]){
    if(a == NULL){
        *h=1;
        a=creerAVL(id_trajet, ville);
        return a;
    }
    int compare=strcmp(ville, a->ville);
    if(compare=0){
        for(int i=0;a->tab[i]==NULL,i++){
            if(id_trajet==a->tab[i]){
                return a;
            }
        }
        else{
            //Ville déjà présente donc on met à jour les informations nécessaires
            a->compteur_total++;
            if(strcmp(type,"depart")){
                a->compteur_depart++;
            }
            //Réallouer le tableau avec une taille augmentée
            int* temp = (int*)realloc(a->tab, a->trajets_depart * sizeof(int));
            if(temp != NULL){
                a->tab = temp;
                a->tab[a->trajets_depart - 1] = id_trajet;
            }
            else{
            // Gestion de l'erreur d'allocation mémoire
            fprintf(stderr, "Erreur lors de la réallocation du tableau d'identifiants de trajets.\n");
            exit(EXIT_FAILURE);
            }
        }
    }
    else if(compare < 0){
        a->fg = ajouterAVL(a->fg, ville, id_trajet, h);
        *h=-*h;
    }
    else{
        a->fd = ajouterAVL(a->fd, ville, id_trajet, h);
    }
    if(*h!=0){
		a->equilibre=a->equilibre+*h;
		a=equilibreAVL(a);
		if(a->equilibre==0){
			*h=0;
		}
		else{
			*h=1;
		}
	}
    return a;
}

void libererMemoire(AVL* a){
    if(a!= NULL){
        libererMemoire(a->fg);
        libererMemoire(a->fd);
        free(a);
    }
}

int main(int argc, char *argv[]){
    //Vérification du nombre d'arguments
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

    char *ligne;
    int id_trajet;
    int id_etape;
    char *ville_depart;
    char *ville_arrivee;
    float distance;
    char *nom;

    while(fgets(ligne,sizeof(ligne),fichierEntree) != NULL){
        sscanf(ligne, "%d[^;];%d[^;];%s[^;];%s[^;];%f[^;];%s[^\n]", &id_trajet, &id_etape, ville_depart, ville_arrivee, &distance, nom);
        //Traitement des données en fonction du type
        if(strcmp(argv[3], "-t") == 0){
            //Faut envoyer dans les fichiers de tri avec ville_depart ville_arrivee nom
        }
        else if(strcmp(argv[3], "-s") == 0){
            //Faut envoyer dans les fichiers de tri avec distance et id_trajet
        }
        else{
            fprintf(stderr, "Type de traitement non reconnu : %s\n", argv[3]);
            fclose(fichierEntree);
            return 1;
        }
    }

    //Création du fichier de sortie
    FILE *fichierSortie = fopen(argv[2], "w");
    if(fichierSortie == NULL){
        perror("Erreur lors de l'ouverture du fichier de sortie");
        fclose(fichierEntree);
        return 1;
    }
    //Fermeture des fichiers
    fclose(fichierEntree);
    fclose(fichierEntree);
    return 0;
}
