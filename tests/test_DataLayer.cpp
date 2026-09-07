#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDate>
#include <QTime>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QtGlobal>
#include <cstdio>

#include "Formateur.h"
#include "Cours.h"

static int g_fail = 0;
static int g_pass = 0;

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        if (cond) {                                                             \
            ++g_pass;                                                           \
            printf("PASS: %s\n", msg);                                          \
        } else {                                                                \
            ++g_fail;                                                           \
            printf("FAIL: %s\n", msg);                                          \
        }                                                                       \
    } while (0)

static const QString MARKER_EMAIL = "test.datalayer@example.com";
static const QString MARKER_COURS = "TEST DataLayer Cours";

static int findFormateurId(const QSqlDatabase &db, const QString &email)
{
    QSqlQuery q(db);
    q.prepare("SELECT ID_FORMATEUR FROM FORMATEUR WHERE EMAIL = :email");
    q.bindValue(":email", email);
    if (!q.exec() || !q.next()) return -1;
    return q.value(0).toInt();
}

static void testSchema(QSqlDatabase &db)
{
    QStringList tables = db.tables(QSql::Tables);
    CHECK(tables.contains("FORMATEUR"), "Table FORMATEUR existe");
    CHECK(tables.contains("COURS"), "Table COURS existe");

    QSqlQuery q(db);
    CHECK(q.exec("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'COURS'"), "Lecture des colonnes de COURS");
    QStringList cols;
    while (q.next()) cols << q.value(0).toString().toUpper();
    CHECK(cols.contains("HEURE_DEBUT"), "Colonne COURS.HEURE_DEBUT presente");
    CHECK(cols.contains("HEURE_FIN"), "Colonne COURS.HEURE_FIN presente");

    CHECK(q.exec("SELECT SEQUENCE_NAME FROM USER_SEQUENCES WHERE SEQUENCE_NAME = 'SEQ_FORMATEUR'"), "Sequence SEQ_FORMATEUR presente");
    CHECK(q.next(), "SEQ_FORMATEUR existe");
    CHECK(q.exec("SELECT SEQUENCE_NAME FROM USER_SEQUENCES WHERE SEQUENCE_NAME = 'SEQ_COURS'"), "Sequence SEQ_COURS presente");
    CHECK(q.next(), "SEQ_COURS existe");
}

static void testFormateurCrud(QSqlDatabase &db)
{
    // cleanup residue from a previous run
    int oldId = findFormateurId(db, MARKER_EMAIL);
    if (oldId > 0) {
        QSqlQuery clean(db);
        clean.prepare("DELETE FROM FORMATEUR WHERE EMAIL = :email");
        clean.bindValue(":email", MARKER_EMAIL);
        clean.exec();
    }

    Formateur f(-1, "Test", "DataLayer", MARKER_EMAIL, "1234567890",
                "IA", QDate(2026, 1, 1), "Actif");
    CHECK(f.insert(db), "Formateur::insert reussi");

    int id = findFormateurId(db, MARKER_EMAIL);
    CHECK(id > 0, "Formateur recupere par email (id > 0)");

    if (id > 0) {
        Formateur upd(id, "TestModifie", "DataLayer", MARKER_EMAIL, "0987654321",
                      "IA", QDate(2026, 2, 2), "Inactif");
        CHECK(upd.update(db), "Formateur::update reussi");

        QSqlQuery q(db);
        q.prepare("SELECT NOM, PRENOM, TELEPHONE, STATUS, TO_CHAR(DATE_EMBAUCHE,'YYYY-MM-DD') "
                  "FROM FORMATEUR WHERE ID_FORMATEUR = :id");
        q.bindValue(":id", id);
        CHECK(q.exec() && q.next(), "SELECT formateur apres update");

        QSqlQuery r(db);
        r.prepare("SELECT NOM, PRENOM, TELEPHONE, STATUS, TO_CHAR(DATE_EMBAUCHE,'YYYY-MM-DD') "
                  "FROM FORMATEUR WHERE ID_FORMATEUR = :id");
        r.bindValue(":id", id);
        if (r.exec() && r.next()) {
            CHECK(r.value(0).toString() == "TestModifie", "NOM mis a jour");
            CHECK(r.value(1).toString() == "DataLayer", "PRENOM inchange");
            CHECK(r.value(2).toString() == "0987654321", "TELEPHONE mis a jour");
            CHECK(r.value(3).toString() == "Inactif", "STATUS mis a jour");
            CHECK(r.value(4).toString() == "2026-02-02", "DATE_EMBAUCHE mise a jour");
        } else {
            CHECK(false, "Relecture formateur mise a jour");
        }

        CHECK(upd.remove(db), "Formateur::remove reussi");
        CHECK(findFormateurId(db, MARKER_EMAIL) < 0, "Formateur supprime de la base");
    }
}

