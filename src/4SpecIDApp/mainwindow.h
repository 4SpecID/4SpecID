#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUndoStack>
#include <QTimer>
#include <QMovie>
#include <QFutureWatcher>

#include "databaseconnector.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    const QString& getCurrentProject(){ return m_projectName; }
    void setCurrentProject(QString& project){ m_projectName = project; }

private:
   void onNewProject();
   Ui::MainWindow* ui;
   QString m_projectName;
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
