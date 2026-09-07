#include "gformateurcours.h"
#include "ui_gformateurcours.h"
#include "Formateur.h"
#include "Cours.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDate>
#include <QSqlRecord>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QLegend>
#include <QtCharts/QChartView>
#include <QLabel>
#include <QColor>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QPageSize>
#include <QApplication>
#include <QFile>
#include <QRegularExpression>
#include <utility>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QVBoxLayout>
#include <QBrush>
#include <QDir>
#include <QtMath>
#include <functional>

namespace {
// Cherche le logo du projet dans plusieurs chemins possibles.
// Cela permet de lancer l'application depuis le dossier racine ou depuis le dossier de build.
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

// Normalise une valeur pour comparer des libellés de combo-box.
// Exemple : "Réseau & systèmes" devient "reseausystemes" pour comparaison plus fiable.
QString normalizeComboValue(const QString &value)
{
    QString normalized = value.trimmed();
    normalized = normalized.toLower();
    normalized.remove(QRegularExpression("[^a-z0-9]"));
    return normalized;
}

// Sélectionne le bon item dans un QComboBox même si le texte BD diffère légèrement du libellé UI.
void setComboValueSafely(QComboBox *combo, const QString &value)
{
    if (!combo) {
        return;
    }

    const QString text = value.trimmed();
    if (text.isEmpty()) {
        combo->setCurrentIndex(0);
        return;
    }

    // 1) recherche exacte
    int index = combo->findText(text, Qt::MatchExactly);
    if (index < 0) {
        // 2) recherche partielle
        index = combo->findText(text, Qt::MatchContains);
    }

    if (index < 0) {
        // 3) comparaison normalisée pour gérer les accents, & et différences de libellé.
        const QString normalizedValue = normalizeComboValue(text);
        for (int i = 0; i < combo->count(); ++i) {
            const QString itemText = combo->itemText(i);
            const QString normalizedItem = normalizeComboValue(itemText);
            if (normalizedItem == normalizedValue ||
                normalizedItem.contains(normalizedValue) ||
                normalizedValue.contains(normalizedItem)) {
                index = i;
                break;
            }
        }
    }

    if (index >= 0) {
        combo->setCurrentIndex(index);
    } else {
        combo->setCurrentIndex(0);
    }
}
}

// Constructeur principal de la fenêtre de gestion.
// Il initialise l'interface, charge le style, configure le planning, les tables et la connexion.
GFormateurCours::GFormateurCours(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , modelFormateurs(nullptr)
    , modelCours(nullptr)
    , modelSearchFormateurs(nullptr)
    , modelSearchCours(nullptr)
{
    // 1) Charge l'interface générée par Qt Designer.
    ui->setupUi(this);
    setWindowTitle("Gestion Formateur & Cours");
    resize(1100, 700);
    setMinimumSize(1024, 640);

    // 2) Charge le logo depuis le dossier du projet ou du build.
    const QString logoPath = QDir::currentPath() + "/Logo Centre de formation.jpg";
    if (QFile::exists(logoPath)) {
        QPixmap logoPixmap(logoPath);
        if (!logoPixmap.isNull()) {
            QPixmap scaledLogo = logoPixmap.scaled(180, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QLabel *logoLabel = new QLabel(this);
            logoLabel->setPixmap(scaledLogo);
            logoLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            logoLabel->setContentsMargins(0, 4, 12, 4);
            if (auto *layout = qobject_cast<QVBoxLayout *>(ui->centralwidget->layout())) {
                layout->insertWidget(0, logoLabel);
            }
        }
        setWindowIcon(QIcon(logoPath));
    }

    // 3) Initialisation des libellés par défaut des filtres.
    ui->cbFiltreNiveau->setItemText(0, "Tous");
    ui->cbFiltreNiveau->setCurrentIndex(0);
    ui->cbFiltreFormateur->setItemText(0, "Formateur");
    ui->cbTrierParCours->setItemText(0, "Aucun");

    // 4) Charge le thème visuel depuis le fichier style.qss.
    QStringList stylePaths;
    const QString appDir = QCoreApplication::applicationDirPath();
    const QString currentDir = QDir::currentPath();

    stylePaths << appDir + "/style.qss"
               << appDir + "/../style.qss"
               << appDir + "/../../style.qss"
               << appDir + "/../../../style.qss"
               << currentDir + "/style.qss"
               << currentDir + "/../style.qss"
               << currentDir + "/../../style.qss"
               << currentDir + "/../../../style.qss";

    QFile styleFile;
    for (const QString &path : stylePaths) {
        const QString normalizedPath = QDir::cleanPath(path);
        if (QFile::exists(normalizedPath)) {
            styleFile.setFileName(normalizedPath);
            break;
        }
    }

    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        const QString style = QString::fromUtf8(styleFile.readAll());
        qApp->setStyleSheet(style);
        styleFile.close();
    } else {
        qWarning() << "Unable to load style.qss from" << stylePaths;
    }

    // 5) Initialise la base, les modèles tables et les filtres du formulaire.
    initDatabase();
    initModels();
    loadFormateurs();
    loadCours();
    populateFormateurComboBoxes();

    // 6) Connecte les boutons et les champs aux méthodes correspondantes.
    connect(ui->btnAjouterFormateur, &QPushButton::clicked, this, &GFormateurCours::onAjouterFormateur);
    connect(ui->btnModifierFormateur, &QPushButton::clicked, this, &GFormateurCours::onModifierFormateur);
    connect(ui->btnSupprimerFormateur, &QPushButton::clicked, this, &GFormateurCours::onSupprimerFormateur);
    connect(ui->btnViderFormateur, &QPushButton::clicked, this, &GFormateurCours::onViderFormateur);
    connect(ui->btnAjouterCours, &QPushButton::clicked, this, &GFormateurCours::onAjouterCours);
    connect(ui->btnModifierCours, &QPushButton::clicked, this, &GFormateurCours::onModifierCours);
    connect(ui->btnSupprimerCours, &QPushButton::clicked, this, &GFormateurCours::onSupprimerCours);
    connect(ui->btnViderCours, &QPushButton::clicked, this, &GFormateurCours::onViderCours);
    connect(ui->btnRechercher, &QPushButton::clicked, this, &GFormateurCours::onRechercherFormateurs);
    connect(ui->btnRechercherCours, &QPushButton::clicked, this, &GFormateurCours::onRechercherCours);
    connect(ui->btnActualiserStatsFormateurs, &QPushButton::clicked, this, &GFormateurCours::onActualiserStatsFormateurs);
    connect(ui->btnActualiserStatsCours, &QPushButton::clicked, this, &GFormateurCours::onActualiserStatsCours);
    connect(ui->cbStatCoursGroupe, &QComboBox::currentIndexChanged, this, &GFormateurCours::refreshCoursStats);
    connect(ui->btnGenererPdfFormateur, &QPushButton::clicked, this, &GFormateurCours::onGenererPdfFormateur);
    connect(ui->btnGenererDocCours, &QPushButton::clicked, this, &GFormateurCours::onGenererDocCours);
    connect(ui->tvFormateurs->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &GFormateurCours::onFormateurSelected);
    connect(ui->tvFormateurs, &QTableView::doubleClicked, this, &GFormateurCours::onFormateurSelected);
    connect(ui->tvCours->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &GFormateurCours::onCoursSelected);
    connect(ui->tvCours, &QTableView::doubleClicked, this, &GFormateurCours::onCoursSelected);

    // Recherche / tri dynamiques : les résultats se mettent à jour à la saisie.
    connect(ui->leRechercheNom, &QLineEdit::textChanged, this, &GFormateurCours::onRechercherFormateurs);
    connect(ui->cbFiltreSpecialite, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GFormateurCours::onRechercherFormateurs);
    connect(ui->deDateDebut, &QDateEdit::dateChanged, this, &GFormateurCours::onRechercherFormateurs);
    connect(ui->deDateFin, &QDateEdit::dateChanged, this, &GFormateurCours::onRechercherFormateurs);
    connect(ui->cbTrierPar, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GFormateurCours::onRechercherFormateurs);
    connect(ui->checkBoxDesc, &QCheckBox::toggled, this, &GFormateurCours::onRechercherFormateurs);

    connect(ui->leRechercheIntitule, &QLineEdit::textChanged, this, &GFormateurCours::onRechercherCours);
    connect(ui->cbFiltreNiveau, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GFormateurCours::onRechercherCours);
    connect(ui->cbFiltreFormateur, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GFormateurCours::onRechercherCours);
    connect(ui->leDureeMin, &QLineEdit::textChanged, this, &GFormateurCours::onRechercherCours);
    connect(ui->leDureeMax, &QLineEdit::textChanged, this, &GFormateurCours::onRechercherCours);
    connect(ui->cbTrierParCours, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GFormateurCours::onRechercherCours);
    connect(ui->checkBox, &QCheckBox::toggled, this, &GFormateurCours::onRechercherCours);

    // 7) Configure le tableau des alertes pour afficher les priorités, types et détails.
    ui->twAlertes->setColumnCount(3);
    ui->twAlertes->setHorizontalHeaderLabels(QStringList() << "Priorité" << "Type" << "Détail");
    ui->twAlertes->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->twAlertes->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->twAlertes->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->twAlertes->verticalHeader()->setVisible(false);
    ui->twAlertes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->twAlertes->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 8) Crée le widget Gantt qui affiche le planning visuel.
    m_gantt = new GanttWidget(ui->saGanttContents);
    QVBoxLayout *ganttLayout = new QVBoxLayout(ui->saGanttContents);
    ganttLayout->setContentsMargins(0, 0, 0, 0);
    ganttLayout->addWidget(m_gantt);

    connect(ui->btnActualiserPlanning, &QPushButton::clicked, this, &GFormateurCours::onActualiserPlanning);
    connect(ui->btnActualiserAlertes, &QPushButton::clicked, this, &GFormateurCours::onActualiserAlertes);
    connect(ui->cbPlanningFormateur, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GFormateurCours::refreshPlanning);
    connect(ui->cbPlanningVue, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GFormateurCours::refreshPlanning);
    connect(ui->dePlanningJour, &QDateEdit::dateChanged, this, &GFormateurCours::refreshPlanning);

    // 9) Valeurs par défaut des dates du planning et du formulaire.
    ui->dePlanningJour->setDate(QDate::currentDate());
    ui->dePlanningJour->setVisible(false);

    ui->deDateEmbauche->setDate(QDate::currentDate());
    ui->deDateDebut->setDate(QDate::fromString("2000-01-01", Qt::ISODate));
    ui->deDateFin->setDate(QDate::fromString("2099-12-31", Qt::ISODate));
    ui->deDateDebutCours->setDate(QDate::currentDate());
    ui->deDateFinCours->setDate(QDate::currentDate());

    // 10) Charge les données initiales et met à jour l'interface.
    onRechercherFormateurs();
    onRechercherCours();
    refreshFormateurStats();
    refreshCoursStats();
    refreshPlanning();
    refreshAlertes();
}