static void testCoursCrud(QSqlDatabase &db)
{
    // ensure a formateur exists for FK
    int fid = findFormateurId(db, MARKER_EMAIL);
    if (fid <= 0) {
        Formateur f(-1, "Test", "DataLayer", MARKER_EMAIL, "1234567890",
                    "IA", QDate(2026, 1, 1), "Actif");
        if (!f.insert(db)) {
            CHECK(false, "Insertion formateur de secours pour testCoursCrud");
            return;
        }
        fid = findFormateurId(db, MARKER_EMAIL);
    }

    // cleanup residue
    QSqlQuery clean(db);
    clean.prepare("DELETE FROM COURS WHERE INTITULE = :intitule");
    clean.bindValue(":intitule", MARKER_COURS);
    clean.exec();

    Cours c(-1, MARKER_COURS, "IA", "Debutant", QString::number(fid), 3,
            QDate(2026, 12, 1), QDate(2026, 12, 1), 25, "Programme test",
            QTime(9, 0), QTime(12, 0));
    c.setIdFormateur(fid);
    CHECK(c.insert(db), "Cours::insert reussi");

    QSqlQuery q(db);
    q.prepare("SELECT ID_COURS, ID_FORMATEUR, HEURE_DEBUT, HEURE_FIN, CAPACITE "
              "FROM COURS WHERE INTITULE = :intitule");
    q.bindValue(":intitule", MARKER_COURS);
    CHECK(q.exec() && q.next(), "SELECT cours insere");
    int coursId = -1;
    if (q.isValid()) {
        coursId = q.value(0).toInt();
        CHECK(q.value(1).toInt() == fid, "ID_FORMATEUR correct");
        CHECK(q.value(2).toInt() == 540, "HEURE_DEBUT stocke en minutes (09:00 = 540)");
        CHECK(q.value(3).toInt() == 720, "HEURE_FIN stocke en minutes (12:00 = 720)");
        CHECK(q.value(4).toInt() == 25, "CAPACITE correcte");
    } else {
        CHECK(false, "Lecture du cours insere");
    }

    if (coursId > 0) {
        Cours upd(coursId, MARKER_COURS + " modifie", "IA", "Avance", QString::number(fid), 5,
                  QDate(2026, 12, 2), QDate(2026, 12, 4), 10, "Programme modifie",
                  QTime(14, 0), QTime(17, 0));
        upd.setIdFormateur(fid);
        CHECK(upd.update(db), "Cours::update reussi");

        QSqlQuery r(db);
        r.prepare("SELECT INTITULE, DUREE_HEURES, TO_CHAR(DATE_DEBUT,'YYYY-MM-DD'), TO_CHAR(DATE_FIN,'YYYY-MM-DD'), "
                  "HEURE_DEBUT, HEURE_FIN, CAPACITE FROM COURS WHERE ID_COURS = :id");
        r.bindValue(":id", coursId);
        if (r.exec() && r.next()) {
            CHECK(r.value(0).toString() == MARKER_COURS + " modifie", "INTITULE mis a jour");
            CHECK(r.value(1).toInt() == 5, "DUREE mise a jour");
            CHECK(r.value(2).toString() == "2026-12-02", "DATE_DEBUT mise a jour");
            CHECK(r.value(3).toString() == "2026-12-04", "DATE_FIN mise a jour");
            CHECK(r.value(4).toInt() == 840, "HEURE_DEBUT 14:00 = 840");
            CHECK(r.value(5).toInt() == 1020, "HEURE_FIN 17:00 = 1020");
            CHECK(r.value(6).toInt() == 10, "CAPACITE mise a jour");
        } else {
            CHECK(false, "Relecture cours mis a jour");
        }

        CHECK(upd.remove(db), "Cours::remove reussi");

        QSqlQuery gone(db);
        gone.prepare("SELECT COUNT(*) FROM COURS WHERE ID_COURS = :id");
        gone.bindValue(":id", coursId);
        gone.exec();
        gone.next();
        CHECK(gone.value(0).toInt() == 0, "Cours supprime de la base");
    }
}

