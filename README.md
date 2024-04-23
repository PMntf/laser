Ce projet consiste en une résolution algorthmique d'un problème inspiré de l'exercice "laser" du concours castor 2014.\

On dispose d'une grille composée de miroirs, d'un faisceau qui démarre en haut à gauche de la grille et d'un capteur en haut à droite.\
L'objectif est de dénombrer le nombre de solutions valides possible. \\
Une solution est un fonction qui à un miroir lui associe sa rotation, cette solution est valide si le laser atteint le capteur avec cette disposition.\

Pour exécuter cet algorithme il est nécessaire de compiler le fichier et de l'éxécuter par la commande ./*nom_du_fichier_* grille \
où grille est un fichier représentant l'entrée du problème. Il sera construit de la sorte : \
La première ligne est de la forme : *nombre_de_lignes* *nombre_de_colonnes* *nombre_de_miroirs* \
Les suivantes seront les positions des miroirs de la forme : *ligne*,*colonne* \

Le code sera composé de deux algorithmes distints.
Le premier est la solution optimale que j'ai trouvé, utilisant une représentation du problème par un graphe et réalisant un parcours dfs de celui-ci pour arriver au résultat. 
Le second est une solution naïve pour comparer le résultat avec une solution de force brut. 
