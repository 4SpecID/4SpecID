#include "projectselection.h"
#include "ui_projectselection.h"
#include "databaseconnector.h"
#include <QStringListModel>
#include <QPushButton>
#include <qdebug.h>


ProjectSelectionDialog::ProjectSelectionDialog(MainWindow *parent, Action action) :
    QDialog(parent),
    ui(new Ui::ProjectSelectionDialog)
{
    ui->setupUi(this);
    if(action == Action::LOAD){
        setupLoad();
    }
    if(action == Action::DELETE){
        setupRemove();
    }

    loadProjects();
    connect(ui->cancelButton, &QPushButton::clicked, [this](){
        setResult(QDialog::Rejected);
        close();
    });
}

void ProjectSelectionDialog::loadProjects(){
    DatabaseConnector dbc;
    QStringList projects = dbc.getProjects();
    QStringListModel *model = new QStringListModel(projects);
    ui->projectList->setModel(model);
}

void ProjectSelectionDialog::setupLoad()
{
    ui->actionButton->setText("Load");
    connect(ui->actionButton, &QPushButton::clicked, [this](){
        this->load();
    });
}

void ProjectSelectionDialog::setupRemove()
{
    ui->actionButton->setText("Delete");
    connect(ui->actionButton, &QPushButton::clicked, [this](){
        this->remove();
    });
}



void ProjectSelectionDialog::load(){
    QString selectedProject = getSelectedProject();
    QString openProject = static_cast<MainWindow*>(this->parentWidget())->getCurrentProject();
    if(!selectedProject.isEmpty() && selectedProject != openProject){
        static_cast<MainWindow*>(this->parentWidget())->setCurrentProject(selectedProject);
        setResult(QDialog::Accepted);
        close();
    }else if(selectedProject == openProject){
        QMessageBox::warning(this,
                             QObject::tr("Project already loaded"),
                             QObject::tr("Project already loaded.\nClick Ok to exit."),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
    }
}

const QString ProjectSelectionDialog::getSelectedProject(){
    auto selectedIdxs = ui->projectList->selectionModel()->selectedIndexes();
    if (!selectedIdxs.isEmpty()) {
        const QVariant var = ui->projectList->model()->data(selectedIdxs.first());
        const QString selectedItemString = var.toString();
        return selectedItemString;
    }
    return "";
}


void ProjectSelectionDialog::remove(){
    DatabaseConnector dbc;
    QString selectedProject = getSelectedProject();
    QString openProject = static_cast<MainWindow*>(this->parentWidget())->getCurrentProject();
    if(selectedProject.isEmpty() || selectedProject == openProject) {
        QMessageBox::critical(this,
                              "Cannot delete project.",
                              "Current project can't be deleted or no project selected.\nClick in Ok",
                              QMessageBox::StandardButton::Ok, QMessageBox::Ok);
        return;
    }else if(selectedProject.isEmpty()){
        return;
    }
    bool success = dbc.deleteProject(selectedProject);
    if(!success) {
        QMessageBox::critical(this,
                              QObject::tr("Could delete project"),
                              QObject::tr("Could not delete project\n.Click Ok to exit."),
                              QMessageBox::Ok);
        return;
    }
    setResult(QDialog::Accepted);
    close();
}

ProjectSelectionDialog::~ProjectSelectionDialog()
{
    delete ui;
}
