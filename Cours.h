#ifndef COURS_H
#define COURS_H

#include <QString>
#include <QDate>
#include <QTime>

class QSqlDatabase;

class Cours
{
public:
    Cours();
    Cours(int id,
          const QString &intitule,
          const QString &categorie,
          const QString &niveau,
          const QString &formateur,
          int duree,
          const QDate &dateDebut,
          const QDate &dateFin,
          int capacite,
          const QString &programme,
          const QTime &heureDebut = QTime(9, 0),
          const QTime &heureFin = QTime(17, 0));

    int id() const;
    QString intitule() const;
    QString categorie() const;
    QString niveau() const;
    QString formateur() const;
    int idFormateur() const;
    int duree() const;
    QDate dateDebut() const;
    QDate dateFin() const;
    QTime heureDebut() const;
    QTime heureFin() const;
    int capacite() const;
    QString programme() const;

    void setId(int id);
    void setIntitule(const QString &intitule);
    void setCategorie(const QString &categorie);
    void setNiveau(const QString &niveau);
    void setFormateur(const QString &formateur);
    void setIdFormateur(int idFormateur);
    void setDuree(int duree);
    void setDateDebut(const QDate &dateDebut);
    void setDateFin(const QDate &dateFin);
    void setHeureDebut(const QTime &heureDebut);
    void setHeureFin(const QTime &heureFin);
    void setCapacite(int capacite);
    void setProgramme(const QString &programme);

    bool insert(const QSqlDatabase &database) const;
    bool update(const QSqlDatabase &database) const;
    bool remove(const QSqlDatabase &database) const;
    static bool createTable(const QSqlDatabase &database);
    static bool ensureAutoIncrement(const QSqlDatabase &database);
    static bool ensureTimeColumns(const QSqlDatabase &database);

private:
    int m_id;
    QString m_intitule;
    QString m_categorie;
    QString m_niveau;
    QString m_formateur;
    int m_formateurId;
    int m_duree;
    QDate m_dateDebut;
    QDate m_dateFin;
    QTime m_heureDebut;
    QTime m_heureFin;
    int m_capacite;
    QString m_programme;
};

#endif // COURS_H