GFormateurCours::~GFormateurCours()
{
    delete modelFormateurs;
    delete modelCours;
    delete modelSearchFormateurs;
    delete modelSearchCours;
    delete ui;
}

void GFormateurCours::initDatabase()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Erreur de connexion", db.lastError().text());
        return;
    }

    if (!db.tables(QSql::Tables).contains("FORMATEUR")) {
        Formateur::createTable(db);
    }
    Formateur::ensureAutoIncrement(db);

    if (!db.tables(QSql::Tables).contains("COURS")) {
        Cours::createTable(db);
    }
    Cours::ensureTimeColumns(db);
    Cours::ensureAutoIncrement(db);
}

void GFormateurCours::initModels()
{
    modelFormateurs = new QSqlTableModel(this, QSqlDatabase::database());
    modelFormateurs->setTable("FORMATEUR");
    modelFormateurs->select();
    modelFormateurs->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tvFormateurs->setModel(modelFormateurs);
    ui->tvFormateurs->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvFormateurs->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvFormateurs->setColumnHidden(modelFormateurs->fieldIndex("ID_FORMATEUR"), true);

    modelCours = new QSqlTableModel(this, QSqlDatabase::database());
    modelCours->setTable("COURS");
    modelCours->select();
    modelCours->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tvCours->setModel(modelCours);
    ui->tvCours->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvCours->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvCours->setColumnHidden(modelCours->fieldIndex("ID_COURS"), true);
    ui->tvCours->setColumnHidden(modelCours->fieldIndex("ID_FORMATEUR"), true);
    int hd = modelCours->fieldIndex("HEURE_DEBUT");
    int hf = modelCours->fieldIndex("HEURE_FIN");
    if (hd >= 0) ui->tvCours->setColumnHidden(hd, true);
    if (hf >= 0) ui->tvCours->setColumnHidden(hf, true);

    modelSearchFormateurs = new QSqlQueryModel(this);
    ui->tvResultatFormateurs->setModel(modelSearchFormateurs);

    modelSearchCours = new QSqlQueryModel(this);
    ui->tableView->setModel(modelSearchCours);
}

void GFormateurCours::loadFormateurs()
{
    if (modelFormateurs) {
        modelFormateurs->select();
    }
}

void GFormateurCours::loadCours()
{
    if (modelCours) {
        modelCours->select();
    }
}

void GFormateurCours::populateFormateurComboBoxes()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    ui->cbFormateur->clear();
    ui->cbSelectFormateurDoc->clear();
    ui->cbFiltreFormateur->clear();
    ui->cbPlanningFormateur->clear();
    ui->cbSelectCoursDoc->clear();

    ui->cbFormateur->addItem("Sélectionner un formateur", QVariant(0));
    ui->cbSelectFormateurDoc->addItem("Tous", QVariant(0));
    ui->cbFiltreFormateur->addItem("Formateur");
    ui->cbPlanningFormateur->addItem("Tous", QVariant(0));
    ui->cbSelectCoursDoc->addItem("Tous");

    QSqlQuery query(db);
    if (query.exec("SELECT ID_FORMATEUR, NOM || ' ' || PRENOM FROM FORMATEUR ORDER BY NOM")) {
        while (query.next()) {
            int formateurId = query.value(0).toInt();
            QString fullName = query.value(1).toString();
            ui->cbFormateur->addItem(fullName, formateurId);
            ui->cbSelectFormateurDoc->addItem(fullName, formateurId);
            ui->cbFiltreFormateur->addItem(fullName);
            ui->cbPlanningFormateur->addItem(fullName, formateurId);
        }
    }

    QSqlQuery catQuery(db);
    if (catQuery.exec("SELECT DISTINCT CATEGORIE FROM COURS ORDER BY CATEGORIE")) {
        while (catQuery.next()) {
            ui->cbSelectCoursDoc->addItem(catQuery.value(0).toString());
        }
    }
}

void GFormateurCours::clearFormateurForm()
{
    ui->leNom->clear();
    ui->lePrenom->clear();
    ui->leEmail->clear();
    ui->leTelephone->clear();
    ui->cbSpecialite->setCurrentIndex(0);
    ui->deDateEmbauche->setDate(QDate::currentDate());
    ui->cbStatus->setCurrentIndex(0);
}

void GFormateurCours::clearCoursForm()
{
    ui->leIntitule->clear();
    ui->cbCategorie->setCurrentIndex(0);
    ui->cbNiveau->setCurrentIndex(0);
    ui->cbFormateur->setCurrentIndex(0);
    ui->sbDuree->setValue(1);
    ui->deDateDebutCours->setDate(QDate::currentDate());
    ui->deDateFinCours->setDate(QDate::currentDate());
    ui->teHeureDebutCours->setTime(QTime(9, 0));
    ui->teHeureFinCours->setTime(QTime(17, 0));
    ui->sbCapacite->setValue(1);
    ui->teProgramme->clear();
}

bool GFormateurCours::isSpecialiteCompatible(const QString &specialite, const QString &categorie) const
{
    QString spec = specialite.toLower();
    QString cat = categorie.toLower();

    if (cat.contains("programmation") && !spec.contains("programmation") && !spec.contains("c++") && !spec.contains("python")) {
        return false;
    }
    if (cat.contains("base") && !spec.contains("base")) {
        return false;
    }
    if (cat.contains("cloud") && !spec.contains("cloud")) {
        return false;
    }
    if (cat.contains("reseau") && !spec.contains("reseau") && !spec.contains("syst") && !spec.contains("administration")) {
        return false;
    }
    return true;
}

