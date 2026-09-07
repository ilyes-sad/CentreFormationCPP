#ifndef FORMATEUR_H
#define FORMATEUR_H

#include <QString>
#include <QDate>

class QSqlDatabase;

class Formateur
{
public:
    Formateur();
    Formateur(int id,
              const QString &nom,
              const QString &prenom,
              const QString &email,
              const QString &telephone,
              const QString &specialite,
              const QDate &dateEmbauche,
              const QString &status);

    int id() const;
    QString nom() const;
    QString prenom() const;
    QString email() const;
    QString telephone() const;
    QString specialite() const;
    QDate dateEmbauche() const;
    QString status() const;

    void setId(int id);
    void setNom(const QString &nom);
    void setPrenom(const QString &prenom);
    void setEmail(const QString &email);
    void setTelephone(const QString &telephone);
    void setSpecialite(const QString &specialite);
    void setDateEmbauche(const QDate &dateEmbauche);
    void setStatus(const QString &status);

    bool insert(const QSqlDatabase &database) const;
    bool update(const QSqlDatabase &database) const;
    bool remove(const QSqlDatabase &database) const;
    static bool createTable(const QSqlDatabase &database);
    static bool ensureAutoIncrement(const QSqlDatabase &database);

private:
    int m_id;
    QString m_nom;
    QString m_prenom;
    QString m_email;
    QString m_telephone;
    QString m_specialite;
    QDate m_dateEmbauche;
    QString m_status;
};

#endif // FORMATEUR_H
