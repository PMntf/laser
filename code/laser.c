#include "./laser.h"

pile creer_pile(){
    return NULL;
}
bool est_vide_pile(pile p){
    return p == NULL;
}
void empiler(pile* p, contenu elt) {
    maillon* m = malloc(sizeof(maillon));
    assert(m != NULL);
    m->valeur = elt;
    m->suivant = *p;
    *p = m;
}

contenu depiler(pile* p) {
    assert(!est_vide_pile(*p));
    maillon* sommet = *p;
    contenu elt = sommet->valeur;
    *p = sommet->suivant;
    free(sommet);
    return elt;
}


bool est_dans_pile(pile p, contenu elt){
    if(!est_vide_pile(p)){
        if(p->valeur == elt){
            return true;
        }
        return est_dans_pile(p->suivant, elt);
    }    
    else{
        return false;
    } 
}

graphe make_graph(miroirs* tab,  int taille_l, int taille_c, int taille_t){ //crée le graphe d'adjacence en O(n²)
    int plus_proche_gauche[2];
    int plus_proche_droite[2];
    int plus_proche_haut[2];
    int plus_proche_bas[2];
    graphe g = malloc(4*taille_t*sizeof(miroirs) + sizeof(miroirs)); // Les taille_t miroirs et la zone de départ
    for(int i = 0; i < taille_t; i += 1){
        plus_proche_gauche[0] = -1; // Le plus proche au départ est le mur. 
        plus_proche_droite[0] = taille_c;
        plus_proche_haut[0] = -1;
        plus_proche_bas[0] = taille_l;
        for(int j = 0; j < taille_t; j += 1){
            if (i != j){
                if (tab[i].x == tab[j].x ){ // on regarde si le miroir n'est pas en contact avec un autre verticalement
                    if(tab[i].y - tab[j].y > 0 && tab[j].y > plus_proche_haut[0]){
                        plus_proche_haut[0] = tab[j].y;
                        plus_proche_haut[1] = j;
                    }
                    if(tab[j].y - tab[i].y > 0 && tab[j].y < plus_proche_bas[0]){
                        plus_proche_bas[0] = tab[j].y;
                        plus_proche_bas[1] = j;
                    }
                }
                if (tab[i].y == tab[j].y){     // de même horizontalement
                    if(tab[j].x - tab[i].x > 0 && tab[j].x < plus_proche_droite[0]){
                        plus_proche_droite[0] = tab[j].x;
                        plus_proche_droite[1] = j;
                    }
                    if(tab[i].x - tab[j].x > 0 && tab[j].x > plus_proche_gauche[0]){
                        plus_proche_gauche[0] = tab[j].x;
                        plus_proche_gauche[1] = j;
                    }
                }
            }
        }
        // Si il y a un miroir proche on le mets en tant que voisin, sinon on dit que le voisin est le mur représenté par {-1;-1}
        // On crée maintenant ses adjacences, au lieu de faire un tableau de tableau, vu que le sous tableau est de taille fixe 4, on fait un tableau linéarisé. 
        if(plus_proche_haut[0] != -1){ 
            g[4*i + 3] = 4*plus_proche_haut[1] + 1;
        }
        else{
            g[4*i + 3] = -1; //par défaut le mur est noté -1.
        }
        if(plus_proche_bas[0] != taille_l){
            g[4*i + 1] = 4*plus_proche_bas[1] + 3;
        }
        else{
            g[4*i + 1] = -1;
        }
        if(plus_proche_gauche[0] != -1){
            g[4*i + 2] = 4*plus_proche_gauche[1];
        }
        else{
            if(tab[i].y == 0){ // Le premier miroir avec lequel on sera en contact
                g[4*taille_t] = 4*i;
            }
            g[4*i + 2] = -1;
        }
        if(plus_proche_droite[0] != taille_c){
            g[4*i] = 4*plus_proche_droite[1] + 2;
        }
        else{
            if(tab[i].y == 0){
                g[4*i] = -2; // position d'arrivée. 
            }
            else{
                g[4*i] = -1;
            }
        }
    }
    return g;
}