QString GFormateurCours::conflictingCourseInfo(int formateurId,
                                               const QDate &start, const QTime &heureDebut,
                                               const QDate &end, const QTime &heureFin,
                                               int excludeCourseId) const
{
    if (formateurId <= 0 || !start.isValid() || !end.isValid() || !heureDebut.isValid() || !heureFin.isValid()) {
        return QString();
    }
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return QString();

    int debutMin = heureDebut.hour() * 60 + heureDebut.minute();
    int finMin = heureFin.hour() * 60 + heureFin.minute();

    QString where = QString(
        "ID_FORMATEUR = %1"
        " AND TO_DATE('%2','YYYY-MM-DD') + %3/1440 < DATE_FIN + COALESCE(HEURE_FIN, 1439)/1440"
        " AND DATE_DEBUT + COALESCE(HEURE_DEBUT, 0)/1440 < TO_DATE('%4','YYYY-MM-DD') + %5/1440"
        " AND (%6 = 0 OR ID_COURS <> %6)")
        .arg(formateurId)
        .arg(start.toString("yyyy-MM-dd"))
        .arg(debutMin)
        .arg(end.toString("yyyy-MM-dd"))
        .arg(finMin)
        .arg(excludeCourseId);

    QSqlQuery query(db);
    QString infoSql = QString(
        "SELECT INTITULE, "
        "TO_CHAR(DATE_DEBUT,'DD/MM/YYYY') || ' ' || LPAD(TRUNC(COALESCE(HEURE_DEBUT,540)/60),2,'0') || ':' || LPAD(MOD(COALESCE(HEURE_DEBUT,540),60),2,'0') || ' → ' || "
        "TO_CHAR(DATE_FIN,'DD/MM/YYYY') || ' ' || LPAD(TRUNC(COALESCE(HEURE_FIN,1020)/60),2,'0') || ':' || LPAD(MOD(COALESCE(HEURE_FIN,1020),60),2,'0') "
        "FROM COURS WHERE %1 AND ROWNUM = 1").arg(where);
    if (query.exec(infoSql) && query.next()) {
        return QString("« %1 » (%2)").arg(query.value(0).toString(), query.value(1).toString());
    }
    return QString();
}

QString GFormateurCours::getFormateurSpecialite(int formateurId) const
{
    if (formateurId <= 0) return QString();
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return QString();

    QSqlQuery query(db);
    query.prepare("SELECT SPECIALITE FROM FORMATEUR WHERE ID_FORMATEUR = :id");
    query.bindValue(":id", formateurId);
    if (!query.exec() || !query.next()) {
        return QString();
    }
    return query.value(0).toString();
}

bool GFormateurCours::formateurHasFutureAssignments(int formateurId) const
{
    if (formateurId <= 0) return false;
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM COURS WHERE ID_FORMATEUR = :id AND DATE_FIN >= SYSDATE");
    query.bindValue(":id", formateurId);
    if (!query.exec()) {
        qWarning() << "Future assignments query failed:" << query.lastError().text();
        return false;
    }
    if (query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

bool GFormateurCours::validateFormateur(const Formateur &formateur, QString &message) const
{
    if (formateur.nom().trimmed().isEmpty()) {
        message = "Le nom du formateur est requis.";
        return false;
    }
    if (formateur.prenom().trimmed().isEmpty()) {
        message = "Le prénom du formateur est requis.";
        return false;
    }
    if (formateur.email().trimmed().isEmpty()) {
        message = "L'adresse e-mail est requise.";
        return false;
    }
    QRegularExpression emailRegex(R"(([^@\s]+)@([^@\s]+)\.([^@\s]+))");
    if (!formateur.email().contains(emailRegex)) {
        message = "L'adresse e-mail n'est pas valide.";
        return false;
    }
    if (formateur.specialite().trimmed().isEmpty()) {
        message = "La spécialité est requise.";
        return false;
    }
    if (formateur.status().trimmed().isEmpty()) {
        message = "Le statut est requis.";
        return false;
    }
    if (formateur.status().toLower() != "actif" && formateur.status().toLower() != "inactif") {
        message = "Le statut doit être Actif ou Inactif.";
        return false;
    }
    if (!formateur.dateEmbauche().isValid()) {
        message = "La date d'embauche est invalide.";
        return false;
    }
    return true;
}

bool GFormateurCours::validateCours(const Cours &cours, QString &message) const
{
    if (cours.intitule().trimmed().isEmpty()) {
        message = "L'intitulé du cours est requis.";
        return false;
    }
    if (cours.categorie().trimmed().isEmpty()) {
        message = "La catégorie du cours est requise.";
        return false;
    }
    if (cours.niveau().trimmed().isEmpty()) {
        message = "Le niveau du cours est requis.";
        return false;
    }
    if (cours.idFormateur() <= 0) {
        message = "Un formateur doit être sélectionné.";
        return false;
    }
    if (cours.duree() <= 0) {
        message = "La durée du cours doit être supérieure à 0.";
        return false;
    }
    if (!cours.dateDebut().isValid() || !cours.dateFin().isValid()) {
        message = "Les dates du cours sont invalides.";
        return false;
    }
    if (cours.dateFin() < cours.dateDebut()) {
        message = "La date de fin doit être postérieure ou égale à la date de début.";
        return false;
    }
    if (!cours.heureDebut().isValid() || !cours.heureFin().isValid()) {
        message = "Les heures du cours sont invalides.";
        return false;
    }
    if (cours.heureFin() <= cours.heureDebut()) {
        message = "L'heure de fin doit être postérieure à l'heure de début.";
        return false;
    }
    if (cours.capacite() <= 0) {
        message = "La capacité doit être supérieure à 0.";
        return false;
    }

    QString specialiteFormateur = getFormateurSpecialite(cours.idFormateur());
    if (specialiteFormateur.isEmpty()) {
        message = "Impossible de déterminer la spécialité du formateur sélectionné.";
        return false;
    }
    if (!isSpecialiteCompatible(specialiteFormateur, cours.categorie())) {
        message = "La spécialité du formateur ne semble pas compatible avec la catégorie du cours.";
        return false;
    }
    QString conflictInfo = conflictingCourseInfo(cours.idFormateur(), cours.dateDebut(), cours.heureDebut(),
                                                 cours.dateFin(), cours.heureFin(), cours.id());
    if (!conflictInfo.isEmpty()) {
        message = QString("Un formateur peut avoir plusieurs cours le même jour, mais le créneau %1 → %2 chevauche "
                          "le cours existant %3. Choisissez des horaires qui ne se chevauchent pas (ex. 09:00–12:00 puis 14:00–17:00).")
                      .arg(cours.heureDebut().toString("HH:mm"), cours.heureFin().toString("HH:mm"), conflictInfo);
        return false;
    }
    return true;
}

void GFormateurCours::onAjouterFormateur()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    Formateur f;
    f.setNom(ui->leNom->text());
    f.setPrenom(ui->lePrenom->text());
    f.setEmail(ui->leEmail->text());
    f.setTelephone(ui->leTelephone->text());
    f.setSpecialite(ui->cbSpecialite->currentText());
    f.setDateEmbauche(ui->deDateEmbauche->date());
    f.setStatus(ui->cbStatus->currentText());

    QString message;
    if (!validateFormateur(f, message)) {
        QMessageBox::warning(this, "Validation métier", message);
        return;
    }

    if (!f.insert(db)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ajouter le formateur. Consultez les logs.");
        return;
    }

    loadFormateurs();
    populateFormateurComboBoxes();
    clearFormateurForm();
    refreshPlanning();
    refreshAlertes();
}

void GFormateurCours::onModifierFormateur()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    QModelIndex current = ui->tvFormateurs->currentIndex();
    if (!current.isValid()) {
        QMessageBox::information(this, "Modifier", "Sélectionnez un formateur.");
        return;
    }

    int row = current.row();
    QSqlRecord record = modelFormateurs->record(row);
    int id = record.value("ID_FORMATEUR").toInt();

    Formateur f;
    f.setId(id);
    f.setNom(ui->leNom->text());
    f.setPrenom(ui->lePrenom->text());
    f.setEmail(ui->leEmail->text());
    f.setTelephone(ui->leTelephone->text());
    f.setSpecialite(ui->cbSpecialite->currentText());
    f.setDateEmbauche(ui->deDateEmbauche->date());
    f.setStatus(ui->cbStatus->currentText());

    if (!f.update(db)) {
        QMessageBox::warning(this, "Erreur", "Impossible de modifier le formateur. Consultez les logs.");
        return;
    }

    loadFormateurs();
    populateFormateurComboBoxes();
    refreshPlanning();
    refreshAlertes();
}

void GFormateurCours::onSupprimerFormateur()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    QModelIndex current = ui->tvFormateurs->currentIndex();
    if (!current.isValid()) {
        QMessageBox::information(this, "Supprimer", "Sélectionnez un formateur.");
        return;
    }

    int row = current.row();
    QSqlRecord record = modelFormateurs->record(row);
    int id = record.value("ID_FORMATEUR").toInt();

    Formateur f;
    f.setId(id);
    if (!f.remove(db)) {
        QMessageBox::warning(this, "Erreur", "Impossible de supprimer le formateur. Consultez les logs.");
        return;
    }

    loadFormateurs();
    populateFormateurComboBoxes();
    clearFormateurForm();
    refreshPlanning();
    refreshAlertes();
}

void GFormateurCours::onViderFormateur()
{
    clearFormateurForm();
}

