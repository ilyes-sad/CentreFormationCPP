# CentreFormationCPP

## 1. Contexte général

Cette application de gestion de formation a pour objectif de simplifier la gestion des formateurs, des cours, du planning et des alertes métier au sein d’un centre de formation.

Elle permet de centraliser les informations liées à :
- la gestion des formateurs,
- la planification des cours,
- la recherche et le tri des données,
- les statistiques de répartition,
- les alertes sur les conflits et les échéances.

## 2. Problématique

Un centre de formation doit organiser les disponibilités des formateurs, attribuer les cours selon leur spécialité et éviter les chevauchements de planning. Sans outil dédié, ces tâches deviennent difficiles à suivre, subjectives et sujettes aux erreurs.

L’application vise à répondre à ce besoin en offrant une interface graphique Qt permettant de gérer les données dans une base Oracle et d’alerter sur les incohérences métier.

## 3. Solution proposée

Le projet met en place une application C++/Qt avec base de données Oracle XE et interface utilisateur graphique.

### 3.1 Modules principaux

#### Formateur
Attributs principaux :
- identifiant,
- nom,
- prénom,
- email,
- téléphone,
- spécialité,
- date d’embauche,
- statut.

Fonctionnalités : ajout, modification, suppression, recherche, tri, statistique.

#### Cours
Attributs principaux :
- identifiant,
- intitulé,
- catégorie,
- niveau,
- formateur associé,
- durée,
- dates de début et fin,
- horaires de début et fin,
- capacité,
- programme.

Fonctionnalités : ajout, modification, suppression, recherche, tri, validation métier, planning.

## 4. Exigences fonctionnelles

### 4.1 CRUD

Les entités Formateur et Cours implémentent les opérations CRUD de base dans le code source via les classes métiers et les méthodes de la couche de données.

### 4.2 Recherche et tri

L’interface permet :
- recherche de formateurs par nom,
- recherche de cours par intitulé,
- filtres par spécialité, niveau et formateur,
- tri par critère sélectionné.

### 4.3 Statistiques

Une vue de statistiques affiche la répartition des formateurs par spécialité et des cours par catégorie ou niveau à l’aide de diagrammes circulaires Qt Charts.

### 4.4 Planning et alertes

L’application intègre :
- un planning visuel (Gantt),
- la détection des chevauchements de cours pour un même formateur,
- des alertes sur les échéances, les cours en cours, et les formateurs inactifs.

### 4.5 Export

Le projet fournit un export PDF pour les fiches formateurs et les listes de cours.

## 5. Exigences non fonctionnelles

Le projet prend en compte plusieurs aspects non fonctionnels :
- ergonomie de l’interface utilisateur,
- cohérence visuelle via une feuille de style Qt,
- validation métier des données saisies,
- gestion des erreurs SQL et des messages utilisateur,
- fiabilité via les contrôles avant insertion/modification.

## 6. ODD (Objectif de Développement Durable)

### ODD 4 : Éducation de qualité

Le projet contribue à l’ODD 4 en favorisant une meilleure organisation des formations et une gestion plus efficace des ressources pédagogiques. En optimisant la planification des cours et des formateurs, l’application aide à maintenir une qualité de service élevée dans les dispositifs de formation continue.

Cet objectif est directement lié à la mission de gestion des formateurs et des cours dans un centre de formation.

## 7. Conception graphique

Le projet utilise Qt Widgets et une charte visuelle cohérente dans le fichier de style [style.qss](style.qss). Le logo du projet est présent dans le dossier racine du projet et est intégré dans l’interface de l’application.

## 8. Technologie utilisée

- C++17
- Qt 6
- Qt Widgets
- Qt SQL
- Qt Charts
- Oracle XE via ODBC

## 9. Fichiers principaux

- [main.cpp](main.cpp)
- [gformateurcours.cpp](gformateurcours.cpp)
- [gformateurcours.h](gformateurcours.h)
- [Formateur.h](Formateur.h)
- [Formateur.cpp](Formateur.cpp)
- [Cours.h](Cours.h)
- [Cours.cpp](Cours.cpp)
- [database.h](database.h)
- [database.cpp](database.cpp)
- [Project2A.sql](Project2A.sql)
- [style.qss](style.qss)
- [gformateurcours.ui](gformateurcours.ui)

## 10. Conclusion

Le projet répond à un besoin métier concret de gestion de centre de formation avec une application fonctionnelle, évolutive et visuellement cohérente. La principale amélioration restante concerne la documentation formelle et l’extension à d’autres exportations et critères de sécurité.
