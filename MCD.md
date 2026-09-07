# MCD - Modèle Conceptuel de Données

## Entités

### Formateur
- id_formateur
- nom
- prenom
- email
- telephone
- specialite
- date_embauche
- status

### Cours
- id_cours
- intitule
- categorie
- niveau
- duree_heures
- date_debut
- date_fin
- heure_debut
- heure_fin
- capacite
- programme
- id_formateur

## Relation

Un Formateur peut donner plusieurs Cours.
Un Cours est dispensé par un seul Formateur.

Formateur 1 --- N Cours