void GFormateurCours::onAjouterCours()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    Cours c;
    c.setIntitule(ui->leIntitule->text());
    c.setCategorie(ui->cbCategorie->currentText());
    c.setNiveau(ui->cbNiveau->currentText());
    c.setFormateur(ui->cbFormateur->currentText());
    c.setIdFormateur(ui->cbFormateur->currentData().toInt());
    c.setDuree(ui->sbDuree->value());
    c.setDateDebut(ui->deDateDebutCours->date());
    c.setDateFin(ui->deDateFinCours->date());
    c.setHeureDebut(ui->teHeureDebutCours->time());
    c.setHeureFin(ui->teHeureFinCours->time());
    c.setCapacite(ui->sbCapacite->value());
    c.setProgramme(ui->teProgramme->toPlainText());

    QString message;
    if (!validateCours(c, message)) {
        QMessageBox::warning(this, "Validation métier", message);
        return;
    }

    if (!c.insert(db)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ajouter le cours. Consultez les logs.");
        return;
    }

    loadCours();
    clearCoursForm();
    refreshPlanning();
    refreshAlertes();
}

void GFormateurCours::onModifierCours()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    QModelIndex current = ui->tvCours->currentIndex();
    if (!current.isValid()) {
        QMessageBox::information(this, "Modifier", "Sélectionnez un cours.");
        return;
    }

    int row = current.row();
    QSqlRecord record = modelCours->record(row);
    int id = record.value("ID_COURS").toInt();

    Cours c;
    c.setId(id);
    c.setIntitule(ui->leIntitule->text());
    c.setCategorie(ui->cbCategorie->currentText());
    c.setNiveau(ui->cbNiveau->currentText());
    c.setFormateur(ui->cbFormateur->currentText());
    c.setIdFormateur(ui->cbFormateur->currentData().toInt());
    c.setDuree(ui->sbDuree->value());
    c.setDateDebut(ui->deDateDebutCours->date());
    c.setDateFin(ui->deDateFinCours->date());
    c.setHeureDebut(ui->teHeureDebutCours->time());
    c.setHeureFin(ui->teHeureFinCours->time());
    c.setCapacite(ui->sbCapacite->value());
    c.setProgramme(ui->teProgramme->toPlainText());

    QString message;
    if (!validateCours(c, message)) {
        QMessageBox::warning(this, "Validation métier", message);
        return;
    }

    if (!c.update(db)) {
        QMessageBox::warning(this, "Erreur", "Impossible de modifier le cours. Consultez les logs.");
        return;
    }

    loadCours();
    refreshPlanning();
    refreshAlertes();
}

void GFormateurCours::onSupprimerCours()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    QModelIndex current = ui->tvCours->currentIndex();
    if (!current.isValid()) {
        QMessageBox::information(this, "Supprimer", "Sélectionnez un cours.");
        return;
    }

    int row = current.row();
    QSqlRecord record = modelCours->record(row);
    int id = record.value("ID_COURS").toInt();

    Cours c;
    c.setId(id);
    if (!c.remove(db)) {
        QMessageBox::warning(this, "Erreur", "Impossible de supprimer le cours. Consultez les logs.");
        return;
    }

    loadCours();
    clearCoursForm();
    refreshPlanning();
    refreshAlertes();
}

void GFormateurCours::onViderCours()
{
    clearCoursForm();
}

void GFormateurCours::onRechercherFormateurs()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    QString sql = "SELECT NOM, PRENOM, EMAIL, TELEPHONE, SPECIALITE, DATE_EMBAUCHE, STATUS FROM FORMATEUR WHERE 1=1";
    QString name = ui->leRechercheNom->text().trimmed();
    if (!name.isEmpty()) {
        sql += " AND LOWER(NOM) LIKE LOWER('%" + name + "%')";
    }
    QString specialite = ui->cbFiltreSpecialite->currentText();
    if (!specialite.isEmpty() && specialite != "Toutes") {
        sql += " AND SPECIALITE = '" + specialite + "'";
    }
    QDate dateDebut = ui->deDateDebut->date();
    QDate dateFin = ui->deDateFin->date();
    sql += " AND DATE_EMBAUCHE BETWEEN TO_DATE('" + dateDebut.toString("yyyy-MM-dd") + "','YYYY-MM-DD') AND TO_DATE('" + dateFin.toString("yyyy-MM-dd") + "','YYYY-MM-DD')";
    QString orderBy;
    if (ui->cbTrierPar->currentText() == "Nom") orderBy = "NOM";
    else if (ui->cbTrierPar->currentText() == "Spécialité") orderBy = "SPECIALITE";
    else if (ui->cbTrierPar->currentText() == "Date d'embauche") orderBy = "DATE_EMBAUCHE";
    if (!orderBy.isEmpty()) {
        sql += " ORDER BY " + orderBy;
        if (ui->checkBoxDesc->isChecked()) sql += " DESC";
    }

    modelSearchFormateurs->setQuery(sql, QSqlDatabase::database());
    if (modelSearchFormateurs->lastError().isValid()) {
        QMessageBox::warning(this, "Erreur recherche", modelSearchFormateurs->lastError().text());
    }
}

void GFormateurCours::onRechercherCours()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    QString sql = "SELECT c.INTITULE, c.CATEGORIE, c.NIVEAU, "
                  "COALESCE(f.NOM || ' ' || f.PRENOM, 'Inconnu') AS FORMATEUR, "
                  "c.DUREE_HEURES, c.DATE_DEBUT, c.DATE_FIN, c.CAPACITE "
                  "FROM COURS c LEFT JOIN FORMATEUR f ON c.ID_FORMATEUR = f.ID_FORMATEUR WHERE 1=1";
    QString intitule = ui->leRechercheIntitule->text().trimmed();
    if (!intitule.isEmpty() && intitule != "Title") {
        sql += " AND LOWER(c.INTITULE) LIKE :intitule";
    }
    QString niveau = ui->cbFiltreNiveau->currentText();
    if (!niveau.isEmpty() && niveau != "Tous") {
        sql += " AND c.NIVEAU = :niveau";
    }
    QString formateur = ui->cbFiltreFormateur->currentText();
    if (!formateur.isEmpty() && formateur != "Formateur") {
        sql += " AND f.NOM || ' ' || f.PRENOM = :formateur";
    }
    bool okMin, okMax;
    int dureeMin = ui->leDureeMin->text().toInt(&okMin);
    int dureeMax = ui->leDureeMax->text().toInt(&okMax);
    if (okMin) sql += " AND c.DUREE_HEURES >= :dureeMin";
    if (okMax) sql += " AND c.DUREE_HEURES <= :dureeMax";

    QString orderBy;
    if (ui->cbTrierParCours->currentText() == "Intitulé") orderBy = "c.INTITULE";
    else if (ui->cbTrierParCours->currentText() == "Niveau") orderBy = "c.NIVEAU";
    else if (ui->cbTrierParCours->currentText() == "Durée") orderBy = "c.DUREE_HEURES";
    else if (ui->cbTrierParCours->currentText() == "Date de début") orderBy = "c.DATE_DEBUT";
    if (!orderBy.isEmpty()) {
        sql += " ORDER BY " + orderBy;
        if (ui->checkBox->isChecked()) sql += " DESC";
    }

    QSqlQuery query(db);
    query.prepare(sql);
    if (!intitule.isEmpty() && intitule != "Title") {
        query.bindValue(":intitule", "%" + intitule.toLower() + "%");
    }
    if (!niveau.isEmpty() && niveau != "Tous") {
        query.bindValue(":niveau", niveau);
    }
    if (!formateur.isEmpty() && formateur != "Formateur") {
        query.bindValue(":formateur", formateur);
    }
    if (okMin) {
        query.bindValue(":dureeMin", dureeMin);
    }
    if (okMax) {
        query.bindValue(":dureeMax", dureeMax);
    }

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur recherche", query.lastError().text());
        return;
    }

    modelSearchCours->setQuery(std::move(query));
    modelSearchCours->setHeaderData(0, Qt::Horizontal, "Intitulé");
    modelSearchCours->setHeaderData(1, Qt::Horizontal, "Catégorie");
    modelSearchCours->setHeaderData(2, Qt::Horizontal, "Niveau");
    modelSearchCours->setHeaderData(3, Qt::Horizontal, "Formateur");
    modelSearchCours->setHeaderData(4, Qt::Horizontal, "Durée (h)");
    modelSearchCours->setHeaderData(5, Qt::Horizontal, "Début");
    modelSearchCours->setHeaderData(6, Qt::Horizontal, "Fin");
    modelSearchCours->setHeaderData(7, Qt::Horizontal, "Capacité");

    if (modelSearchCours->lastError().isValid()) {
        QMessageBox::warning(this, "Erreur recherche", modelSearchCours->lastError().text());
    }
}

