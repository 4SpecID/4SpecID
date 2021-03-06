#include "gradingoptionsdialog.h"
#include "ui_gradingoptionsdialog.h"
#include <QMessageBox>
#include <qdebug.h>

GradingOptionsDialog::GradingOptionsDialog(ispecid::datatypes::Project& project , MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::GradingOptionsDialog),
    m_parameters(project.parameters)
{
    ui->setupUi(this);
    connect(this, &GradingOptionsDialog::updateGradingParameters,[parent](const ispecid::datatypes::GradingParameters& parameters){
        parent->setGradingParameters(parameters);
    });
    connect(ui->saveButton, &QPushButton::clicked, [this](){
        bool ok = checkValues();
        if(ok){
            qDebug() << "aqui";
            emit updateGradingParameters(m_parameters);
        }
    });
    connect(ui->okButton, &QPushButton::clicked, [this](){
        bool ok = checkValues();
        if(ok){
            emit updateGradingParameters(m_parameters);
            this->close();
        }
    });
    ui->sourcesText->setText(QString::number(m_parameters.minSources));
    ui->recordsText->setText(QString::number(m_parameters.minRecords));
    ui->distanceText->setText(QString::number(m_parameters.maxDistance));
}


void GradingOptionsDialog::showErrorMessage(QString error_name, QString error){
    QMessageBox::critical(this, error_name, error, QMessageBox::Cancel);
}

bool GradingOptionsDialog::checkValues(){
    double distance = m_parameters.maxDistance;
    int sources = m_parameters.minSources;
    int records = m_parameters.minRecords;
    bool ok = true;
    if(!ui->distanceText->text().isEmpty())
    {
        distance = ui->distanceText->text().toDouble(&ok);
        if(!ok ||(distance > 100 || distance < 0) ){
            showErrorMessage("Maximun distance error","Maximun distance error: Value should be between 0 and 100");
            return ok;
        }
    }
    if(!ui->sourcesText->text().isEmpty())
    {
        sources= ui->sourcesText->text().toInt(&ok);
        if(!ok || sources < 1){
            showErrorMessage("Minimum of laboratories deposited sequences error","Minimum of laboratories deposited sequences error: Value should be a positive integer value");
            return ok;
        }if(ok && sources == 1){
            QMessageBox::warning(this,"Minimum of laboratories deposited sequences error","Minimum of laboratories deposited sequences error: not advisable except to validate local repositories.",QMessageBox::Ok,QMessageBox::Ok);
        }
    }
    if(!ui->recordsText->text().isEmpty())
    {
        records= ui->recordsText->text().toInt(&ok);

        if(!ok || records < 3){
            showErrorMessage("Sequences deposited or published independently error", "Sequences deposited or published independently error: Value should be an integer value greater or equal 3");
            return ok;
        }
    }
    m_parameters.maxDistance = distance;
    m_parameters.minSources = sources;
    m_parameters.minRecords = records;
    return ok;
}


GradingOptionsDialog::~GradingOptionsDialog()
{
    delete ui;
}
