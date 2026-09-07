/********************************************************************************
** Form generated from reading UI file 'gformateurcours.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GFORMATEURCOURS_H
#define UI_GFORMATEURCOURS_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidgetModules;
    QWidget *pageFormateurs;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabFormateursSections;
    QWidget *tab;
    QHBoxLayout *horizontalLayout;
    QGroupBox *gbFormFormateur;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QLineEdit *leNom;
    QLineEdit *lePrenom;
    QLineEdit *leEmail;
    QLineEdit *leTelephone;
    QComboBox *cbSpecialite;
    QDateEdit *deDateEmbauche;
    QComboBox *cbStatus;
    QPushButton *btnAjouterFormateur;
    QPushButton *btnModifierFormateur;
    QPushButton *btnSupprimerFormateur;
    QPushButton *btnViderFormateur;
    QTableView *tvFormateurs;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *gbFiltreFormateur;
    QVBoxLayout *verticalLayout_4;
    QTableView *tvResultatFormateurs;
    QLineEdit *leRechercheNom;
    QComboBox *cbFiltreSpecialite;
    QDateEdit *deDateDebut;
    QDateEdit *deDateFin;
    QComboBox *cbTrierPar;
    QCheckBox *checkBoxDesc;
    QPushButton *btnRechercher;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_5;
    QChartView *chartViewFormateurs;
    QLabel *lblStatInfoFormateurs;
    QPushButton *btnActualiserStatsFormateurs;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_6;
    QLabel *CheminPDF;
    QComboBox *cbSelectFormateurDoc;
    QPushButton *btnGenererPdfFormateur;
    QWidget *pageCours;
    QHBoxLayout *horizontalLayout_3;
    QTabWidget *tabCoursSections;
    QWidget *tab_5;
    QHBoxLayout *horizontalLayout_5;
    QGroupBox *gbFormCours;
    QVBoxLayout *verticalLayout_10;
    QLineEdit *leIntitule;
    QComboBox *cbCategorie;
    QComboBox *cbNiveau;
    QComboBox *cbFormateur;
    QSpinBox *sbDuree;
    QDateEdit *deDateDebutCours;
    QDateEdit *deDateFinCours;
    QHBoxLayout *horizontalLayout_Heures;
    QLabel *labelHeureDebutCours;
    QTimeEdit *teHeureDebutCours;
    QLabel *labelHeureFinCours;
    QTimeEdit *teHeureFinCours;
    QSpinBox *sbCapacite;
    QTextEdit *teProgramme;
    QPushButton *btnAjouterCours;
    QPushButton *btnModifierCours;
    QPushButton *btnSupprimerCours;
    QPushButton *btnViderCours;
    QTableView *tvCours;
    QWidget *tab_6;
    QVBoxLayout *verticalLayout_7;
    QGroupBox *gbFiltreCours;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *cbTrierParCours;
    QComboBox *cbFiltreNiveau;
    QLineEdit *leDureeMin;
    QLineEdit *leDureeMax;
    QLineEdit *leRechercheIntitule;
    QComboBox *cbFiltreFormateur;
    QCheckBox *checkBox;
    QPushButton *btnRechercherCours;
    QTableView *tableView;
    QWidget *tab_7;
    QVBoxLayout *verticalLayout_8;
    QChartView *chartViewCours;
    QHBoxLayout *horizontalLayout_stats_cours;
    QLabel *labelGroupeCours;
    QComboBox *cbStatCoursGroupe;
    QPushButton *btnActualiserStatsCours;
    QLabel *lblStatInfoCours;
    QWidget *tab_8;
    QVBoxLayout *verticalLayout_9;
    QComboBox *cbSelectCoursDoc;
    QPushButton *btnGenererDocCours;
    QLabel *lblStatusDocCours;
    QWidget *pagePlanning;
    QVBoxLayout *verticalLayout_planning;
    QHBoxLayout *horizontalLayout_planning;
    QLabel *labelPlanningFormateur;
    QComboBox *cbPlanningFormateur;
    QComboBox *cbPlanningVue;
    QDateEdit *dePlanningJour;
    QPushButton *btnActualiserPlanning;
    QSpacerItem *horizontalSpacerPlanning;
    QScrollArea *saGantt;
    QWidget *saGanttContents;
    QWidget *pageAlertes;
    QVBoxLayout *verticalLayout_alertes;
    QLabel *lblAlertesInfo;
    QTableWidget *twAlertes;
    QPushButton *btnActualiserAlertes;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1491, 843);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        tabWidgetModules = new QTabWidget(centralwidget);
        tabWidgetModules->setObjectName("tabWidgetModules");
        pageFormateurs = new QWidget();
        pageFormateurs->setObjectName("pageFormateurs");
        verticalLayout_2 = new QVBoxLayout(pageFormateurs);
        verticalLayout_2->setObjectName("verticalLayout_2");
        tabFormateursSections = new QTabWidget(pageFormateurs);
        tabFormateursSections->setObjectName("tabFormateursSections");
        tab = new QWidget();
        tab->setObjectName("tab");
        horizontalLayout = new QHBoxLayout(tab);
        horizontalLayout->setObjectName("horizontalLayout");
        gbFormFormateur = new QGroupBox(tab);
        gbFormFormateur->setObjectName("gbFormFormateur");
        verticalLayout_3 = new QVBoxLayout(gbFormFormateur);
        verticalLayout_3->setObjectName("verticalLayout_3");
        label = new QLabel(gbFormFormateur);
        label->setObjectName("label");
        QSizePolicy sizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(label);

        leNom = new QLineEdit(gbFormFormateur);
        leNom->setObjectName("leNom");

        verticalLayout_3->addWidget(leNom);

        lePrenom = new QLineEdit(gbFormFormateur);
        lePrenom->setObjectName("lePrenom");

        verticalLayout_3->addWidget(lePrenom);

        leEmail = new QLineEdit(gbFormFormateur);
        leEmail->setObjectName("leEmail");

        verticalLayout_3->addWidget(leEmail);

        leTelephone = new QLineEdit(gbFormFormateur);
        leTelephone->setObjectName("leTelephone");

        verticalLayout_3->addWidget(leTelephone);

        cbSpecialite = new QComboBox(gbFormFormateur);
        cbSpecialite->addItem(QString());
        cbSpecialite->addItem(QString());
        cbSpecialite->addItem(QString());
        cbSpecialite->addItem(QString());
        cbSpecialite->addItem(QString());
        cbSpecialite->addItem(QString());
        cbSpecialite->addItem(QString());
        cbSpecialite->setObjectName("cbSpecialite");

        verticalLayout_3->addWidget(cbSpecialite);

        deDateEmbauche = new QDateEdit(gbFormFormateur);
        deDateEmbauche->setObjectName("deDateEmbauche");
        deDateEmbauche->setMinimumSize(QSize(677, 0));

        verticalLayout_3->addWidget(deDateEmbauche);

        cbStatus = new QComboBox(gbFormFormateur);
        cbStatus->addItem(QString());
        cbStatus->addItem(QString());
        cbStatus->setObjectName("cbStatus");

        verticalLayout_3->addWidget(cbStatus);

        btnAjouterFormateur = new QPushButton(gbFormFormateur);
        btnAjouterFormateur->setObjectName("btnAjouterFormateur");

        verticalLayout_3->addWidget(btnAjouterFormateur);

        btnModifierFormateur = new QPushButton(gbFormFormateur);
        btnModifierFormateur->setObjectName("btnModifierFormateur");

        verticalLayout_3->addWidget(btnModifierFormateur);

        btnSupprimerFormateur = new QPushButton(gbFormFormateur);
        btnSupprimerFormateur->setObjectName("btnSupprimerFormateur");

        verticalLayout_3->addWidget(btnSupprimerFormateur);

        btnViderFormateur = new QPushButton(gbFormFormateur);
        btnViderFormateur->setObjectName("btnViderFormateur");

        verticalLayout_3->addWidget(btnViderFormateur);


        horizontalLayout->addWidget(gbFormFormateur);

        tvFormateurs = new QTableView(tab);
        tvFormateurs->setObjectName("tvFormateurs");
        tvFormateurs->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        horizontalLayout->addWidget(tvFormateurs);

        tabFormateursSections->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        horizontalLayout_2 = new QHBoxLayout(tab_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        gbFiltreFormateur = new QGroupBox(tab_2);
        gbFiltreFormateur->setObjectName("gbFiltreFormateur");
        gbFiltreFormateur->setFlat(false);
        verticalLayout_4 = new QVBoxLayout(gbFiltreFormateur);
        verticalLayout_4->setObjectName("verticalLayout_4");
        tvResultatFormateurs = new QTableView(gbFiltreFormateur);
        tvResultatFormateurs->setObjectName("tvResultatFormateurs");

        verticalLayout_4->addWidget(tvResultatFormateurs);

        leRechercheNom = new QLineEdit(gbFiltreFormateur);
        leRechercheNom->setObjectName("leRechercheNom");

        verticalLayout_4->addWidget(leRechercheNom);

        cbFiltreSpecialite = new QComboBox(gbFiltreFormateur);
        cbFiltreSpecialite->addItem(QString());
        cbFiltreSpecialite->addItem(QString());
        cbFiltreSpecialite->addItem(QString());
        cbFiltreSpecialite->addItem(QString());
        cbFiltreSpecialite->addItem(QString());
        cbFiltreSpecialite->addItem(QString());
        cbFiltreSpecialite->addItem(QString());
        cbFiltreSpecialite->addItem(QString());
        cbFiltreSpecialite->setObjectName("cbFiltreSpecialite");

        verticalLayout_4->addWidget(cbFiltreSpecialite);

        deDateDebut = new QDateEdit(gbFiltreFormateur);
        deDateDebut->setObjectName("deDateDebut");
        deDateDebut->setMinimumSize(QSize(1389, 29));

        verticalLayout_4->addWidget(deDateDebut);

        deDateFin = new QDateEdit(gbFiltreFormateur);
        deDateFin->setObjectName("deDateFin");

        verticalLayout_4->addWidget(deDateFin);

        cbTrierPar = new QComboBox(gbFiltreFormateur);
        cbTrierPar->addItem(QString());
        cbTrierPar->addItem(QString());
        cbTrierPar->addItem(QString());
        cbTrierPar->setObjectName("cbTrierPar");

        verticalLayout_4->addWidget(cbTrierPar);

        checkBoxDesc = new QCheckBox(gbFiltreFormateur);
        checkBoxDesc->setObjectName("checkBoxDesc");

        verticalLayout_4->addWidget(checkBoxDesc);

        btnRechercher = new QPushButton(gbFiltreFormateur);
        btnRechercher->setObjectName("btnRechercher");

        verticalLayout_4->addWidget(btnRechercher);


        horizontalLayout_2->addWidget(gbFiltreFormateur);

        tabFormateursSections->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        verticalLayout_5 = new QVBoxLayout(tab_3);
        verticalLayout_5->setObjectName("verticalLayout_5");
        chartViewFormateurs = new QChartView(tab_3);
        chartViewFormateurs->setObjectName("chartViewFormateurs");

        verticalLayout_5->addWidget(chartViewFormateurs);

        lblStatInfoFormateurs = new QLabel(tab_3);
        lblStatInfoFormateurs->setObjectName("lblStatInfoFormateurs");
        lblStatInfoFormateurs->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(lblStatInfoFormateurs);

        btnActualiserStatsFormateurs = new QPushButton(tab_3);
        btnActualiserStatsFormateurs->setObjectName("btnActualiserStatsFormateurs");

        verticalLayout_5->addWidget(btnActualiserStatsFormateurs);

        tabFormateursSections->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        verticalLayout_6 = new QVBoxLayout(tab_4);
        verticalLayout_6->setObjectName("verticalLayout_6");
        CheminPDF = new QLabel(tab_4);
        CheminPDF->setObjectName("label_2");

        verticalLayout_6->addWidget(CheminPDF);

        cbSelectFormateurDoc = new QComboBox(tab_4);
        cbSelectFormateurDoc->addItem(QString());
        cbSelectFormateurDoc->setObjectName("cbSelectFormateurDoc");

        verticalLayout_6->addWidget(cbSelectFormateurDoc);

        btnGenererPdfFormateur = new QPushButton(tab_4);
        btnGenererPdfFormateur->setObjectName("btnGenererPdfFormateur");

        verticalLayout_6->addWidget(btnGenererPdfFormateur);

        tabFormateursSections->addTab(tab_4, QString());

        verticalLayout_2->addWidget(tabFormateursSections);

        tabWidgetModules->addTab(pageFormateurs, QString());
        pageCours = new QWidget();
        pageCours->setObjectName("pageCours");
        horizontalLayout_3 = new QHBoxLayout(pageCours);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        tabCoursSections = new QTabWidget(pageCours);
        tabCoursSections->setObjectName("tabCoursSections");
        tab_5 = new QWidget();
        tab_5->setObjectName("tab_5");
        horizontalLayout_5 = new QHBoxLayout(tab_5);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        gbFormCours = new QGroupBox(tab_5);
        gbFormCours->setObjectName("gbFormCours");
        verticalLayout_10 = new QVBoxLayout(gbFormCours);
        verticalLayout_10->setObjectName("verticalLayout_10");
        leIntitule = new QLineEdit(gbFormCours);
        leIntitule->setObjectName("leIntitule");

        verticalLayout_10->addWidget(leIntitule);

        cbCategorie = new QComboBox(gbFormCours);
        cbCategorie->addItem(QString());
        cbCategorie->addItem(QString());
        cbCategorie->addItem(QString());
        cbCategorie->addItem(QString());
        cbCategorie->addItem(QString());
        cbCategorie->addItem(QString());
        cbCategorie->addItem(QString());
        cbCategorie->setObjectName("cbCategorie");

        verticalLayout_10->addWidget(cbCategorie);

        cbNiveau = new QComboBox(gbFormCours);
        cbNiveau->addItem(QString());
        cbNiveau->addItem(QString());
        cbNiveau->addItem(QString());
        cbNiveau->setObjectName("cbNiveau");

        verticalLayout_10->addWidget(cbNiveau);

        cbFormateur = new QComboBox(gbFormCours);
        cbFormateur->addItem(QString());
        cbFormateur->addItem(QString());
        cbFormateur->addItem(QString());
        cbFormateur->setObjectName("cbFormateur");

        verticalLayout_10->addWidget(cbFormateur);

        sbDuree = new QSpinBox(gbFormCours);
        sbDuree->setObjectName("sbDuree");
        sbDuree->setMinimum(1);
        sbDuree->setMaximum(100);

        verticalLayout_10->addWidget(sbDuree);

        deDateDebutCours = new QDateEdit(gbFormCours);
        deDateDebutCours->setObjectName("deDateDebutCours");
        deDateDebutCours->setCalendarPopup(true);

        verticalLayout_10->addWidget(deDateDebutCours);

        deDateFinCours = new QDateEdit(gbFormCours);
        deDateFinCours->setObjectName("deDateFinCours");
        deDateFinCours->setCalendarPopup(true);

        verticalLayout_10->addWidget(deDateFinCours);

        horizontalLayout_Heures = new QHBoxLayout();
        horizontalLayout_Heures->setObjectName("horizontalLayout_Heures");
        labelHeureDebutCours = new QLabel(gbFormCours);
        labelHeureDebutCours->setObjectName("labelHeureDebutCours");

        horizontalLayout_Heures->addWidget(labelHeureDebutCours);

        teHeureDebutCours = new QTimeEdit(gbFormCours);
        teHeureDebutCours->setObjectName("teHeureDebutCours");
        teHeureDebutCours->setTime(QTime(9, 0, 0));

        horizontalLayout_Heures->addWidget(teHeureDebutCours);

        labelHeureFinCours = new QLabel(gbFormCours);
        labelHeureFinCours->setObjectName("labelHeureFinCours");

        horizontalLayout_Heures->addWidget(labelHeureFinCours);

        teHeureFinCours = new QTimeEdit(gbFormCours);
        teHeureFinCours->setObjectName("teHeureFinCours");
        teHeureFinCours->setTime(QTime(17, 0, 0));

        horizontalLayout_Heures->addWidget(teHeureFinCours);


        verticalLayout_10->addLayout(horizontalLayout_Heures);

        sbCapacite = new QSpinBox(gbFormCours);
        sbCapacite->setObjectName("sbCapacite");
        sbCapacite->setMinimum(1);
        sbCapacite->setMaximum(100);

        verticalLayout_10->addWidget(sbCapacite);

        teProgramme = new QTextEdit(gbFormCours);
        teProgramme->setObjectName("teProgramme");

        verticalLayout_10->addWidget(teProgramme);

        btnAjouterCours = new QPushButton(gbFormCours);
        btnAjouterCours->setObjectName("btnAjouterCours");

        verticalLayout_10->addWidget(btnAjouterCours);

        btnModifierCours = new QPushButton(gbFormCours);
        btnModifierCours->setObjectName("btnModifierCours");

        verticalLayout_10->addWidget(btnModifierCours);

        btnSupprimerCours = new QPushButton(gbFormCours);
        btnSupprimerCours->setObjectName("btnSupprimerCours");

        verticalLayout_10->addWidget(btnSupprimerCours);

        btnViderCours = new QPushButton(gbFormCours);
        btnViderCours->setObjectName("btnViderCours");

        verticalLayout_10->addWidget(btnViderCours);


        horizontalLayout_5->addWidget(gbFormCours);

        tvCours = new QTableView(tab_5);
        tvCours->setObjectName("tvCours");

        horizontalLayout_5->addWidget(tvCours);

        tabCoursSections->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName("tab_6");
        verticalLayout_7 = new QVBoxLayout(tab_6);
        verticalLayout_7->setObjectName("verticalLayout_7");
        gbFiltreCours = new QGroupBox(tab_6);
        gbFiltreCours->setObjectName("gbFiltreCours");
        horizontalLayout_4 = new QHBoxLayout(gbFiltreCours);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        cbTrierParCours = new QComboBox(gbFiltreCours);
        cbTrierParCours->addItem(QString());
        cbTrierParCours->addItem(QString());
        cbTrierParCours->addItem(QString());
        cbTrierParCours->addItem(QString());
        cbTrierParCours->addItem(QString());
        cbTrierParCours->setObjectName("cbTrierParCours");

        horizontalLayout_4->addWidget(cbTrierParCours);

        cbFiltreNiveau = new QComboBox(gbFiltreCours);
        cbFiltreNiveau->addItem(QString());
        cbFiltreNiveau->addItem(QString());
        cbFiltreNiveau->addItem(QString());
        cbFiltreNiveau->addItem(QString());
        cbFiltreNiveau->setObjectName("cbFiltreNiveau");

        horizontalLayout_4->addWidget(cbFiltreNiveau);

        leDureeMin = new QLineEdit(gbFiltreCours);
        leDureeMin->setObjectName("leDureeMin");

        horizontalLayout_4->addWidget(leDureeMin);

        leDureeMax = new QLineEdit(gbFiltreCours);
        leDureeMax->setObjectName("leDureeMax");

        horizontalLayout_4->addWidget(leDureeMax);

        leRechercheIntitule = new QLineEdit(gbFiltreCours);
        leRechercheIntitule->setObjectName("leRechercheIntitule");

        horizontalLayout_4->addWidget(leRechercheIntitule);

        cbFiltreFormateur = new QComboBox(gbFiltreCours);
        cbFiltreFormateur->addItem(QString());
        cbFiltreFormateur->setObjectName("cbFiltreFormateur");

        horizontalLayout_4->addWidget(cbFiltreFormateur);

        checkBox = new QCheckBox(gbFiltreCours);
        checkBox->setObjectName("checkBox");

        horizontalLayout_4->addWidget(checkBox);

        btnRechercherCours = new QPushButton(gbFiltreCours);
        btnRechercherCours->setObjectName("btnRechercherCours");

        horizontalLayout_4->addWidget(btnRechercherCours);


        verticalLayout_7->addWidget(gbFiltreCours);

        tableView = new QTableView(tab_6);
        tableView->setObjectName("tableView");

        verticalLayout_7->addWidget(tableView);

        tabCoursSections->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName("tab_7");
        verticalLayout_8 = new QVBoxLayout(tab_7);
        verticalLayout_8->setObjectName("verticalLayout_8");
        chartViewCours = new QChartView(tab_7);
        chartViewCours->setObjectName("chartViewCours");

        verticalLayout_8->addWidget(chartViewCours);

        horizontalLayout_stats_cours = new QHBoxLayout();
        horizontalLayout_stats_cours->setObjectName("horizontalLayout_stats_cours");
        labelGroupeCours = new QLabel(tab_7);
        labelGroupeCours->setObjectName("labelGroupeCours");

        horizontalLayout_stats_cours->addWidget(labelGroupeCours);

        cbStatCoursGroupe = new QComboBox(tab_7);
        cbStatCoursGroupe->addItem(QString());
        cbStatCoursGroupe->addItem(QString());
        cbStatCoursGroupe->setObjectName("cbStatCoursGroupe");

        horizontalLayout_stats_cours->addWidget(cbStatCoursGroupe);

        btnActualiserStatsCours = new QPushButton(tab_7);
        btnActualiserStatsCours->setObjectName("btnActualiserStatsCours");

        horizontalLayout_stats_cours->addWidget(btnActualiserStatsCours);


        verticalLayout_8->addLayout(horizontalLayout_stats_cours);

        lblStatInfoCours = new QLabel(tab_7);
        lblStatInfoCours->setObjectName("lblStatInfoCours");
        lblStatInfoCours->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(lblStatInfoCours);

        tabCoursSections->addTab(tab_7, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName("tab_8");
        verticalLayout_9 = new QVBoxLayout(tab_8);
        verticalLayout_9->setObjectName("verticalLayout_9");
        cbSelectCoursDoc = new QComboBox(tab_8);
        cbSelectCoursDoc->addItem(QString());
        cbSelectCoursDoc->addItem(QString());
        cbSelectCoursDoc->addItem(QString());
        cbSelectCoursDoc->addItem(QString());
        cbSelectCoursDoc->addItem(QString());
        cbSelectCoursDoc->setObjectName("cbSelectCoursDoc");

        verticalLayout_9->addWidget(cbSelectCoursDoc);

        btnGenererDocCours = new QPushButton(tab_8);
        btnGenererDocCours->setObjectName("btnGenererDocCours");

        verticalLayout_9->addWidget(btnGenererDocCours);

        lblStatusDocCours = new QLabel(tab_8);
        lblStatusDocCours->setObjectName("lblStatusDocCours");

        verticalLayout_9->addWidget(lblStatusDocCours);

        tabCoursSections->addTab(tab_8, QString());

        horizontalLayout_3->addWidget(tabCoursSections);

        tabWidgetModules->addTab(pageCours, QString());
        pagePlanning = new QWidget();
        pagePlanning->setObjectName("pagePlanning");
        verticalLayout_planning = new QVBoxLayout(pagePlanning);
        verticalLayout_planning->setObjectName("verticalLayout_planning");
        horizontalLayout_planning = new QHBoxLayout();
        horizontalLayout_planning->setObjectName("horizontalLayout_planning");
        labelPlanningFormateur = new QLabel(pagePlanning);
        labelPlanningFormateur->setObjectName("labelPlanningFormateur");

        horizontalLayout_planning->addWidget(labelPlanningFormateur);

        cbPlanningFormateur = new QComboBox(pagePlanning);
        cbPlanningFormateur->addItem(QString());
        cbPlanningFormateur->setObjectName("cbPlanningFormateur");

        horizontalLayout_planning->addWidget(cbPlanningFormateur);

        cbPlanningVue = new QComboBox(pagePlanning);
        cbPlanningVue->addItem(QString());
        cbPlanningVue->addItem(QString());
        cbPlanningVue->setObjectName("cbPlanningVue");

        horizontalLayout_planning->addWidget(cbPlanningVue);

        dePlanningJour = new QDateEdit(pagePlanning);
        dePlanningJour->setObjectName("dePlanningJour");
        dePlanningJour->setCalendarPopup(true);

        horizontalLayout_planning->addWidget(dePlanningJour);

        btnActualiserPlanning = new QPushButton(pagePlanning);
        btnActualiserPlanning->setObjectName("btnActualiserPlanning");

        horizontalLayout_planning->addWidget(btnActualiserPlanning);

        horizontalSpacerPlanning = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_planning->addItem(horizontalSpacerPlanning);


        verticalLayout_planning->addLayout(horizontalLayout_planning);

        saGantt = new QScrollArea(pagePlanning);
        saGantt->setObjectName("saGantt");
        saGantt->setWidgetResizable(true);
        saGanttContents = new QWidget();
        saGanttContents->setObjectName("saGanttContents");
        saGanttContents->setGeometry(QRect(0, 0, 1441, 697));
        saGantt->setWidget(saGanttContents);

        verticalLayout_planning->addWidget(saGantt);

        tabWidgetModules->addTab(pagePlanning, QString());
        pageAlertes = new QWidget();
        pageAlertes->setObjectName("pageAlertes");
        verticalLayout_alertes = new QVBoxLayout(pageAlertes);
        verticalLayout_alertes->setObjectName("verticalLayout_alertes");
        lblAlertesInfo = new QLabel(pageAlertes);
        lblAlertesInfo->setObjectName("lblAlertesInfo");

        verticalLayout_alertes->addWidget(lblAlertesInfo);

        twAlertes = new QTableWidget(pageAlertes);
        twAlertes->setObjectName("twAlertes");

        verticalLayout_alertes->addWidget(twAlertes);

        btnActualiserAlertes = new QPushButton(pageAlertes);
        btnActualiserAlertes->setObjectName("btnActualiserAlertes");

        verticalLayout_alertes->addWidget(btnActualiserAlertes);

        tabWidgetModules->addTab(pageAlertes, QString());

        verticalLayout->addWidget(tabWidgetModules);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidgetModules->setCurrentIndex(0);
        tabFormateursSections->setCurrentIndex(3);
        tabCoursSections->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        gbFormFormateur->setTitle(QString());
        label->setText(QCoreApplication::translate("MainWindow", "Formulaire", nullptr));
        leNom->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        lePrenom->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom", nullptr));
        leEmail->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        leTelephone->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone", nullptr));
        cbSpecialite->setItemText(0, QCoreApplication::translate("MainWindow", "Cloud / DevOps", nullptr));
        cbSpecialite->setItemText(1, QCoreApplication::translate("MainWindow", "Programmation C++", nullptr));
        cbSpecialite->setItemText(2, QCoreApplication::translate("MainWindow", "Programmation Python", nullptr));
        cbSpecialite->setItemText(3, QCoreApplication::translate("MainWindow", "Developpement Web", nullptr));
        cbSpecialite->setItemText(4, QCoreApplication::translate("MainWindow", "Base de donn\303\251es", nullptr));
        cbSpecialite->setItemText(5, QCoreApplication::translate("MainWindow", "R\303\251seau & syst\303\251mes", nullptr));
        cbSpecialite->setItemText(6, QCoreApplication::translate("MainWindow", "IA", nullptr));

        cbStatus->setItemText(0, QCoreApplication::translate("MainWindow", "Actif", nullptr));
        cbStatus->setItemText(1, QCoreApplication::translate("MainWindow", "Inactif", nullptr));

        btnAjouterFormateur->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        btnModifierFormateur->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        btnSupprimerFormateur->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        btnViderFormateur->setText(QCoreApplication::translate("MainWindow", "Vider", nullptr));
        tabFormateursSections->setTabText(tabFormateursSections->indexOf(tab), QCoreApplication::translate("MainWindow", "Gestion", nullptr));
#if QT_CONFIG(tooltip)
        gbFiltreFormateur->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>suii</p><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        gbFiltreFormateur->setTitle(QString());
        leRechercheNom->setPlaceholderText(QCoreApplication::translate("MainWindow", "Nom \303\240 rechercher", nullptr));
        cbFiltreSpecialite->setItemText(0, QCoreApplication::translate("MainWindow", "Toutes", nullptr));
        cbFiltreSpecialite->setItemText(1, QCoreApplication::translate("MainWindow", "Cloud / DevOps", nullptr));
        cbFiltreSpecialite->setItemText(2, QCoreApplication::translate("MainWindow", "Programmation C++", nullptr));
        cbFiltreSpecialite->setItemText(3, QCoreApplication::translate("MainWindow", "Programmation Python", nullptr));
        cbFiltreSpecialite->setItemText(4, QCoreApplication::translate("MainWindow", "Developpement Web", nullptr));
        cbFiltreSpecialite->setItemText(5, QCoreApplication::translate("MainWindow", "Base de donn\303\251es", nullptr));
        cbFiltreSpecialite->setItemText(6, QCoreApplication::translate("MainWindow", "IA", nullptr));
        cbFiltreSpecialite->setItemText(7, QCoreApplication::translate("MainWindow", "R\303\251seau & syst\303\251mes", nullptr));

        cbTrierPar->setItemText(0, QCoreApplication::translate("MainWindow", "Nom", nullptr));
        cbTrierPar->setItemText(1, QCoreApplication::translate("MainWindow", "Sp\303\251cialit\303\251", nullptr));
        cbTrierPar->setItemText(2, QCoreApplication::translate("MainWindow", "Date d'embauche", nullptr));

        checkBoxDesc->setText(QCoreApplication::translate("MainWindow", "D\303\251croissant", nullptr));
        btnRechercher->setText(QCoreApplication::translate("MainWindow", "Rechercher", nullptr));
        tabFormateursSections->setTabText(tabFormateursSections->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Recherche/Tri", nullptr));
        lblStatInfoFormateurs->setText(QCoreApplication::translate("MainWindow", "Survolez une part du diagramme pour afficher le d\303\251tail.", nullptr));
        btnActualiserStatsFormateurs->setText(QCoreApplication::translate("MainWindow", "Actualiser", nullptr));
        tabFormateursSections->setTabText(tabFormateursSections->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        CheminPDF->setText(QCoreApplication::translate("MainWindow", "PDF g\303\251ner\303\251 : chemin", nullptr));
        cbSelectFormateurDoc->setItemText(0, QCoreApplication::translate("MainWindow", "Formateur ", nullptr));

        btnGenererPdfFormateur->setText(QCoreApplication::translate("MainWindow", "Generer PDF", nullptr));
        tabFormateursSections->setTabText(tabFormateursSections->indexOf(tab_4), QCoreApplication::translate("MainWindow", "Document", nullptr));
        tabWidgetModules->setTabText(tabWidgetModules->indexOf(pageFormateurs), QCoreApplication::translate("MainWindow", "Formateurs", nullptr));
        gbFormCours->setTitle(QString());
        leIntitule->setText(QCoreApplication::translate("MainWindow", "Intitul\303\251", nullptr));
        cbCategorie->setItemText(0, QCoreApplication::translate("MainWindow", "Programmation C++", nullptr));
        cbCategorie->setItemText(1, QCoreApplication::translate("MainWindow", "R\303\251seaux & syst\303\251mes", nullptr));
        cbCategorie->setItemText(2, QCoreApplication::translate("MainWindow", "IA", nullptr));
        cbCategorie->setItemText(3, QCoreApplication::translate("MainWindow", "Developpement Web", nullptr));
        cbCategorie->setItemText(4, QCoreApplication::translate("MainWindow", "Programmation Python ", nullptr));
        cbCategorie->setItemText(5, QCoreApplication::translate("MainWindow", "Cloud / DevOps", nullptr));
        cbCategorie->setItemText(6, QCoreApplication::translate("MainWindow", "Base de donn\303\251es", nullptr));

        cbNiveau->setItemText(0, QCoreApplication::translate("MainWindow", "D\303\251butant", nullptr));
        cbNiveau->setItemText(1, QCoreApplication::translate("MainWindow", "Interm\303\251diaire", nullptr));
        cbNiveau->setItemText(2, QCoreApplication::translate("MainWindow", "Avanc\303\251", nullptr));

        cbFormateur->setItemText(0, QCoreApplication::translate("MainWindow", "Ahmed", nullptr));
        cbFormateur->setItemText(1, QCoreApplication::translate("MainWindow", "Sami", nullptr));
        cbFormateur->setItemText(2, QCoreApplication::translate("MainWindow", "Ali", nullptr));

        sbDuree->setSuffix(QCoreApplication::translate("MainWindow", "heures", nullptr));
        labelHeureDebutCours->setText(QCoreApplication::translate("MainWindow", "Heure d\303\251but", nullptr));
        teHeureDebutCours->setDisplayFormat(QCoreApplication::translate("MainWindow", "HH:mm", nullptr));
        labelHeureFinCours->setText(QCoreApplication::translate("MainWindow", "Heure fin", nullptr));
        teHeureFinCours->setDisplayFormat(QCoreApplication::translate("MainWindow", "HH:mm", nullptr));
        btnAjouterCours->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        btnModifierCours->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        btnSupprimerCours->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        btnViderCours->setText(QCoreApplication::translate("MainWindow", "Vider", nullptr));
        tabCoursSections->setTabText(tabCoursSections->indexOf(tab_5), QCoreApplication::translate("MainWindow", "Gestion", nullptr));
        gbFiltreCours->setTitle(QString());
        cbTrierParCours->setItemText(0, QCoreApplication::translate("MainWindow", "Aucun", nullptr));
        cbTrierParCours->setItemText(1, QCoreApplication::translate("MainWindow", "Intitul\303\251", nullptr));
        cbTrierParCours->setItemText(2, QCoreApplication::translate("MainWindow", "Niveau", nullptr));
        cbTrierParCours->setItemText(3, QCoreApplication::translate("MainWindow", "Dur\303\251e", nullptr));
        cbTrierParCours->setItemText(4, QCoreApplication::translate("MainWindow", "Date de d\303\251but", nullptr));

        cbFiltreNiveau->setItemText(0, QCoreApplication::translate("MainWindow", "Tous", nullptr));
        cbFiltreNiveau->setItemText(1, QCoreApplication::translate("MainWindow", "Debutant", nullptr));
        cbFiltreNiveau->setItemText(2, QCoreApplication::translate("MainWindow", "Interm\303\251diaire", nullptr));
        cbFiltreNiveau->setItemText(3, QCoreApplication::translate("MainWindow", "Avanc\303\251e", nullptr));

        leDureeMin->setPlaceholderText(QCoreApplication::translate("MainWindow", "Dur\303\251e min", nullptr));
        leDureeMax->setPlaceholderText(QCoreApplication::translate("MainWindow", "Dur\303\251e max", nullptr));
        leRechercheIntitule->setPlaceholderText(QCoreApplication::translate("MainWindow", "Intitul\303\251 \303\240 rechercher", nullptr));
        cbFiltreFormateur->setItemText(0, QCoreApplication::translate("MainWindow", "Formateur", nullptr));

        checkBox->setText(QCoreApplication::translate("MainWindow", "Decroissant", nullptr));
        btnRechercherCours->setText(QCoreApplication::translate("MainWindow", "Rechercher", nullptr));
        tabCoursSections->setTabText(tabCoursSections->indexOf(tab_6), QCoreApplication::translate("MainWindow", "Recherche/Tri", nullptr));
        labelGroupeCours->setText(QCoreApplication::translate("MainWindow", "Grouper par :", nullptr));
        cbStatCoursGroupe->setItemText(0, QCoreApplication::translate("MainWindow", "Niveau", nullptr));
        cbStatCoursGroupe->setItemText(1, QCoreApplication::translate("MainWindow", "Cat\303\251gorie", nullptr));

        btnActualiserStatsCours->setText(QCoreApplication::translate("MainWindow", "Actualiser", nullptr));
        lblStatInfoCours->setText(QCoreApplication::translate("MainWindow", "Survolez une part du diagramme pour afficher le d\303\251tail.", nullptr));
        tabCoursSections->setTabText(tabCoursSections->indexOf(tab_7), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        cbSelectCoursDoc->setItemText(0, QCoreApplication::translate("MainWindow", "Programmation", nullptr));
        cbSelectCoursDoc->setItemText(1, QCoreApplication::translate("MainWindow", "IA", nullptr));
        cbSelectCoursDoc->setItemText(2, QCoreApplication::translate("MainWindow", "COURS 1", nullptr));
        cbSelectCoursDoc->setItemText(3, QCoreApplication::translate("MainWindow", "COURS 2", nullptr));
        cbSelectCoursDoc->setItemText(4, QCoreApplication::translate("MainWindow", "Tous", nullptr));

        btnGenererDocCours->setText(QCoreApplication::translate("MainWindow", "G\303\251n\303\251rer PDF", nullptr));
        lblStatusDocCours->setText(QString());
        tabCoursSections->setTabText(tabCoursSections->indexOf(tab_8), QCoreApplication::translate("MainWindow", "Document", nullptr));
        tabWidgetModules->setTabText(tabWidgetModules->indexOf(pageCours), QCoreApplication::translate("MainWindow", "Cours", nullptr));
        labelPlanningFormateur->setText(QCoreApplication::translate("MainWindow", "Formateur :", nullptr));
        cbPlanningFormateur->setItemText(0, QCoreApplication::translate("MainWindow", "Tous", nullptr));

        cbPlanningVue->setItemText(0, QCoreApplication::translate("MainWindow", "Par jour", nullptr));
        cbPlanningVue->setItemText(1, QCoreApplication::translate("MainWindow", "Par heure", nullptr));

        btnActualiserPlanning->setText(QCoreApplication::translate("MainWindow", "Actualiser", nullptr));
        tabWidgetModules->setTabText(tabWidgetModules->indexOf(pagePlanning), QCoreApplication::translate("MainWindow", "Planning", nullptr));
        lblAlertesInfo->setText(QCoreApplication::translate("MainWindow", "V\303\251rification des r\303\250gles m\303\251tier...", nullptr));
        btnActualiserAlertes->setText(QCoreApplication::translate("MainWindow", "Actualiser les alertes", nullptr));
        tabWidgetModules->setTabText(tabWidgetModules->indexOf(pageAlertes), QCoreApplication::translate("MainWindow", "Alertes", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GFORMATEURCOURS_H