void GFormateurCours::onActualiserStatsFormateurs()
{
    refreshFormateurStats();
}

void GFormateurCours::onActualiserStatsCours()
{
    refreshCoursStats();
}

void GFormateurCours::refreshFormateurStats()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;
    QSqlQuery query(db);
    query.exec("SELECT SPECIALITE, COUNT(*) FROM FORMATEUR GROUP BY SPECIALITE");

    QPieSeries *series = new QPieSeries();
    while (query.next()) {
        series->append(query.value(0).toString(), query.value(1).toInt());
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des formateurs par spécialité");
    chart->setBackgroundVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setLabelColor(QColor("#334155"));

    ui->chartViewFormateurs->setChart(chart);
    ui->chartViewFormateurs->setRenderHint(QPainter::Antialiasing);
    ui->lblStatInfoFormateurs->setText("Survolez une part du diagramme pour afficher le détail.");

    setupPieHover(series, ui->lblStatInfoFormateurs, "Spécialité");
}

void GFormateurCours::refreshCoursStats()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    QString groupCol = "NIVEAU";
    QString groupLabel = "Niveau";
    if (ui->cbStatCoursGroupe->currentText() == "Catégorie") {
        groupCol = "CATEGORIE";
        groupLabel = "Catégorie";
    }

    QSqlQuery query(db);
    query.exec("SELECT " + groupCol + ", COUNT(*) FROM COURS GROUP BY " + groupCol);

    QPieSeries *series = new QPieSeries();
    while (query.next()) {
        series->append(query.value(0).toString(), query.value(1).toInt());
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des cours par " + groupLabel.toLower());
    chart->setBackgroundVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setLabelColor(QColor("#334155"));

    ui->chartViewCours->setChart(chart);
    ui->chartViewCours->setRenderHint(QPainter::Antialiasing);
    ui->lblStatInfoCours->setText("Survolez une part du diagramme pour afficher le détail.");

    setupPieHover(series, ui->lblStatInfoCours, groupLabel);
}

void GFormateurCours::setupPieHover(QPieSeries *series, QLabel *infoLabel, const QString &entity)
{
    static const QList<QColor> palette = {
        QColor("#2563EB"), QColor("#7C3AED"), QColor("#0891B2"), QColor("#16A34A"),
        QColor("#D97706"), QColor("#DC2626"), QColor("#DB2777"), QColor("#0F766E"),
        QColor("#9333EA"), QColor("#4F46E5"), QColor("#EA580C"), QColor("#0284C7")
    };

    int idx = 0;
    for (QPieSlice *slice : series->slices()) {
        const QColor base = palette[idx % palette.size()];
        ++idx;
        slice->setBrush(base);
        slice->setBorderColor(QColor("#FFFFFF"));
        slice->setBorderWidth(2);
        slice->setLabelColor(QColor("#1F2937"));
        slice->setLabelVisible(false);

        const QString name = slice->label();
        connect(slice, &QPieSlice::hovered, this, [slice, infoLabel, entity, base, name](bool state) {
            if (state) {
                const qreal pct = slice->percentage() * 100.0;
                slice->setLabel(QString("%1\n%2 (%3 %)").arg(name).arg(slice->value()).arg(pct, 0, 'f', 1));
                slice->setLabelVisible(true);
                slice->setExploded(true);
                slice->setBrush(base.lighter(118));
                if (infoLabel) {
                    infoLabel->setText(QString("%1 : %2 — %3 (%4 %)")
                                           .arg(entity, name)
                                           .arg(slice->value())
                                           .arg(pct, 0, 'f', 1));
                }
            } else {
                slice->setLabelVisible(false);
                slice->setExploded(false);
                slice->setBrush(base);
            }
        });
    }
}

void GanttWidget::setBars(const QList<Bar> &bars)
{
    m_bars = bars;
    m_barRects.clear();
    m_barRects.resize(bars.size());

    if (bars.isEmpty()) {
        m_totalDays = 0;
        setMinimumHeight(60);
        setMinimumWidth(400);
        update();
        return;
    }

    m_minDate = m_bars.first().start;
    m_maxDate = m_bars.first().end;
    for (const Bar &b : m_bars) {
        if (b.start < m_minDate) m_minDate = b.start;
        if (b.end > m_maxDate) m_maxDate = b.end;
    }
    m_totalDays = qMax(1, m_minDate.daysTo(m_maxDate) + 1);

    QSet<int> ids;
    for (const Bar &b : m_bars) {
        ids.insert(b.formateurId);
    }
    setMinimumHeight(50 + ids.size() * 42 + 20);
    resizeTimeline();
}

void GanttWidget::setViewMode(ViewMode mode, const QDate &focusDate)
{
    m_viewMode = mode;
    m_focusDate = focusDate;
    resizeTimeline();
    update();
}

void GanttWidget::resizeTimeline()
{
    if (m_viewMode == ViewMode::Hour) {
        setMinimumWidth(qMax(700, 220 + 10 + 1440 / 8));
    } else {
        setMinimumWidth(qMax(400, 220 + 10 + m_totalDays * m_pixelPerDay));
    }
    update();
}

void GanttWidget::mouseMoveEvent(QMouseEvent *event)
{
    for (int i = 0; i < m_bars.size() && i < m_barRects.size(); ++i) {
        if (m_barRects[i].contains(event->pos())) {
            const Bar &b = m_bars[i];
            setToolTip(QString("%1\nFormateur : %2\nDébut : %3 à %4\nFin : %5 à %6")
                           .arg(b.intitule, b.formateurName,
                                b.start.toString("dd/MM/yyyy"), b.heureDebut.toString("HH:mm"),
                                b.end.toString("dd/MM/yyyy"), b.heureFin.toString("HH:mm")));
            return;
        }
    }
    setToolTip(QString());
}

void GanttWidget::wheelEvent(QWheelEvent *event)
{
    if (m_viewMode != ViewMode::Day) {
        event->ignore();
        return;
    }
    int steps = event->angleDelta().y() / 120;
    if (steps == 0) {
        event->accept();
        return;
    }
    double factor = qPow(1.15, steps);
    int newPpd = qRound(m_pixelPerDay * factor);
    newPpd = qBound(8, newPpd, 200);
    if (newPpd != m_pixelPerDay) {
        m_pixelPerDay = newPpd;
        resizeTimeline();
    }
    event->accept();
}

void GanttWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#fafafa"));

    if (m_bars.isEmpty()) {
        painter.setPen(QColor("#808080"));
        painter.drawText(rect(), Qt::AlignCenter, "Aucun cours planifié.");
        return;
    }

    QFont f = font();
    f.setPointSizeF(9);

    if (m_viewMode == ViewMode::Hour) {
        paintHourView(painter, f);
    } else {
        paintDayView(painter, f);
    }
}

void GanttWidget::paintDayView(QPainter &painter, const QFont &f)
{
    const int labelWidth = 220;
    const int headerHeight = 34;
    const int hourRowHeight = 16;
    const int rowHeight = 42;
    const int barHeight = 20;
    const int plotTop = headerHeight + hourRowHeight;
    const int plotLeft = labelWidth;

    painter.setFont(f);

    QDate monthCursor = m_minDate;
    while (monthCursor <= m_maxDate) {
        QDate monthEnd = QDate(monthCursor.year(), monthCursor.month(), 1).addMonths(1).addDays(-1);
        if (monthEnd > m_maxDate) monthEnd = m_maxDate;
        double x0 = plotLeft + m_minDate.daysTo(monthCursor) * 1.0 * m_pixelPerDay;
        double x1 = plotLeft + (m_minDate.daysTo(monthEnd) + 1.0) * m_pixelPerDay;
        QRectF cell(x0, 0, x1 - x0, headerHeight);
        painter.setPen(QColor("#444444"));
        painter.drawText(cell, Qt::AlignCenter, monthCursor.toString("MMM yyyy"));
        painter.setPen(QColor("#cfcfcf"));
        painter.drawLine((int)x1, headerHeight, (int)x1, height());
        monthCursor = monthEnd.addDays(1);
    }

    int hourStep = m_pixelPerDay >= 80 ? 1 : (m_pixelPerDay >= 40 ? 3 : 6);
    painter.setFont(f);
    for (int d = 0; d < m_totalDays; ++d) {
        for (int h = hourStep; h < 24; h += hourStep) {
            int gx = plotLeft + (int)((d + h / 24.0) * m_pixelPerDay);
            painter.setPen(QColor("#888888"));
            painter.drawText(gx + 1, headerHeight + hourRowHeight - 3, QString("%1h").arg(h));
            painter.setPen(QPen(QColor("#e0e0e0"), 1));
            painter.drawLine(gx, headerHeight, gx, headerHeight + hourRowHeight);
        }
    }

    QDate today = QDate::currentDate();
    if (today >= m_minDate && today <= m_maxDate) {
        int tx = plotLeft + (int)(m_minDate.daysTo(today) * 1.0 * m_pixelPerDay);
        painter.setPen(QPen(QColor("#c62828"), 2));
        painter.drawLine(tx, plotTop, tx, height());
        painter.drawText(tx + 4, headerHeight + 10, "Aujourd'hui");
    }

    drawBars(painter, f, labelWidth, plotTop, rowHeight, barHeight,
             [&](const Bar &b) -> QPair<double, double> {
                 double startOffset = m_minDate.daysTo(b.start)
                                      + (b.heureDebut.hour() * 60.0 + b.heureDebut.minute()) / 1440.0;
                 double endOffset = m_minDate.daysTo(b.end)
                                    + (b.heureFin.hour() * 60.0 + b.heureFin.minute()) / 1440.0;
                 return qMakePair(plotLeft + startOffset * m_pixelPerDay,
                                  plotLeft + endOffset * m_pixelPerDay);
             });
}

