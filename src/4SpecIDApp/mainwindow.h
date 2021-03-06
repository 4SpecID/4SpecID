#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUndoStack>
#include <QTimer>
#include <QMovie>
#include <QFutureWatcher>

#include "databaseconnector.h"
#include "datatypes.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getCurrentProject(){ return QString::fromStdString(m_project.name); }
    void setGradingParameters(const ispecid::datatypes::GradingParameters& params){ m_project.parameters = params; }
    void setCurrentProject(const QString& project){ m_project.name = project.toStdString(); }

private:
   void onNewProject();
   Ui::MainWindow* ui;
   DatabaseConnector m_dbc;
   ispecid::datatypes::Project m_project;

};

#include <QUndoCommand>

/*
//! [0]
class ActionCommand : public QUndoCommand
{
public:

    ActionCommand(std::vector<QRecord> *data, std::vector<QRecord> old,
                  std::vector<QRecord> *deleted, std::vector<QRecord> old_d,
                  QUndoCommand *parent = nullptr): QUndoCommand(parent){
        this->old = old;
        this->current = *data;
        this->data = data;
        this->old_d = old_d;
        this->current_d = *deleted;
        this->deleted = deleted;
    }


    void undo() override{
        *data = old;
        *deleted = old_d;
    };
    void redo() override{
        *data = current;
        *deleted = current_d;
    };

private:
    std::vector<QRecord> *data;
    std::vector<QRecord> old;
    std::vector<QRecord> current;

    std::vector<QRecord> *deleted;
    std::vector<QRecord> old_d;
    std::vector<QRecord> current_d;
};
*/




#endif // MAINWINDOW_H
