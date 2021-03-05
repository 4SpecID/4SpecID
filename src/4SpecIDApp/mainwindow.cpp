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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    DatabaseConnector dbc;
    dbc.setup();
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
        ProjectSelectionDialog * psDialog =
                new ProjectSelectionDialog(this);
        psDialog->exec();
    });
    connect(ui->deleteProjectAction, &QAction::triggered, [this](){
        ProjectSelectionDialog * psDialog =
                new ProjectSelectionDialog(this, ProjectSelectionDialog::Action::DELETE);
        psDialog->exec();
    });

    connect(ui->newProjectAction, &QAction::triggered, [this](){
        onNewProject();
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
        DatabaseConnector dbc;
        bool success = dbc.createProject(projectName);
        if(!success) {
            QMessageBox::critical(this,
                                  QObject::tr("Could create project"),
                                  QObject::tr("Could not create project.\nClick Ok to exit."),
                                  QMessageBox::Ok);
        }
        else{
            QString filename = QFileDialog::getOpenFileName(
                        this,
                        tr("Open File"),
                        QDir::homePath(),
                        tr("SV (*.txt *.tsv)"), 0,
                        QFileDialog::DontUseNativeDialog
                        );
            if(filename.isEmpty()){
                return;
            }
            qDebug() << filename;

        }
    }
}
MainWindow::~MainWindow()
{

}