int parcours_dfs(graphe g, miroirs* tab, pile* vus, int pos, int taille){
    if (pos == 4*taille){ //Si on est au début, on parcours vers la première valeur du graphe. 
        empiler(vus, pos);
        int res = parcours_dfs(g, tab, vus, g[pos], taille);
        return res;
    }
    else if (pos == -1){ //on s'arrête si on est dans un mur
        return 0;
    }
    else if (pos == -2){ //on s'arrête et ajoute 1 si on touche à la fin
        return 1;
    }
    else if(est_dans_pile(*vus, pos)){ // dans un parcours de laser, deux lasers ne peuvent pas se chevaucher, le chemin est incorrect
        return 0;
    }
    else{
        empiler(vus, pos); //on ajoute dans la pile les deux sens possibles, (on ne revient jamais en arrière !) 
        empiler(vus, g[pos]);
        if(pos%2 == 1){ 
            int nb_chemins = parcours_dfs(g, tab, vus, g[(pos - pos%4) + 2], taille); 
            nb_chemins += parcours_dfs(g, tab, vus, g[(pos - pos%4)], taille);
            depiler(vus);
            depiler(vus);
            return nb_chemins;
        }
        else{
            int nb_chemins = parcours_dfs(g, tab, vus, g[(pos - pos%4) + 3], taille);
            nb_chemins += parcours_dfs(g, tab, vus, g[(pos - pos%4) + 1], taille);
            depiler(vus);
            depiler(vus);
            return nb_chemins;
        }
    }
}

int vfinale(int taille[], miroirs tab[]){
    graphe gr0 = make_graph(tab, taille[0], taille[1], taille[2]);
    pile p = creer_pile();
    int res = parcours_dfs(gr0, tab, &p, 4*taille[2], taille[2]);
    free(gr0);
    free(p);
    return res;
}

// méthode naïve

void supprime_pile(pile p){
    if (!est_vide_pile(p)){
        supprime_pile(p->suivant);
        free(p);
    }
}

bool change_position(bool* position, int taille){
    int ind = taille - 1;
    while(true && ind >= 0 ){
        if(position[ind]){
            position[ind] = false;
        }
        else{
            position[ind] = true; 
            return false;
        }
        ind -= 1;
    }
    return true; // on a atteint la dernière position, le tableau est remplit de true
}
bool compare_pile(pile chemin1, pile chemin2){
    if(!est_vide_pile(chemin1) && est_vide_pile(chemin2)){
        return false;
    }
    if(!est_vide_pile(chemin1) && est_vide_pile(chemin2)){
        return false;
    }
    if(!est_vide_pile(chemin1) && !est_vide_pile(chemin2)){
        if(chemin1->valeur == chemin2->valeur){
            return compare_pile(chemin1->suivant, chemin2->suivant);
        }
        return false;
    }
    return true;
}

int** cree_parcours(miroirs* tab, int taille_l, int taille_c, int taille_m, bool* position){
    int** parcours_cree = malloc(taille_l * sizeof(int*));
    for (int i = 0; i < taille_l; i+= 1){
        parcours_cree[i] = malloc(taille_c * sizeof(int));
        for (int j = 0; j < taille_c; j += 1){
            parcours_cree[i][j] = 0;
        }
    }
    
    for (int i = 0; i < taille_m; i+=1){
        if(position[i]){
            parcours_cree[tab[i].y][tab[i].x] = -1;
        }
        else{
            parcours_cree[tab[i].y][tab[i].x] = -2;
        }
    }
    return parcours_cree;
}

void supprime_parcours(int** parcours, int taille_l){
    for (int i = 0; i <taille_l; i+=1){
        free(parcours[i]);
    }
    free(parcours);
}

pile parcours_grille(int taille_l, int taille_c, int** parcours){
    char sens ='E'; // on commence en allant à l'est
    int i = 0; // et en haut à gauche du tableau
    int j = 0;
    pile p = creer_pile();
    while(i >= 0 && j >= 0 && i < taille_l  && j < taille_c){
        empiler(&p, i);
        empiler(&p, j);
        if(parcours[i][j] == -1){ //cas où on croise un miroir de ce type : / 
            if(sens == 'N'){
                sens = 'E';
            }
            else if(sens == 'S'){
                sens = 'O';
            }
            else if(sens == 'E'){
                sens = 'N';
            }
            else{
                sens = 'S';
            }
        }
        else if(parcours[i][j] == -2){ // cas où on croise un miroir de l'autre sens
            if(sens == 'N'){
                sens = 'O';
            }
            else if(sens == 'S'){
                sens = 'E';
            }
            else if(sens == 'E'){
                sens = 'S';
            }
            else{
                sens = 'N';
            }
        }
        // maintenant on se déplace à la case suivante
        if (sens == 'N'){
            i -= 1;
        }
        else if (sens == 'E'){
            j += 1;
        }
        else if (sens == 'S'){
            i += 1;
        }
        else{
            j -= 1;
        }     
    }
    empiler(&p, i);
    empiler(&p, j);
    return p;
}

