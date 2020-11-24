#include "4SpecIDApp/mainwindow.h"
#include <QMessageBox>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DbConnection dbc(a.applicationDirPath());
    if(dbc.createConnection()){
        dbc.setup();
        if(!dbc.success()){
            QMessageBox::critical(nullptr, QObject::tr("Cannot setup database"),
                                  QObject::tr("Error in setup database."), QMessageBox::Ok);
            return 1;
        }
    }else{
        return 1;
    }
    MainWindow w(a.applicationDirPath());
    w.show();
    return a.exec();
}
