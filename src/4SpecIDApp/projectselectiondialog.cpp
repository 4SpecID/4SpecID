#include "projectselectiondialog.h"
#include "ui_projectselectiondialog.h"
#include "dbconnection.h"
#include <QStringListModel>
#include <QPushButton>
#include <qdebug.h>

ProjectSelectionDialog::ProjectSelectionDialog(QString app_dir, QString current_project, QStringList projects, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectSelectionDialog)
{
    ui->setupUi(this);
    this->app_dir = app_dir;
    this->current_project = current_project;
    QStringListModel *model = new QStringListModel();
    model->setStringList(projects);
    ui->projectList->setModel(model);
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(onCloseProjectSelection()));
}

LoadProjectSelectionDialog::LoadProjectSelectionDialog(QString app_dir, QString current_project, QStringList projects, QWidget *parent) :
    ProjectSelectionDialog(app_dir, current_project, projects, parent)
{
    ui->actionButton->setText("Load");
    connect(ui->actionButton, SIGNAL(clicked()), this, SLOT(loadProject()));
}

DeleteProjectSelectionDialog::DeleteProjectSelectionDialog(QString app_dir, QString current_project, QStringList projects, QWidget *parent) :
    ProjectSelectionDialog(app_dir, current_project, projects, parent)
{
    ui->actionButton->setText("Delete");
    connect(ui->actionButton, SIGNAL(clicked()), this, SLOT(deleteProject()));
}



void LoadProjectSelectionDialog::loadProject(){
    selected_project = getProject();
    if(!selected_project.isEmpty()){
        close();
        this->setResult(QDialog::Accepted);
    }
}
QString LoadProjectSelectionDialog::getProject(){
    auto selection = ui->projectList->selectionModel()->selectedIndexes();
    if(selection.size() == 0) return "";
    return selection[0].data(Qt::DisplayRole).toString();
}


void DeleteProjectSelectionDialog::deleteProject(){
    auto selection = ui->projectList->selectionModel()->selectedIndexes();
    if(selection.size() == 0) return;
    QString project = selection[0].data(Qt::DisplayRole).toString();
    if(project == current_project) {
        QMessageBox::critical(this, "Delete project", "Current project can't be deleted", QMessageBox::StandardButton::Ok, QMessageBox::Ok);
        return;
    }else if(project.isEmpty()){
        return;
    }
    DbConnection dbc(app_dir);
    if(!dbc.createConnection()) return;
    QString dropStat = "drop table %1";
    dropStat = dropStat.arg(project);
    dbc.execQuery(dropStat);
    QString dropStat2 = "drop table Ex%1";
    dropStat2 = dropStat2.arg(project);
    dbc.execQuery(dropStat2);
    QString deleteStat = "delete from projects where name = \"%1\"";
    deleteStat = deleteStat.arg(project);
    dbc.execQuery(deleteStat);
    ui->projectList->model()->removeRow(selection[0].row(),QModelIndex());
    close();
}

DeleteProjectSelectionDialog::~DeleteProjectSelectionDialog()
{
}

LoadProjectSelectionDialog::~LoadProjectSelectionDialog()
{
}

ProjectSelectionDialog::~ProjectSelectionDialog()
{
    delete ui;
}
void ProjectSelectionDialog::onCloseProjectSelection()
{
    close();
    this->setResult(QDialog::Rejected);
}
