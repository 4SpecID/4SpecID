#ifndef PROJECTSELECTION_H
#define PROJECTSELECTION_H

#include <QDialog>
#include "databaseconnector.h"
#include "mainwindow.h"

namespace Ui {
class ProjectSelectionDialog;
}


class ProjectSelectionDialog : public QDialog
{
    Q_OBJECT

public:
enum class Action{
    LOAD,
    DELETE
};
    explicit ProjectSelectionDialog(MainWindow *parent = nullptr, Action action = Action::LOAD);
    ~ProjectSelectionDialog();
private:
    const QString getSelectedProject();
    void load();
    void remove();
    void setupLoad();
    void setupRemove();
    void loadProjects();

    Ui::ProjectSelectionDialog *ui;

};

#endif // PROJECTSELECTION_H
