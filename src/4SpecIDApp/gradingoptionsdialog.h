#ifndef GRADINGOPTIONSDIALOG_H
#define GRADINGOPTIONSDIALOG_H

#include <QDialog>
#include "datatypes.h"
#include "mainwindow.h"
namespace Ui {
class GradingOptionsDialog;
}

class GradingOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GradingOptionsDialog(ispecid::datatypes::Project& params, MainWindow *parent = nullptr);
    ~GradingOptionsDialog();

signals:
    void updateGradingParameters(const ispecid::datatypes::GradingParameters&);
private:
    void showErrorMessage(QString error_name, QString error);
    bool checkValues();
private:
    Ui::GradingOptionsDialog *ui;
    ispecid::datatypes::GradingParameters m_parameters;
};

#endif // GRADINGOPTIONSDIALOG_H
