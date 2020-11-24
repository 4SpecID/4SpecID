#include "datasetanalysisdialog.h"
#include "ui_datasetanalysisdialog.h"
#include <qtconcurrentrun.h>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFileDialog>
#include "mainwindow.h"



void DatasetAnalysisDialog::create_records_per_species(QMap<QString, int> records_per_species){
    auto sz = records_per_species.size();
    auto keys = records_per_species.keys();
    QStandardItemModel* table_model = new QStandardItemModel(sz, 2);
    int row = 0;
    for (auto& key : keys) {

        QStandardItem *value_1 = new QStandardItem(key); // you should set your data here (in this case as a string)
        QStandardItem *value_2 = new QStandardItem(QString::number(records_per_species[key]));
        table_model->setItem(row, 0, value_1);
        table_model->setItem(row, 1, value_2);
        row++;
    }
    table_model->setHeaderData( 0, Qt::Horizontal, QObject::tr("Species") );
    table_model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Number of records") );
    ui->table_1->horizontalHeader()->setStretchLastSection(true);
    ui->table_1->setModel(table_model);
    ui->table_1->resizeColumnsToContents();
}


void DatasetAnalysisDialog::create_records_per_bin(QMap<QString, int> records_per_bin){
    auto sz = records_per_bin.size();
    auto keys = records_per_bin.keys();
    QStandardItemModel* table_model = new QStandardItemModel(sz, 2);
    int row = 0;
    for (auto& key : keys) {

        QStandardItem *value_1 = new QStandardItem(key); // you should set your data here (in this case as a string)
        QStandardItem *value_2 = new QStandardItem(QString::number(records_per_bin[key]));
        table_model->setItem(row, 0, value_1);
        table_model->setItem(row, 1, value_2);
        row++;
    }
    table_model->setHeaderData( 0, Qt::Horizontal, QObject::tr("BIN") );
    table_model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Number of records") );
    ui->table_2->horizontalHeader()->setStretchLastSection(true);
    ui->table_2->setModel(table_model);
    ui->table_2->resizeColumnsToContents();
}
void DatasetAnalysisDialog::create_sources_per_species(QMap<QString, QSet<QString>> sources_per_species){
    auto sz = sources_per_species.size();
    auto keys = sources_per_species.keys();
    QStandardItemModel* table_model = new QStandardItemModel(sz, 2);
    int row = 0;
    for (auto& key : keys) {

        QStandardItem *value_1 = new QStandardItem(key); // you should set your data here (in this case as a string)
        QStandardItem *value_2 = new QStandardItem(QString::number(sources_per_species[key].size()));
        table_model->setItem(row, 0, value_1);
        table_model->setItem(row, 1, value_2);
        row++;
    }
    table_model->setHeaderData( 0, Qt::Horizontal, QObject::tr("Species") );
    table_model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Number of sources") );
    ui->table_3->horizontalHeader()->setStretchLastSection(true);
    ui->table_3->setModel(table_model);
    ui->table_3->resizeColumnsToContents();
}
void DatasetAnalysisDialog::create_sources_per_bin(QMap<QString, QSet<QString>> sources_per_bin){
    auto sz = sources_per_bin.size();
    auto keys = sources_per_bin.keys();
    QStandardItemModel* table_model = new QStandardItemModel(sz, 2);
    int row = 0;
    for (auto& key : keys) {

        QStandardItem *value_1 = new QStandardItem(key); // you should set your data here (in this case as a string)
        QStandardItem *value_2 = new QStandardItem(QString::number(sources_per_bin[key].size()));
        table_model->setItem(row, 0, value_1);
        table_model->setItem(row, 1, value_2);
        row++;
    }
    table_model->setHeaderData( 0, Qt::Horizontal, QObject::tr("BIN") );
    table_model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Number of sources") );
    ui->table_4->horizontalHeader()->setStretchLastSection(true);
    ui->table_4->setModel(table_model);
    ui->table_4->resizeColumnsToContents();
    //    auto keys = sources_per_bin.keys();
    //    QBarSeries *series = new QBarSeries();
    //    for (auto& key : keys){
    //        QBarSet *bar = new QBarSet(key);
    //        *bar << sources_per_bin[key].size();
    //        series->append(bar);
    //    }
    //    QChart *chart = new QChart();
    //    chart->addSeries(series);
    //    chart->setTitle("Number of sources per bin");
    //    chart->setAnimationOptions(QChart::SeriesAnimations);
    //    QStringList categories;
    //    categories << "Species";
    //    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    //    axisX->append(categories);
    //    chart->addAxis(axisX, Qt::AlignBottom);
    //    series->attachAxis(axisX);

    //    QValueAxis *axisY = new QValueAxis();
    ////    axisY->setRange(0,15);
    //    chart->addAxis(axisY, Qt::AlignLeft);
    //    series->attachAxis(axisY);
    //    chart->legend()->setVisible(true);
    //    chart->legend()->setAlignment(Qt::AlignBottom);
    //    ui->chart_4->setChart(chart);
    //    ui->chart_4->setRenderHint(QPainter::Antialiasing);
}


DatasetAnalysisDialog::DatasetAnalysisDialog(QMap<QString, int> records_per_species,QMap<QString, int> records_per_bin,QMap<QString, QSet<QString>> sources_per_species,QMap<QString, QSet<QString>> sources_per_bin, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatasetAnalysisDialog)
{

    ui->setupUi(this);
    create_records_per_species(records_per_species);
    create_records_per_bin(records_per_bin);
    create_sources_per_species(sources_per_species);
    create_sources_per_bin(sources_per_bin);
    ui->tabWidget->setTabText(0,"Records per species");
    ui->tabWidget->setTabText(1,"Records per BIN");
    ui->tabWidget->setTabText(2,"Institution per species");
    ui->tabWidget->setTabText(3,"Institution per BIN");
    ui->table_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->save_table_button, SIGNAL(clicked()),
            this, SLOT(saveCurrentTable()));
}

void DatasetAnalysisDialog::saveCurrentTable(){
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save "), "",
                                                    tr("(*.txt);;All Files (*)"),
                                                    0,
                                                    QFileDialog::DontUseNativeDialog);
    qDebug() << fileName;
    if (fileName.isEmpty())
        return;
    else {
        QtConcurrent::run(std::function<void(QString)>(
                              [this](QString fileName){
                              auto index = ui->tabWidget->currentIndex();
                              auto tab = ui->tabWidget->widget(index);
                              auto table = (QTableView *)tab->children()[1];
                              auto table_model = table->model();
                              int row = table_model->rowCount();
                              QFile file(fileName+".txt");
                              if (!file.open(QIODevice::WriteOnly)) {
                                  QString error_msg = "File %1 couldn't be open.";
                                  //								  QMessageBox::critical( this, error_type, error, QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Ok );
                                  return;
                              }
                              QTextStream stream(&file);
                              stream
                                  << table_model->headerData(0, Qt::Horizontal).toString()
                                  << "\t"
                                  <<  table_model->headerData(1, Qt::Horizontal).toString()
                                  << "\n";
                              for(int i = 0; i < row; i++){
                                  auto key = table_model->index(i, 0);
                                  auto value = table_model->index(i, 1);
                                  auto key_str = table_model->data(key).toString();
                                  auto value_str = table_model->data(value).toString();
                                  stream << key_str << "\t" << value_str << "\n";
                              }
                              file.close();
                          }), fileName);
    }
}


DatasetAnalysisDialog::~DatasetAnalysisDialog()
{
    delete ui;
}
