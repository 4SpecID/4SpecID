#ifndef DATASETANALYSISDIALOG_H
#define DATASETANALYSISDIALOG_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include "qrecord.h"
QT_CHARTS_USE_NAMESPACE
namespace Ui {
class DatasetAnalysisDialog;
}

class DatasetAnalysisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatasetAnalysisDialog(QMap<QString, int> records_per_species,QMap<QString, int> records_per_bin,QMap<QString, QSet<QString>> sources_per_species,QMap<QString, QSet<QString>> sources_per_bin, QWidget *parent = nullptr);
    ~DatasetAnalysisDialog();
private slots:
    void saveCurrentTable();

private:
    Ui::DatasetAnalysisDialog *ui;
    void create_records_per_species(QMap<QString, int> records_per_species);
    void create_records_per_bin(QMap<QString, int> records_per_bin);
    void create_sources_per_species(QMap<QString, QSet<QString>> sources_per_species);
    void create_sources_per_bin(QMap<QString, QSet<QString>> sources_per_bin);
    void create(std::vector<QRecord>* data);
};

#endif // DATASETANALYSISDIALOG_H