pile_solution creer_pile_solution(){
    return NULL;
}
bool est_vide_pile_solution(pile_solution p){
    return p == NULL;
}
void empiler_solution(pile_solution* p, pile elt) {
    maillon_solution* m = malloc(sizeof(maillon_solution));
    assert(m != NULL);
    m->valeur = elt;
    m->suivant = *p;
    *p = m;
}

pile depiler_solution(pile_solution* p) {
    assert(!est_vide_pile_solution(*p));
    maillon_solution* sommet = *p;
    pile elt = sommet->valeur;
    *p = sommet->suivant;
    free(sommet);
    return elt;
}

bool est_dans_pile_solution(pile_solution p, pile elt){
    if(!est_vide_pile_solution(p)){
        if(compare_pile(p->valeur, elt)){
            return true;
        }
        return est_dans_pile_solution(p->suivant, elt);
    }    
    else{
        return false;
    } 
}

int taille_pile_solution(pile_solution* p){
    int t = 0;
    while(!est_vide_pile_solution(*p)){
        t+=1;
        depiler_solution(p);
    }
    return t;
}
int compte_solutions_differents(pile_solution* p){
    pile_solution q = creer_pile_solution();
    while (!est_vide_pile_solution(*p)){
        pile chem = depiler_solution(p);
        if(!est_dans_pile_solution(*p, chem)){
            empiler_solution(&q, chem);
        }
        else{
            supprime_pile(chem);
        }
    }
    int t = taille_pile_solution(&q);
    free(q);
    return t;
}
int v1(int taille[], miroirs tab[]){
    bool* position = malloc(taille[2]*sizeof(bool));
    for (int i = 0; i < taille[2]; i+= 1){
        position[i] = false; 
    }
    pile_solution p = creer_pile_solution();
    while(true){
        int** parcours = cree_parcours(tab, taille[0], taille[1], taille[2], position);
        if (change_position(position, taille[2])){
            break; 
        }
        pile chem = parcours_grille(taille[0],taille[1],parcours);
        //printf("%d,%d", chem->suivant->valeur == 0, chem->valeur == taille[1]);
        if(chem->suivant->valeur == 0 && chem->valeur == taille[1]){
            empiler_solution(&p, chem);
        }
        else{
            supprime_pile(chem);
        }
    }
    int nb_solutions = compte_solutions_differents(&p);
    free(position);
    free(p);
    return nb_solutions;
}

int main(int argc, char* argv[]){
    assert(argc == 2);
    FILE* informations = fopen(argv[1], "r");
    int taille[3];
    fscanf(informations, "%d%d%d", &taille[0],&taille[1],&taille[2]); //récupère respectivement le nombre de lignes, de colonnes, et de miroirs
    miroirs* tab = malloc(taille[2] * sizeof(miroirs));
    for(int i = 0; i < taille[2]; i+=1){
        fscanf(informations,"%d,%d", &tab[i].y, &tab[i].x);
    }
    fclose(informations);
    clock_t debut = clock();
    printf("Il y a %d chemins.\n", vfinale(taille, tab));
    long double elapsed_ms = (clock() - debut)/(CLOCKS_PER_SEC/1000);
    printf("Résultat obtenu en : %Lfms\n", elapsed_ms);
    debut = clock();
    printf("Il y a %d chemins avec la méthode naïve.\n", v1(taille, tab));
    elapsed_ms = (clock() - debut)/(CLOCKS_PER_SEC/1000);
    printf("Résultat obtenu en : %Lfms\n", elapsed_ms);
    free(tab);
    return 0; 
}