void GanttWidget::paintHourView(QPainter &painter, const QFont &f)
{
    const int labelWidth = 220;
    const int headerHeight = 34;
    const int hourRowHeight = 16;
    const int rowHeight = 42;
    const int barHeight = 20;
    const int plotTop = headerHeight + hourRowHeight;
    const int plotLeft = labelWidth;
    const int plotWidth = qMax(300, width() - plotLeft - 10);

    painter.setFont(f);
    QRectF titleCell(plotLeft, 0, plotWidth, headerHeight);
    painter.setPen(QColor("#444444"));
    painter.drawText(titleCell, Qt::AlignCenter, m_focusDate.toString("dddd d MMMM yyyy"));

    for (int h = 0; h <= 24; ++h) {
        int gx = plotLeft + (int)((h / 24.0) * plotWidth);
        painter.setPen(QPen(QColor("#d0d0d0"), 1));
        painter.drawLine(gx, headerHeight, gx, height());
        if (h < 24) {
            painter.setPen(QColor("#888888"));
            painter.drawText(gx + 2, headerHeight + hourRowHeight - 3, QString("%1h").arg(h));
        }
    }

    QDate today = QDate::currentDate();
    if (m_focusDate == today) {
        QTime now = QTime::currentTime();
        int nx = plotLeft + (int)((now.hour() * 60.0 + now.minute()) / 1440.0 * plotWidth);
        painter.setPen(QPen(QColor("#c62828"), 2));
        painter.drawLine(nx, plotTop, nx, height());
        painter.drawText(nx + 4, headerHeight + 10, "Maintenant");
    }

    drawBars(painter, f, labelWidth, plotTop, rowHeight, barHeight,
             [&](const Bar &b) -> QPair<double, double> {
                 double x0 = plotLeft + (b.heureDebut.hour() * 60.0 + b.heureDebut.minute()) / 1440.0 * plotWidth;
                 double x1 = plotLeft + (b.heureFin.hour() * 60.0 + b.heureFin.minute()) / 1440.0 * plotWidth;
                 return qMakePair(x0, x1);
             });
}

void GanttWidget::drawBars(QPainter &painter, const QFont &f, int labelWidth,
                           int plotTop, int rowHeight, int barHeight,
                           const std::function<QPair<double, double>(const Bar &)> &geo)
{
    QMap<int, QList<int>> rowMap;
    QList<int> rowOrder;
    for (int i = 0; i < m_bars.size(); ++i) {
        if (m_viewMode == ViewMode::Hour) {
            const Bar &b = m_bars[i];
            if (m_focusDate < b.start || m_focusDate > b.end) continue;
        }
        int fid = m_bars[i].formateurId;
        if (!rowMap.contains(fid)) {
            rowMap.insert(fid, QList<int>());
            rowOrder.append(fid);
        }
        rowMap[fid].append(i);
    }

    QFont bold = f;
    bold.setBold(true);

    for (int r = 0; r < rowOrder.size(); ++r) {
        int fid = rowOrder[r];
        int top = plotTop + r * rowHeight;

        if (r % 2 == 0) {
            painter.fillRect(QRect(0, top, width(), rowHeight), QColor("#f0f0f0"));
        }

        painter.setPen(QColor("#333333"));
        painter.setFont(bold);
        QString name = m_bars[rowMap[fid].first()].formateurName;
        painter.drawText(QRect(8, top, labelWidth - 14, rowHeight), Qt::AlignVCenter | Qt::AlignLeft, name);
        painter.setFont(f);

        for (int bi : rowMap[fid]) {
            const Bar &b = m_bars[bi];
            QPair<double, double> pos = geo(b);
            double x0 = pos.first;
            double x1 = pos.second;
            if (x1 - x0 < 2) x1 = x0 + 2;

            QRectF barRect(x0, top + (rowHeight - barHeight) / 2.0, x1 - x0, barHeight);
            m_barRects[bi] = barRect;

            QColor color = b.actif ? QColor("#43a047") : QColor("#9e9e9e");
            if (b.conflict) color = QColor("#d32f2f");

            painter.setBrush(color);
            painter.setPen(QColor(color.darker(140)));
            painter.drawRoundedRect(barRect, 3, 3);

            if (barRect.width() > 46) {
                painter.setPen(Qt::white);
                painter.setFont(bold);
                QString label = b.intitule + "  " + b.heureDebut.toString("HH:mm")
                                + " → " + b.heureFin.toString("HH:mm");
                QRectF textRect = barRect.adjusted(4, 0, -4, 0);
                painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft,
                                 painter.fontMetrics().elidedText(label, Qt::ElideRight, (int)textRect.width()));
                painter.setFont(f);
            }
        }
    }
}

void GFormateurCours::refreshPlanning()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    int filterId = ui->cbPlanningFormateur->currentData().toInt();

    QSqlQuery query(db);
    if (!query.exec("SELECT c.INTITULE, c.ID_FORMATEUR, f.NOM || ' ' || f.PRENOM, "
                    "TO_CHAR(c.DATE_DEBUT,'YYYY-MM-DD'), TO_CHAR(c.DATE_FIN,'YYYY-MM-DD'), f.STATUS, "
                    "COALESCE(c.HEURE_DEBUT, 540), COALESCE(c.HEURE_FIN, 1020) "
                    "FROM COURS c JOIN FORMATEUR f ON c.ID_FORMATEUR = f.ID_FORMATEUR "
                    "ORDER BY f.NOM, c.DATE_DEBUT")) {
        qWarning() << "Planning query failed:" << query.lastError().text();
        return;
    }

    QList<GanttWidget::Bar> bars;
    while (query.next()) {
        int formateurId = query.value(1).toInt();
        if (filterId > 0 && formateurId != filterId) continue;

        GanttWidget::Bar bar;
        bar.formateurId = formateurId;
        bar.formateurName = query.value(2).toString();
        bar.intitule = query.value(0).toString();
        bar.start = QDate::fromString(query.value(3).toString(), Qt::ISODate);
        bar.end = QDate::fromString(query.value(4).toString(), Qt::ISODate);
        bar.actif = query.value(5).toString().toLower() == "actif";
        bar.heureDebut = QTime::fromMSecsSinceStartOfDay(query.value(6).toInt() * 60000);
        bar.heureFin = QTime::fromMSecsSinceStartOfDay(query.value(7).toInt() * 60000);
        bar.conflict = false;
        bars.append(bar);
    }

    for (int i = 0; i < bars.size(); ++i) {
        QDateTime startA(bars[i].start, bars[i].heureDebut);
        QDateTime endA(bars[i].end, bars[i].heureFin);
        for (int j = 0; j < bars.size(); ++j) {
            if (i == j || bars[i].formateurId != bars[j].formateurId) continue;
            QDateTime startB(bars[j].start, bars[j].heureDebut);
            QDateTime endB(bars[j].end, bars[j].heureFin);
            if (startA < endB && startB < endA) {
                bars[i].conflict = true;
                break;
            }
        }
    }

    bool hourMode = ui->cbPlanningVue->currentIndex() == 1;
    ui->dePlanningJour->setVisible(hourMode);

    if (hourMode && !bars.isEmpty() && !m_planningDaySet) {
        m_planningDaySet = true;
        QDate first = bars.first().start;
        for (const GanttWidget::Bar &b : bars) {
            if (b.start < first) first = b.start;
        }
        ui->dePlanningJour->setDate(first);
    }

    m_gantt->setBars(bars);
    m_gantt->setViewMode(hourMode ? GanttWidget::ViewMode::Hour : GanttWidget::ViewMode::Day,
                         hourMode ? ui->dePlanningJour->date() : QDate());
}

