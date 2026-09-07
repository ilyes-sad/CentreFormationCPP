#ifndef GFORMATEURCOURS_H
#define GFORMATEURCOURS_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QString>
#include <QDate>
#include <QTime>
#include <QWidget>
#include <QList>
#include <QSet>
#include <QPair>
#include <functional>

class QPaintEvent;
class QMouseEvent;
class QWheelEvent;
class QPainter;
class QFont;
class QLabel;
class QPieSeries;

class GanttWidget : public QWidget
{
public:
    enum class ViewMode { Day, Hour };

    struct Bar
    {
        int formateurId;
        QString formateurName;
        QString intitule;
        QDate start;
        QDate end;
        QTime heureDebut;
        QTime heureFin;
        bool conflict;
        bool actif;
    };

    explicit GanttWidget(QWidget *parent = nullptr) : QWidget(parent) {}

    void setBars(const QList<Bar> &bars);
    void setViewMode(ViewMode mode, const QDate &focusDate = QDate());
    QDate minDate() const { return m_minDate; }
    QDate maxDate() const { return m_maxDate; }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QList<Bar> m_bars;
    QList<QRectF> m_barRects;
    QDate m_minDate;
    QDate m_maxDate;
    int m_totalDays = 0;
    int m_pixelPerDay = 32;
    ViewMode m_viewMode = ViewMode::Day;
    QDate m_focusDate;

    void resizeTimeline();
    void paintDayView(QPainter &painter, const QFont &baseFont);
    void paintHourView(QPainter &painter, const QFont &baseFont);
    void drawBars(QPainter &painter, const QFont &f, int labelWidth,
                  int plotTop, int rowHeight, int barHeight,
                  const std::function<QPair<double, double>(const Bar &)> &geo);
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Formateur;
class Cours;

class GFormateurCours : public QMainWindow
{
    Q_OBJECT

public:
    explicit GFormateurCours(QWidget *parent = nullptr);
    ~GFormateurCours();

private slots:
    void onAjouterFormateur();
    void onModifierFormateur();
    void onSupprimerFormateur();
    void onViderFormateur();
    void onAjouterCours();
    void onModifierCours();
    void onSupprimerCours();
    void onViderCours();
    void onRechercherFormateurs();
    void onRechercherCours();
    void onActualiserStatsFormateurs();
    void onActualiserStatsCours();
    void onGenererPdfFormateur();
    void onGenererDocCours();
    void onActualiserPlanning();
    void onActualiserAlertes();
    void onFormateurSelected(const QModelIndex &index);
    void onCoursSelected(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSqlTableModel *modelFormateurs;
    QSqlTableModel *modelCours;
    QSqlQueryModel *modelSearchFormateurs;
    QSqlQueryModel *modelSearchCours;
    GanttWidget *m_gantt;
    bool m_planningDaySet = false;

    void initDatabase();
    void initModels();
    void loadFormateurs();
    void loadCours();
    void refreshFormateurStats();
    void refreshCoursStats();
    void setupPieHover(QPieSeries *series, QLabel *infoLabel, const QString &entity);
    void refreshPlanning();
    void refreshAlertes();
    void populateFormateurComboBoxes();
    void clearFormateurForm();
    void clearCoursForm();

    bool validateFormateur(const Formateur &formateur, QString &message) const;
    bool validateCours(const Cours &cours, QString &message) const;
    bool isSpecialiteCompatible(const QString &specialite, const QString &categorie) const;
    QString getFormateurSpecialite(int formateurId) const;
    QString conflictingCourseInfo(int formateurId,
                                  const QDate &start, const QTime &heureDebut,
                                  const QDate &end, const QTime &heureFin,
                                  int excludeCourseId) const;
    bool formateurHasFutureAssignments(int formateurId) const;
};

#endif // GFORMATEURCOURS_H