static void testConflictDetection(QSqlDatabase &db)
{
    int fid = findFormateurId(db, MARKER_EMAIL);
    if (fid <= 0) {
        CHECK(false, "Formateur absent pour testConflictDetection");
        return;
    }

    // base course 2026-12-10 09:00 -> 12:00
    Cours base(-1, "TEST Conflict Base", "IA", "Debutant", QString::number(fid), 3,
               QDate(2026, 12, 10), QDate(2026, 12, 10), 20, "x", QTime(9, 0), QTime(12, 0));
    base.setIdFormateur(fid);
    if (!base.insert(db)) {
        CHECK(false, "Insertion cours base pour conflit");
        return;
    }
    QSqlQuery bq(db);
    bq.prepare("SELECT ID_COURS FROM COURS WHERE INTITULE = :intitule");
    bq.bindValue(":intitule", QString("TEST Conflict Base"));
    bq.exec();
    int baseId = bq.next() ? bq.value(0).toInt() : -1;

    auto countConflicts = [&](const QDate &dd, const QTime &hd, const QDate &df, const QTime &hf, int excl) -> int {
        QSqlQuery q(db);
        q.prepare("SELECT COUNT(*) FROM COURS WHERE ID_FORMATEUR = :fid AND ID_COURS <> :excl AND "
                  "TO_DATE(:dd,'YYYY-MM-DD') + :hd/1440 < DATE_FIN + COALESCE(HEURE_FIN, 1439)/1440 AND "
                  "DATE_DEBUT + COALESCE(HEURE_DEBUT, 0)/1440 < TO_DATE(:df,'YYYY-MM-DD') + :hf/1440");
        q.bindValue(":fid", fid);
        q.bindValue(":excl", excl);
        q.bindValue(":dd", dd.toString(Qt::ISODate));
        q.bindValue(":hd", hd.hour() * 60 + hd.minute());
        q.bindValue(":df", df.toString(Qt::ISODate));
        q.bindValue(":hf", hf.hour() * 60 + hf.minute());
        if (!q.exec()) {
            printf("DIAG conflict qry ERR: %s\n", qPrintable(q.lastError().text()));
            fflush(stdout);
            return -1;
        }
        q.next();
        return q.value(0).toInt();
    };

    // a new cours being saved has no id yet -> excl = 0
    CHECK(countConflicts(QDate(2026, 12, 10), QTime(10, 0), QDate(2026, 12, 10), QTime(11, 0), 0) == 1,
          "Conflit detecte pour creneau qui chevauche 09:00-12:00");
    CHECK(countConflicts(QDate(2026, 12, 11), QTime(9, 0), QDate(2026, 12, 11), QTime(12, 0), 0) == 0,
          "Aucun conflit pour un jour different");
    CHECK(countConflicts(QDate(2026, 12, 10), QTime(13, 0), QDate(2026, 12, 10), QTime(17, 0), 0) == 0,
          "Aucun conflit pour creneau 13:00-17:00");
    CHECK(countConflicts(QDate(2026, 12, 10), QTime(12, 0), QDate(2026, 12, 10), QTime(14, 0), 0) == 0,
          "Aucun conflit pour creneau adjacent 12:00-14:00");

    QSqlQuery del(db);
    del.prepare("DELETE FROM COURS WHERE ID_COURS = :id");
    del.bindValue(":id", baseId);
    del.exec();
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &ctx, const QString &msg) {
        Q_UNUSED(ctx);
        printf("[QT] %s\n", qPrintable(msg));
        fflush(stdout);
    });

    printf("=== Test de la couche donnees (Formateur / Cours) ===\n");
    fflush(stdout);

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "dataLayerTest");
    db.setDatabaseName("Driver={Oracle in XE};Dbq=XE;Uid=Ilyess;Pwd=0000;");
    if (!db.open()) {
        printf("FAIL: Impossible de se connecter a Oracle: %s\n", qPrintable(db.lastError().text()));
        return 1;
    }
    printf("Connexion Oracle OK\n");
    fflush(stdout);

    testSchema(db);
    testFormateurCrud(db);
    testCoursCrud(db);
    testConflictDetection(db);

    // final cleanup of test formateur
    int fid = findFormateurId(db, MARKER_EMAIL);
    if (fid > 0) {
        QSqlQuery q(db);
        q.prepare("DELETE FROM FORMATEUR WHERE ID_FORMATEUR = :id");
        q.bindValue(":id", fid);
        q.exec();
    }

    db.close();

    printf("---------------------------------------------\n");
    printf("Resultats: %d passe(s), %d echec(s)\n", g_pass, g_fail);
    fflush(stdout);
    return g_fail == 0 ? 0 : 2;
}
