#include "projectselection.h"
#include "ui_projectselection.h"
#include "databaseconnector.h"
#include <QStringListModel>
#include <QPushButton>
#include <qdebug.h>


ProjectSelectionDialog::ProjectSelectionDialog(MainWindow *parent, Action action) :
    QDialog(parent),
    ui(new Ui::ProjectSelectionDialog),
    m_parentProject("")
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

    connect(this, &ProjectSelectionDialog::getProject, [parent, &project = this->m_parentProject](){
        project = parent->getCurrentProject();
    });
    connect(this, &ProjectSelectionDialog::setProject, [parent](const QString& project){
        parent->setCurrentProject(project);
    });
}

void ProjectSelectionDialog::loadProjects(){
    QStringList projects;
    bool success = m_dbc.getProjects(projects);
    if(!success){
        QMessageBox::critical(this,
                          QObject::tr("Could not load projects"),
                          QObject::tr("Could not load projects.\nClick Ok to exit."),
                          QMessageBox::Ok);
    }
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
    emit getProject();
    if(!selectedProject.isEmpty() && selectedProject != m_parentProject){
        emit setProject(selectedProject);
        setResult(QDialog::Accepted);
        close();
    }else if(!selectedProject.isEmpty() && selectedProject == m_parentProject){
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
    QString selectedProject = getSelectedProject();
    emit getProject();
    if(selectedProject.isEmpty() || selectedProject == m_parentProject) {
        QMessageBox::critical(this,
                              "Cannot delete project.",
                              "Current project can't be deleted or no project selected.\nClick in Ok",
                              QMessageBox::StandardButton::Ok, QMessageBox::Ok);
        return;
    }else if(selectedProject.isEmpty()){
        return;
    }
    bool success = m_dbc.deleteProject(selectedProject);
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
