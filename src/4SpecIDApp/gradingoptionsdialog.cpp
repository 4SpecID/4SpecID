#include "gradingoptionsdialog.h"
#include "ui_gradingoptionsdialog.h"
#include <QMessageBox>
#include <qdebug.h>

GradingOptionsDialog::GradingOptionsDialog(ispecid::datatypes::grading_parameters params , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GradingOptionsDialog)
{
    ui->setupUi(this);
    connect(ui->save_options_button, SIGNAL(clicked()),
            this, SLOT(onSaveOptionsButtonClicked()));
    connect(ui->ok_button, SIGNAL(clicked()),
            this, SLOT(onOkButtonClicked()));
    ui->min_lab_text->setText(QString::number(params.min_sources));
    ui->min_sequences_text->setText(QString::number(params.min_size));
    ui->max_dist_text->setText(QString::number(params.max_distance));
}


void showErrorMessage(QString error_name, QString error){
    QMessageBox::critical(nullptr, error_name, error, QMessageBox::Cancel);
}

bool GradingOptionsDialog::handleClick(){
    double dist = 2;
    int labs = 2;
    int seqs = 3;
    bool ok;
    if(!ui->max_dist_text->text().isEmpty())
    {
        dist = ui->max_dist_text->text().toDouble(&ok);
        if(!ok ||(dist > 100 || dist < 0) ){
            showErrorMessage("Maximun distance error","Value should be between 0 and 100");
            dist = 2;
        }
    }
    if(!ui->min_lab_text->text().isEmpty())
    {
        labs= ui->min_lab_text->text().toInt(&ok);
        if(!ok || labs < 1){
            showErrorMessage("Minimum of laboratories deposited sequences error","Value should be a positive integer value");
            labs = 2;
        }if(ok && labs == 1){
            QMessageBox::warning(nullptr,"Minimum of laboratories deposited sequences error","Warning: not advisable except to validate local repositories.",QMessageBox::Ok,QMessageBox::Ok);
        }
    }
    if(!ui->min_sequences_text->text().isEmpty())
    {
        seqs= ui->min_sequences_text->text().toInt(&ok);
        if(!ok || seqs < 3){
            showErrorMessage("Sequences deposited or published independently error", "Value should be an integer value greater or equal 3");
            seqs = 3;
        }
    }
    emit saveConfig(dist,labs,seqs);
    return true;
}


GradingOptionsDialog::~GradingOptionsDialog()
{
    delete ui;
}

void GradingOptionsDialog::onSaveOptionsButtonClicked()
{
    handleClick();
}

void GradingOptionsDialog::onOkButtonClicked()
{
    auto ok = handleClick();
    if(ok)
        this->close();
}
