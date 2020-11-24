#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include "4SpecIDApp/filterscrollarea.h"
#include "ui_filterdialog.h"
#include "qrecord.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FilterDialog; }
QT_END_NAMESPACE

class FilterDialog : public QDialog
{
    Q_OBJECT
    QStringList header;

public:
    FilterDialog(QStringList header, QList<QStringList> completions, QWidget *parent = nullptr);
    ~FilterDialog();
    using Func = std::function<bool(QRecord)>;
    Func getFilterFunc(){
        bool keep = ui->keepCheckBox->isChecked();
        auto func = fs->getFilterFunc();
        if(keep){
            func = [func](QRecord item) {return !func(item);};
        }
        return func;
    }


    bool accepted(){return ok;}

private slots:
    void onButtonboxRejected();
    void onButtonboxAccepted();

private:
    bool ok = false;
    FilterScrollArea *fs;
    Ui::FilterDialog *ui;

};
#endif // FILTERDIALOG_H
