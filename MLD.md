# MLD - Modèle Logique de Données

## Table FORMATEUR
FORMATEUR (
    id_formateur PK,
    nom,
    prenom,
    email,
    telephone,
    specialite,
    date_embauche,
    status
)

## Table COURS
COURS (
    id_cours PK,
    intitule,
    categorie,
    niveau,
    duree_heures,
    date_debut,
    date_fin,
    heure_debut,
    heure_fin,
    capacite,
    programme,
    id_formateur FK -> FORMATEUR(id_formateur)
)

## Relation
- Un formateur peut enseigner plusieurs cours.
- Un cours est associé à un seul formateur.
