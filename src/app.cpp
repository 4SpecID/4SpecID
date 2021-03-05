#include "4SpecIDApp/mainwindow.h"
#include <QMessageBox>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