void GFormateurCours::onActualiserPlanning()
{
    refreshPlanning();
}

void GFormateurCours::refreshAlertes()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    ui->twAlertes->setRowCount(0);
    int count = 0;

    auto addAlert = [&](const QString &priorite, const QString &type, const QString &detail) {
        int row = ui->twAlertes->rowCount();
        ui->twAlertes->insertRow(row);
        QTableWidgetItem *p = new QTableWidgetItem(priorite);
        QTableWidgetItem *t = new QTableWidgetItem(type);
        QTableWidgetItem *d = new QTableWidgetItem(detail);
        QColor color = QColor("#1565c0");
        if (priorite == "Critique") color = QColor("#c62828");
        else if (priorite == "Attention") color = QColor("#ef6c00");
        p->setForeground(color);
        t->setForeground(color);
        d->setForeground(color);
        ui->twAlertes->setItem(row, 0, p);
        ui->twAlertes->setItem(row, 1, t);
        ui->twAlertes->setItem(row, 2, d);
        ++count;
    };

    QSqlQuery q1(db);
    if (q1.exec("SELECT f.NOM || ' ' || f.PRENOM, c1.INTITULE, c2.INTITULE, "
                "TO_CHAR(c1.DATE_DEBUT,'YYYY-MM-DD') || ' ' || TO_CHAR(COALESCE(c1.HEURE_DEBUT,540)/60,'FM00') || ':' || "
                "  TO_CHAR(MOD(COALESCE(c1.HEURE_DEBUT,540),60),'FM00') || ' → ' || "
                "  TO_CHAR(c1.DATE_FIN,'YYYY-MM-DD') || ' ' || TO_CHAR(COALESCE(c1.HEURE_FIN,1020)/60,'FM00') || ':' || "
                "  TO_CHAR(MOD(COALESCE(c1.HEURE_FIN,1020),60),'FM00'), "
                "TO_CHAR(c2.DATE_DEBUT,'YYYY-MM-DD') || ' ' || TO_CHAR(COALESCE(c2.HEURE_DEBUT,540)/60,'FM00') || ':' || "
                "  TO_CHAR(MOD(COALESCE(c2.HEURE_DEBUT,540),60),'FM00') || ' → ' || "
                "  TO_CHAR(c2.DATE_FIN,'YYYY-MM-DD') || ' ' || TO_CHAR(COALESCE(c2.HEURE_FIN,1020)/60,'FM00') || ':' || "
                "  TO_CHAR(MOD(COALESCE(c2.HEURE_FIN,1020),60),'FM00') "
                "FROM COURS c1 JOIN COURS c2 ON c1.ID_FORMATEUR = c2.ID_FORMATEUR "
                "JOIN FORMATEUR f ON f.ID_FORMATEUR = c1.ID_FORMATEUR "
                "WHERE c1.ID_COURS < c2.ID_COURS "
                "AND c1.DATE_DEBUT + COALESCE(c1.HEURE_DEBUT,540)/1440 < c2.DATE_FIN + COALESCE(c2.HEURE_FIN,1020)/1440 "
                "AND c2.DATE_DEBUT + COALESCE(c2.HEURE_DEBUT,540)/1440 < c1.DATE_FIN + COALESCE(c1.HEURE_FIN,1020)/1440")) {
        while (q1.next()) {
            addAlert("Critique", "Chevauchement",
                     QString("%1 est planifié sur deux cours en même temps : « %2 » (%4) et « %3 » (%5).")
                         .arg(q1.value(0).toString(), q1.value(1).toString(), q1.value(2).toString(),
                              q1.value(3).toString(), q1.value(4).toString()));
        }
    }

    QSqlQuery q2(db);
    if (q2.exec("SELECT INTITULE, TO_CHAR(DATE_DEBUT,'YYYY-MM-DD') FROM COURS "
                "WHERE DATE_DEBUT BETWEEN SYSDATE AND SYSDATE + 7 ORDER BY DATE_DEBUT")) {
        while (q2.next()) {
            addAlert("Attention", "Échéance proche",
                     QString("Le cours « %1 » commence le %2 (≤ 7 jours).")
                         .arg(q2.value(0).toString(), q2.value(1).toString()));
        }
    }

    QSqlQuery q3(db);
    if (q3.exec("SELECT DISTINCT f.NOM || ' ' || f.PRENOM FROM FORMATEUR f "
                "JOIN COURS c ON c.ID_FORMATEUR = f.ID_FORMATEUR "
                "WHERE LOWER(f.STATUS) = 'inactif'")) {
        while (q3.next()) {
            addAlert("Attention", "Formateur inactif",
                     QString("Le formateur %1 est inactif mais possède des cours planifiés.").arg(q3.value(0).toString()));
        }
    }

    QSqlQuery q4(db);
    if (q4.exec("SELECT INTITULE FROM COURS WHERE DATE_DEBUT <= SYSDATE AND DATE_FIN >= SYSDATE")) {
        while (q4.next()) {
            addAlert("Information", "Cours en cours",
                     QString("Le cours « %1 » est actuellement en cours.").arg(q4.value(0).toString()));
        }
    }

    if (count == 0) {
        ui->twAlertes->setRowCount(1);
        ui->twAlertes->setItem(0, 0, new QTableWidgetItem("—"));
        ui->twAlertes->setItem(0, 1, new QTableWidgetItem("Aucune alerte"));
        ui->twAlertes->setItem(0, 2, new QTableWidgetItem("Aucune règle métier n'est en infraction."));
    }

    ui->lblAlertesInfo->setText(QString("%1 alerte(s) détectée(s)").arg(count));
}

void GFormateurCours::onActualiserAlertes()
{
    refreshAlertes();
}

void GFormateurCours::onGenererPdfFormateur()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer PDF formateur", QString(), "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    int formateurId = ui->cbSelectFormateurDoc->currentData().toInt();
    QString subtitle = (formateurId > 0)
        ? QString("Fiche formateur : %1").arg(ui->cbSelectFormateurDoc->currentText())
        : "Liste des formateurs";

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(150);
    QPainter painter(&writer);
    if (!painter.isActive()) return;

    const int pageWidth = writer.width();
    const int pageHeight = writer.height();
    const int margin = 40;
    const int contentWidth = pageWidth - 2 * margin;
    const int pageBottom = pageHeight - 60;

    const QColor accentBlue = QColor("#2563EB");
    const QColor darkBlue = QColor("#1E3A8A");
    const QColor lightBg = QColor("#F8FAFC");
    const QColor lineColor = QColor("#D9E2EC");

    painter.fillRect(0, 0, pageWidth, 90, darkBlue);
    painter.fillRect(0, 90, pageWidth, 30, accentBlue);

    const QString logoPath = findProjectLogoPath();
    if (!logoPath.isEmpty()) {
        QImage logoImage(logoPath);
        if (!logoImage.isNull()) {
            QSize targetSize(120, 52);
            QImage scaled = logoImage.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            painter.drawImage(pageWidth - margin - scaled.width(), 18, scaled);
        }
    }

    QFont titleFont("Segoe UI", 18, QFont::Bold);
    QFont subFont("Segoe UI", 10, QFont::Normal);
    QFont headerFont("Segoe UI", 10, QFont::Bold);
    QFont bodyFont("Segoe UI", 9);
    painter.setPen(Qt::white);
    painter.setFont(titleFont);
    painter.drawText(margin, 42, "Centre de Formation");
    painter.setFont(subFont);
    painter.drawText(margin, 70, QString("%1 • %2").arg(subtitle, QDate::currentDate().toString("dd MMMM yyyy")));

    painter.setPen(Qt::black);
    painter.fillRect(margin, 140, contentWidth, 25, lightBg);
    painter.setPen(accentBlue);
    painter.setFont(headerFont);
    painter.drawText(margin + 12, 158, subtitle);

    painter.setPen(lineColor);
    painter.drawLine(margin, 170, pageWidth - margin, 170);

    QString sql = "SELECT ID_FORMATEUR, NOM, PRENOM, EMAIL, TELEPHONE, SPECIALITE, "
                  "TO_CHAR(DATE_EMBAUCHE,'YYYY-MM-DD'), STATUS FROM FORMATEUR";
    if (formateurId > 0) {
        sql += " WHERE ID_FORMATEUR = :id";
    }
    sql += " ORDER BY NOM, PRENOM";

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare(sql);
    if (formateurId > 0) {
        query.bindValue(":id", formateurId);
    }

    int page = 1;
    int y = 190;
    if (query.exec()) {
        int item = 0;
        while (query.next()) {
            if (y > pageBottom - 60) {
                painter.setFont(bodyFont);
                painter.setPen(Qt::darkGray);
                painter.drawText(margin, pageBottom + 30, QString("Page %1").arg(page));
                writer.newPage();
                ++page;
                y = 55;
                painter.fillRect(0, 0, pageWidth, 52, darkBlue);
                painter.setPen(Qt::white);
                painter.setFont(titleFont);
                painter.drawText(margin, 32, "Centre de Formation");
                painter.setFont(subFont);
                painter.drawText(margin, 46, subtitle);
                painter.setPen(Qt::black);
                y = 70;
            }

            ++item;
            painter.setPen(accentBlue);
            painter.setFont(headerFont);
            painter.drawText(margin, y, QString("%1. %2 %3").arg(item).arg(query.value(1).toString(), query.value(2).toString()));
            y += 18;

            painter.setPen(Qt::black);
            painter.setFont(bodyFont);
            painter.drawText(margin, y, QString("Email : %1   |   Téléphone : %2")
                                 .arg(query.value(3).toString(), query.value(4).toString()));
            y += 16;
            painter.drawText(margin, y, QString("Spécialité : %1   |   Statut : %2")
                                 .arg(query.value(5).toString(), query.value(7).toString()));
            y += 16;
            painter.drawText(margin, y, QString("Date d'embauche : %1").arg(query.value(6).toString()));
            y += 18;
            painter.setPen(lineColor);
            painter.drawLine(margin, y, pageWidth - margin, y);
            y += 22;
        }
    }

    painter.setPen(Qt::darkGray);
    painter.setFont(bodyFont);
    painter.drawText(margin, pageBottom + 30, QString("Page %1").arg(page));
    painter.end();
}

