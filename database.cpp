
#include "database.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

// Constructeur de la classe Connection.
Connection::Connection() {}

// Cette méthode ouvre la connexion Oracle via ODBC.
// Elle supprime les connexions existantes pour éviter les conflits Qt/SQL.
bool Connection::createconnect()
{
    // Nettoie les anciennes connexions ouvertes par Qt pour ne garder qu'une seule connexion active.
    for (const QString &name : QSqlDatabase::connectionNames())
        QSqlDatabase::removeDatabase(name);

    // Utilisation du pilote ODBC Oracle XE.
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Driver={Oracle in XE};Dbq=XE;Uid=Ilyess;Pwd=0000;");

    // Tente d'ouvrir la connexion Oracle.
    if (!db.open()) {
        qCritical() << "Connexion échouée:" << db.lastError().text();
        return false;
    }

    // Si tout est OK, on affiche un message de confirmation.
    qDebug() << "Connecté à Oracle XE avec succès !";
    return true;
}
