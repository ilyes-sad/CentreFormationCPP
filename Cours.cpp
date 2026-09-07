#include "Cours.h"

#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

Cours::Cours()
    : m_id(-1)
    , m_formateurId(-1)
    , m_duree(1)
    , m_dateDebut(QDate::currentDate())
    , m_dateFin(QDate::currentDate())
    , m_heureDebut(9, 0)
    , m_heureFin(17, 0)
    , m_capacite(1)
{
}

Cours::Cours(int id,
             const QString &intitule,
             const QString &categorie,
             const QString &niveau,
             const QString &formateur,
             int duree,
             const QDate &dateDebut,
             const QDate &dateFin,
             int capacite,
             const QString &programme,
             const QTime &heureDebut,
             const QTime &heureFin)
    : m_id(id)
    , m_intitule(intitule)
    , m_categorie(categorie)
    , m_niveau(niveau)
    , m_formateur(formateur)
    , m_duree(duree)
    , m_dateDebut(dateDebut)
    , m_dateFin(dateFin)
    , m_heureDebut(heureDebut)
    , m_heureFin(heureFin)
    , m_capacite(capacite)
    , m_programme(programme)
{
}

int Cours::id() const { return m_id; }
QString Cours::intitule() const { return m_intitule; }
QString Cours::categorie() const { return m_categorie; }
QString Cours::niveau() const { return m_niveau; }
QString Cours::formateur() const { return m_formateur; }
int Cours::idFormateur() const { return m_formateurId; }
int Cours::duree() const { return m_duree; }
QDate Cours::dateDebut() const { return m_dateDebut; }
QDate Cours::dateFin() const { return m_dateFin; }
QTime Cours::heureDebut() const { return m_heureDebut; }
QTime Cours::heureFin() const { return m_heureFin; }
int Cours::capacite() const { return m_capacite; }
QString Cours::programme() const { return m_programme; }

void Cours::setId(int id) { m_id = id; }
void Cours::setIntitule(const QString &intitule) { m_intitule = intitule; }
void Cours::setCategorie(const QString &categorie) { m_categorie = categorie; }
void Cours::setNiveau(const QString &niveau) { m_niveau = niveau; }
void Cours::setFormateur(const QString &formateur) { m_formateur = formateur; }
void Cours::setIdFormateur(int idFormateur) { m_formateurId = idFormateur; }
void Cours::setDuree(int duree) { m_duree = duree; }
void Cours::setDateDebut(const QDate &dateDebut) { m_dateDebut = dateDebut; }
void Cours::setDateFin(const QDate &dateFin) { m_dateFin = dateFin; }
void Cours::setHeureDebut(const QTime &heureDebut) { m_heureDebut = heureDebut; }
void Cours::setHeureFin(const QTime &heureFin) { m_heureFin = heureFin; }
void Cours::setCapacite(int capacite) { m_capacite = capacite; }
void Cours::setProgramme(const QString &programme) { m_programme = programme; }

bool Cours::createTable(const QSqlDatabase &database)
{
    if (!database.isOpen()) {
        return false;
    }

    QString sql = "CREATE TABLE COURS ("
                  "ID_COURS NUMBER PRIMARY KEY, "
                  "INTITULE VARCHAR2(150), "
                  "CATEGORIE VARCHAR2(100), "
                  "NIVEAU VARCHAR2(50), "
                  "ID_FORMATEUR NUMBER, "
                  "DUREE_HEURES NUMBER, "
                  "DATE_DEBUT DATE, "
                  "DATE_FIN DATE, "
                  "HEURE_DEBUT NUMBER DEFAULT 540, "
                  "HEURE_FIN NUMBER DEFAULT 1020, "
                  "CAPACITE NUMBER, "
                  "PROGRAMME CLOB)";
    QSqlQuery query(database);
    return query.exec(sql);
}

bool Cours::insert(const QSqlDatabase &database) const
{
    if (!database.isOpen()) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare("INSERT INTO COURS (INTITULE, CATEGORIE, NIVEAU, ID_FORMATEUR, DUREE_HEURES, DATE_DEBUT, DATE_FIN, HEURE_DEBUT, HEURE_FIN, CAPACITE, PROGRAMME) "
                  "VALUES (:intitule, :categorie, :niveau, :formateurId, :duree, :dateDebut, :dateFin, :heureDebut, :heureFin, :capacite, :programme)");
    query.bindValue(":intitule", m_intitule);
    query.bindValue(":categorie", m_categorie);
    query.bindValue(":niveau", m_niveau);
    query.bindValue(":formateurId", m_formateurId);
    query.bindValue(":duree", m_duree);
    query.bindValue(":dateDebut", m_dateDebut);
    query.bindValue(":dateFin", m_dateFin);
    query.bindValue(":heureDebut", m_heureDebut.hour() * 60 + m_heureDebut.minute());
    query.bindValue(":heureFin", m_heureFin.hour() * 60 + m_heureFin.minute());
    query.bindValue(":capacite", m_capacite);
    query.bindValue(":programme", m_programme);

    bool ok = query.exec();
    if (!ok) {
        qWarning() << "Cours insert failed:" << query.lastError().text();
    }
    return ok;
}

