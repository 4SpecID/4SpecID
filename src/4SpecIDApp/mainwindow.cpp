#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QStringList>
#include <QInputDialog>
#include <QKeyEvent>
#include <qdebug.h>
#include <QCompleter>
#include <QStringList>
#include <qtconcurrentrun.h>
#include "projectselection.h"
#include "gradingoptionsdialog.h"
#include "fileio.h"
#include "utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    m_dbc.setup();
    //    undoStack = new QUndoStack(this);
    //    movie = new QMovie(":/icons/dna_cursor.gif");
    //    timer = new QTimer(this);
    //    timer->setInterval(200); //Time in milliseconds
    //    connect(timer, &QTimer::timeout, this, [=](){
    //        QPixmap pix = movie->currentPixmap();
    //        movie->jumpToNextFrame();
    //        QCursor cs = QCursor(pix);
    //        this->setCursor(cs);
    //    });
    showMaximized();

    connect(ui->loadProjectAction, &QAction::triggered, [this](){
        //close current project
        ProjectSelectionDialog * psDialog =
                new ProjectSelectionDialog(this);
        psDialog->exec();
        if(m_project.name == "") return;
        bool success = m_dbc.loadProject(m_project);
        if(!success){
            QMessageBox::critical(this,
                                  QObject::tr("Could not load selected project"),
                                  QObject::tr("Could not load selected project\n.Click Ok to exit."),
                                  QMessageBox::Ok);
        }
        //loadRecords
    });
    connect(ui->deleteProjectAction, &QAction::triggered, [this](){
        ProjectSelectionDialog * psDialog =
                new ProjectSelectionDialog(this, ProjectSelectionDialog::Action::DELETE);
        psDialog->exec();
    });

    connect(ui->newProjectAction, &QAction::triggered, [this](){
        //close current project
        onNewProject();
    });

    connect(ui->gradindOptionsAction, &QAction::triggered, [this](){
        if(this->m_project.name.empty())
        {
            QMessageBox::critical( this,
                                   "No project loaded",
                                   "No project loaded",
                                   QMessageBox::StandardButton::Ok,
                                   QMessageBox::StandardButton::Ok );
            return;
        }
        GradingOptionsDialog * goDialog =
                new GradingOptionsDialog(m_project, this);
        goDialog->exec();
        this->m_dbc.updateProjectParameters(m_project);
    });
}

void MainWindow::onNewProject(){
    bool ok;
    QString projectName = QInputDialog::getText(this,
                                                tr("New project"),
                                                tr("Project name:"),
                                                QLineEdit::Normal,
                                                "",&ok);
    if(!ok){
        return;
    }
    else if(ok && projectName.isEmpty()){
        QMessageBox::critical( this,
                               "New project error",
                               "Error trying to create project",
                               QMessageBox::StandardButton::Ok,
                               QMessageBox::StandardButton::Ok );
        return;
    }
    else if(ok && !projectName.isEmpty()){
        setCurrentProject(projectName);
        qDebug() << QString::fromStdString(m_project.name);
        QString filename = QFileDialog::getOpenFileName(
                    this,
                    tr("Open File"),
                    QDir::homePath(),
                    tr("SV (*.txt *.tsv)"), 0
                    );
        if(filename.isEmpty()){
            return;
        }

        //HELPER
        //load records from file
        std::vector<std::string> header;
        auto records = ispecid::fileio::loadFile(filename.toStdString(), header, ispecid::fileio::Format::TSV);
        if(!utils::verifyHeader(header)){
            QMessageBox::critical(this,
                                  QObject::tr("Could create project"),
                                  QObject::tr("Could not create project, file does not have the correct format.\nClick Ok to exit."),
                                  QMessageBox::Ok);
            return;
        }

        bool hasGrade = false;
        bool hasModification = false;
        if(std::find(header.begin(), header.end(),"grade") != header.end()){
            hasGrade = true;
        }
        if(std::find(header.begin(), header.end(),"modification") != header.end()){
            hasModification = true;
        }
        bool success = m_dbc.createProject(m_project, header, records, hasGrade, hasModification, 500);
        if(!success) {
            QMessageBox::critical(this,
                                  QObject::tr("Could create project"),
                                  QObject::tr("Could not create project.\nClick Ok to exit."),
                                  QMessageBox::Ok);
        }
    }
}





MainWindow::~MainWindow()
{

}