void GFormateurCours::onGenererDocCours()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer PDF cours", QString(), "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(150);
    QPainter painter(&writer);
    if (!painter.isActive()) return;

    const int pageWidth = writer.width();
    const int pageHeight = writer.height();
    const int margin = 40;
    const int contentWidth = pageWidth - 2 * margin;
    const int pageBottom = pageHeight - 60;

    const QColor accentBlue = QColor("#2563EB");
    const QColor darkBlue = QColor("#1E3A8A");
    const QColor lightBg = QColor("#F8FAFC");
    const QColor lineColor = QColor("#D9E2EC");

    painter.fillRect(0, 0, pageWidth, 90, darkBlue);
    painter.fillRect(0, 90, pageWidth, 30, accentBlue);

    const QString logoPath = findProjectLogoPath();
    if (!logoPath.isEmpty()) {
        QImage logoImage(logoPath);
        if (!logoImage.isNull()) {
            QSize targetSize(120, 52);
            QImage scaled = logoImage.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            painter.drawImage(pageWidth - margin - scaled.width(), 18, scaled);
        }
    }

    QFont titleFont("Segoe UI", 18, QFont::Bold);
    QFont subFont("Segoe UI", 10, QFont::Normal);
    QFont headerFont("Segoe UI", 10, QFont::Bold);
    QFont bodyFont("Segoe UI", 9);

    painter.setPen(Qt::white);
    painter.setFont(titleFont);
    painter.drawText(margin, 42, "Centre de Formation");
    painter.setFont(subFont);
    painter.drawText(margin, 70, QString("Liste des cours • %1").arg(QDate::currentDate().toString("dd MMMM yyyy")));

    painter.setPen(Qt::black);
    painter.fillRect(margin, 140, contentWidth, 25, lightBg);
    painter.setPen(accentBlue);
    painter.setFont(headerFont);
    QString filter = ui->cbSelectCoursDoc->currentText();
    painter.drawText(margin + 12, 158, filter.isEmpty() || filter == "Tous" ? "Liste des cours" : QString("Liste des cours - %1").arg(filter));

    painter.setPen(lineColor);
    painter.drawLine(margin, 170, pageWidth - margin, 170);

    QString sql = "SELECT c.ID_COURS, c.INTITULE, c.CATEGORIE, c.NIVEAU, "
                  "COALESCE(f.NOM || ' ' || f.PRENOM, 'Inconnu') AS FORMATEUR, "
                  "c.DUREE_HEURES, TO_CHAR(c.DATE_DEBUT,'YYYY-MM-DD'), TO_CHAR(c.DATE_FIN,'YYYY-MM-DD'), c.CAPACITE "
                  "FROM COURS c LEFT JOIN FORMATEUR f ON c.ID_FORMATEUR = f.ID_FORMATEUR";
    if (!filter.isEmpty() && filter != "Tous") {
        sql += " WHERE c.CATEGORIE = :categorie";
    }
    sql += " ORDER BY c.ID_COURS";

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare(sql);
    if (!filter.isEmpty() && filter != "Tous") {
        query.bindValue(":categorie", filter);
    }

    int page = 1;
    int y = 190;
    if (query.exec()) {
        int item = 0;
        while (query.next()) {
            if (y > pageBottom - 60) {
                painter.setFont(bodyFont);
                painter.setPen(Qt::darkGray);
                painter.drawText(margin, pageBottom + 30, QString("Page %1").arg(page));
                writer.newPage();
                ++page;
                y = 55;
                painter.fillRect(0, 0, pageWidth, 52, darkBlue);
                painter.setPen(Qt::white);
                painter.setFont(titleFont);
                painter.drawText(margin, 32, "Centre de Formation");
                painter.setFont(subFont);
                painter.drawText(margin, 46, "Liste des cours");
                painter.setPen(Qt::black);
                y = 70;
            }

            ++item;
            painter.setPen(accentBlue);
            painter.setFont(headerFont);
            painter.drawText(margin, y, QString("%1. %2").arg(item).arg(query.value(1).toString()));
            y += 18;

            painter.setPen(Qt::black);
            painter.setFont(bodyFont);
            painter.drawText(margin, y, QString("Catégorie : %1   |   Niveau : %2   |   Formateur : %3")
                                 .arg(query.value(2).toString(), query.value(3).toString(), query.value(4).toString()));
            y += 16;
            painter.drawText(margin, y, QString("Durée : %1 h   |   Dates : %2 → %3   |   Capacité : %4")
                                 .arg(query.value(5).toString(), query.value(6).toString(), query.value(7).toString(), query.value(8).toString()));
            y += 18;
            painter.setPen(lineColor);
            painter.drawLine(margin, y, pageWidth - margin, y);
            y += 22;
        }
    }

    painter.setPen(Qt::darkGray);
    painter.setFont(bodyFont);
    painter.drawText(margin, pageBottom + 30, QString("Page %1").arg(page));
    painter.end();
}

void GFormateurCours::onFormateurSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;
    int row = index.row();
    QSqlRecord record = modelFormateurs->record(row);
    ui->leNom->setText(record.value("NOM").toString());
    ui->lePrenom->setText(record.value("PRENOM").toString());
    ui->leEmail->setText(record.value("EMAIL").toString());
    ui->leTelephone->setText(record.value("TELEPHONE").toString());
    setComboValueSafely(ui->cbSpecialite, record.value("SPECIALITE").toString());
    ui->deDateEmbauche->setDate(record.value("DATE_EMBAUCHE").toDate());
    setComboValueSafely(ui->cbStatus, record.value("STATUS").toString());
}

void GFormateurCours::onCoursSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;
    int row = index.row();
    QSqlRecord record = modelCours->record(row);
    ui->leIntitule->setText(record.value("INTITULE").toString());
    setComboValueSafely(ui->cbCategorie, record.value("CATEGORIE").toString());
    setComboValueSafely(ui->cbNiveau, record.value("NIVEAU").toString());
    int formateurId = record.value("ID_FORMATEUR").toInt();
    int formateurIdx = ui->cbFormateur->findData(formateurId);
    if (formateurIdx >= 0) {
        ui->cbFormateur->setCurrentIndex(formateurIdx);
    }
    ui->sbDuree->setValue(record.value("DUREE_HEURES").toInt());
    ui->deDateDebutCours->setDate(record.value("DATE_DEBUT").toDate());
    ui->deDateFinCours->setDate(record.value("DATE_FIN").toDate());
    ui->teHeureDebutCours->setTime(QTime::fromMSecsSinceStartOfDay(record.value("HEURE_DEBUT").toInt() * 60000));
    ui->teHeureFinCours->setTime(QTime::fromMSecsSinceStartOfDay(record.value("HEURE_FIN").toInt() * 60000));
    ui->sbCapacite->setValue(record.value("CAPACITE").toInt());
    ui->teProgramme->setPlainText(record.value("PROGRAMME").toString());
}