bool Cours::update(const QSqlDatabase &database) const
{
    if (!database.isOpen() || m_id < 0) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare("UPDATE COURS SET INTITULE = :intitule, CATEGORIE = :categorie, NIVEAU = :niveau, ID_FORMATEUR = :formateurId, "
                  "DUREE_HEURES = :duree, DATE_DEBUT = :dateDebut, DATE_FIN = :dateFin, HEURE_DEBUT = :heureDebut, HEURE_FIN = :heureFin, "
                  "CAPACITE = :capacite, PROGRAMME = :programme "
                  "WHERE ID_COURS = :id");
    query.bindValue(":intitule", m_intitule);
    query.bindValue(":categorie", m_categorie);
    query.bindValue(":niveau", m_niveau);
    query.bindValue(":formateurId", m_formateurId);
    query.bindValue(":duree", m_duree);
    query.bindValue(":dateDebut", m_dateDebut);
    query.bindValue(":dateFin", m_dateFin);
    query.bindValue(":heureDebut", m_heureDebut.hour() * 60 + m_heureDebut.minute());
    query.bindValue(":heureFin", m_heureFin.hour() * 60 + m_heureFin.minute());
    query.bindValue(":capacite", m_capacite);
    query.bindValue(":programme", m_programme);
    query.bindValue(":id", m_id);

    bool ok = query.exec();
    if (!ok) {
        qWarning() << "Cours update failed:" << query.lastError().text();
    }
    return ok;
}

bool Cours::remove(const QSqlDatabase &database) const
{
    if (!database.isOpen() || m_id < 0) {
        return false;
    }

    QSqlQuery query(database);
    query.prepare("DELETE FROM COURS WHERE ID_COURS = :id");
    query.bindValue(":id", m_id);

    bool ok = query.exec();
    if (!ok) {
        qWarning() << "Cours delete failed:" << query.lastError().text();
    }
    return ok;
}

bool Cours::ensureAutoIncrement(const QSqlDatabase &database)
{
    if (!database.isOpen()) {
        return false;
    }

    QSqlQuery query(database);
    if (!query.exec("SELECT SEQUENCE_NAME FROM USER_SEQUENCES WHERE SEQUENCE_NAME = 'SEQ_COURS'")) {
        qWarning() << "Unable to check COURS sequence:" << query.lastError().text();
        return false;
    }
    if (query.next()) {
        return true;
    }

    int startValue = 1;
    if (query.exec("SELECT NVL(MAX(ID_COURS), 0) + 1 FROM COURS")) {
        if (query.next()) {
            startValue = query.value(0).toInt();
        }
    }

    QString seqSql = QString("CREATE SEQUENCE SEQ_COURS START WITH %1 INCREMENT BY 1").arg(startValue);
    if (!query.exec(seqSql)) {
        qWarning() << "Unable to create COURS sequence:" << query.lastError().text();
        return false;
    }

    QString trigSql =
        "CREATE OR REPLACE TRIGGER TRG_COURS_ID "
        "BEFORE INSERT ON COURS "
        "FOR EACH ROW "
        "BEGIN "
        "SELECT SEQ_COURS.NEXTVAL INTO :NEW.ID_COURS FROM DUAL; "
        "END;";
    if (!query.exec(trigSql)) {
        qWarning() << "Unable to create COURS trigger:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Cours::ensureTimeColumns(const QSqlDatabase &database)
{
    if (!database.isOpen()) {
        return false;
    }

    QStringList columns;
    QSqlQuery query(database);
    if (query.exec("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'COURS'")) {
        while (query.next()) {
            columns.append(query.value(0).toString().toUpper());
        }
    } else {
        qWarning() << "Unable to inspect COURS columns:" << query.lastError().text();
        return false;
    }

    bool ok = true;
    if (!columns.contains("HEURE_DEBUT")) {
        if (!query.exec("ALTER TABLE COURS ADD HEURE_DEBUT NUMBER DEFAULT 540")) {
            qWarning() << "ALTER COURS ADD HEURE_DEBUT failed:" << query.lastError().text();
            ok = false;
        }
    }
    if (!columns.contains("HEURE_FIN")) {
        if (!query.exec("ALTER TABLE COURS ADD HEURE_FIN NUMBER DEFAULT 1020")) {
            qWarning() << "ALTER COURS ADD HEURE_FIN failed:" << query.lastError().text();
            ok = false;
        }
    }
    return ok;
}
