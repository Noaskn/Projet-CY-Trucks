Bienvenue dans le Projet CY-Trucks de Emma Danan et Noa Sakoun

Le principe de ce programme est d'analyser un fichier de données de transport routier. Pour cela, 5 traitements vous sont proposés :

-d1 : 10 conducteurs avec le plus de trajets (tri par ordre décroissant)
 
-d2 : 10 conducteurs ayant parcouru la plus longue distance (tri par ordre décroissant)

-l : 10 numéros d'identifiant des trajets qui ont la plus longue distance (tri par ordre décroissant)

-t : 10 villes avec le plus de trajets (tri par ordre alphabétique)

-s : 50 numéros d'identifiant des trajets qui ont la plus grande différence de distance max-min (tri par ordre décroissant)

Utilisation du programme :

Commencez par insérer votre fichier à trier (data.csv) dans le dossier "data" prévu pour cela.

Pour afficher un message d'aide tapez : make help

Pour faire fonctionner le programme tapez : make ARG="type de traitement" (Les différents types de traitement ont été présentés plus haut. Veillez à bien mettre les guillemets et le tiret. Par exemple si vous voulez exécuter le traitement -d1, tapez : make ARG="-d1")

Une fois le traitement exécuté, vous aurez accès au fichier avec les différentes données dans le dossier "temp" qui a été créé. Le graphique sera dans le dossier "images".
