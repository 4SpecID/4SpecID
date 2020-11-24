#ifndef PROJECTSELECTIONDIALOG_H
#define PROJECTSELECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class ProjectSelectionDialog;
}

class ProjectSelectionDialog : public QDialog
{
    Q_OBJECT

    public:
    explicit ProjectSelectionDialog(QString app_dir,QString current_project, QStringList projects, QWidget *parent = nullptr);
    ~ProjectSelectionDialog();
    Ui::ProjectSelectionDialog *ui;
    QString app_dir;
    QString current_project;
public slots:
    void onCloseProjectSelection();
};


class LoadProjectSelectionDialog : public ProjectSelectionDialog
{
    Q_OBJECT

public:
    explicit LoadProjectSelectionDialog(QString app_dir,QString current_project, QStringList projects, QWidget *parent = nullptr);
    QString getProject();
    ~LoadProjectSelectionDialog();
    QString selected_project ="";
public slots:
    void loadProject();
};


class DeleteProjectSelectionDialog : public ProjectSelectionDialog
{
    Q_OBJECT

public:
    explicit DeleteProjectSelectionDialog(QString app_dir,QString current_project, QStringList projects, QWidget *parent = nullptr);
    ~DeleteProjectSelectionDialog();
public slots:
    void deleteProject();

};


#endif // PROJECTSELECTIONDIALOG_H
