#include "gformateurcours.h"
#include "database.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QMessageBox>

namespace {
QString findProjectLogoPath()
{
    const QStringList searchPaths = {
        QDir::currentPath(),
        QCoreApplication::applicationDirPath(),
        QCoreApplication::applicationDirPath() + "/..",
        QCoreApplication::applicationDirPath() + "/../..",
        QCoreApplication::applicationDirPath() + "/../../.."
    };

    for (const QString &basePath : searchPaths) {
        const QString candidate = QDir(basePath).filePath("Logo Centre de formation.jpg");
        if (QFile::exists(candidate)) {
            return candidate;
        }
    }
    return QString();
}
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    const QString logoPath = findProjectLogoPath();
    if (!logoPath.isEmpty()) {
        a.setWindowIcon(QIcon(logoPath));
    }

    Connection db;
    if (!db.createconnect()) {
        QMessageBox::critical(nullptr, "ERREUR CRITIQUE",
                              "Impossible de se connecter à la base Oracle XE !\n"
                              "Vérifiez que le service Oracle est démarré.");
        return -1;
    }

    GFormateurCours w;
    w.show();                     // Affiche la fenêtre
    w.raise();                    // Met au premier plan
    w.activateWindow();           // Focus

    return a.exec();              // L'application reste ouverte jusqu'à fermeture manuelle
}
