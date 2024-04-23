#ifndef LASER_H_
#define LASER_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
/*  version finale 

0. Auteur: 
    * MONTFORT Perig
1. Brève description de la méthode utilisée :
    * Ce code va d'abbord créer un graph. Un miroir est divisé en 4 noeuds, son côté nord, sud, est, ouest et est relié au miroir le plus proche
    en allant vers le côté voulu. Il renvoie -1 si il n'y a pas de miroir plus proche (on peut assimiler -1 au mur)
    Une fois le graphe créé (presque instantané selon les tests) il suffit donc de parcourir le graphe d'en haut à gauche à en haut à droite. 
2. * Ce graphe est efficace puisque la lumière ne peut jamais toucher deux fois la même face d'un miroir ( il suffit d'essayer sur papier 
    pour voir l'absurditée d'un tel comportement ) Supposons donc que la lumière touche la phase ouest et décide d'aller vers le miroir le plus au sud
    en notant vu ces deux faces il ne reste que les faces nord et est. Si la lumière touche l'autre face (donc nord ou est) il ne restera qu'une seule direction.
    On a donc un moyen simple de représenter le sens des miroirs. Pour tester les deux sens il suffit de faire un parcours et allant dans un sens et un dans l'autre.
3. * L'avantage de passer par un graphe est que l'on ne prends pas en compte la taille du tableau. Puisque ce n'est que des comparaisons un tableau de taille 10000 et 10 auraient la même durée. 
    Le seul bémole est lorsque le nombre de miroirs devient très grand (dans mes tests vers 300 à 400 miroirs selon les cas), le parcours récursif pose des problèmes.
    Cependant je n'ai pas trouvé de meilleur moyen de réaliser ce problème pour de très grand nombre de miroirs donc je pense que cette version reste très efficace.
    De plus, vu que l'algorithme a du mal pour un graphe à 400 miroirs disons, et que la création du graphe est en O(n²), le créer coûte environ 16000 instructions ce qui est presque instantané.
    la création de la structure n'influence donc pas (ou peu) le temps de ce programme.

    Note: il n'y a pas vraiment de soucis en complexité spatiale, la pile peut atteindre la taille n sur le parcours du graphe et le graphe est en O(n).
*/

typedef int contenu;
struct maillon_s {
    contenu valeur;
    struct maillon_s* suivant;
};
typedef struct maillon_s maillon; 

typedef maillon* pile;

pile creer_pile(void);
bool est_vide_pile(pile);
void empiler(pile*, contenu);
contenu depiler(pile*);
bool est_dans_pile(pile, contenu);

struct miroirs_s {
    int x;
    int y;
};
typedef struct miroirs_s miroirs;

typedef int* graphe;

graphe make_graph(miroirs*,  int, int, int);
int parcours_dfs(graphe, miroirs*, pile*, int, int);
int vfinale(int*, miroirs*);

/* version naïve
0. Auteur : 
    * MONTFORT Perig
1. Brève description de la méthode utilisée :
    * Ce code est très brut. Pour ce faire il teste d'abbord tous les chemins en parcourant bêtement avec des coordonnées i et j un tableau où les miroirs sont orientés
    Il y a donc une fonction qui s'occupe de créer toutes les orientations possibles
    Une permettant de créer un tableau
    Une le parcourant et renvoyant une pile des coordonnées 
    Une traîtant le résultat et l'ajoutant à une pile de solution.

2. * Puisque l'on cherche le nombre de chemins, on aura sûrement deux solutions semblables..
    Il faut donc rajouter une fonction qui traîte la pile de solution pour en créer une sans doublon. 
    On renvoie finalement la taille de la pile.

3. * La complexité de ce programme est affreuse. La complexité spatiale monte en O(n³) avec la pile de pile et la pile d'appel
    La complexité temporelle semble exponentiellecde même, je ne pense pas qu'il est possible (sans faire des étapes inutiles) de faire pire en complexité
    sur ce problème.. 

*/

void supprime_pile(pile);
bool change_position(bool* , int);
bool compare_pile(pile, pile);
int** cree_parcours(miroirs*, int, int, int, bool*);
void supprime_parcours(int**, int);
pile parcours_grille(int, int, int**);

struct maillon_solution_s {
    pile valeur;
    struct maillon_solution_s* suivant;
};
typedef struct maillon_solution_s maillon_solution; 
typedef maillon_solution* pile_solution;

pile_solution creer_pile_solution(void);
bool est_vide_pile_solution(pile_solution);
void empiler_solution(pile_solution*, pile);
pile depiler_solution(pile_solution*);
bool est_dans_pile_solution(pile_solution, pile);
int taille_pile_solution(pile_solution*);
int compte_solutions_differents(pile_solution*);
int v1(int*, miroirs*);

#endif