QT       += core gui concurrent sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts



# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        app.cpp \
        4SpecIDApp/datasetanalysisdialog.cpp \
        4SpecIDApp/dbconnection.cpp \
        4SpecIDApp/mainwindow.cpp \
        4SpecIDApp/projectselectiondialog.cpp \
        4SpecIDApp/recordmodel.cpp \
        4SpecIDApp/resultsmodel.cpp \
        4SpecIDApp/node.cpp \
        4SpecIDApp/edge.cpp \
        4SpecIDApp/graphscene.cpp \
        4SpecIDApp/gradingoptionsdialog.cpp \
        4SpecIDApp/filterdialog.cpp \
        4SpecIDCore/src/auditor.cpp \
        4SpecIDCore/src/network.cpp


HEADERS += \
        4SpecIDApp/datasetanalysisdialog.h \
        4SpecIDApp/dbconnection.h \
        4SpecIDApp/projectselectiondialog.h \
        4SpecIDApp/recordmodel.h \
        4SpecIDApp/resultsmodel.h \
        4SpecIDApp/qrecord.h \
        4SpecIDApp/node.h \
        4SpecIDApp/edge.h \
        4SpecIDApp/graphscene.h \
        4SpecIDApp/filterItem.h \
        4SpecIDApp/gradingoptionsdialog.h \
        4SpecIDApp/filterdialog.h \
        4SpecIDApp/filterOp.h \
        4SpecIDApp/filterscrollarea.h \
        4SpecIDCore/include/iengine.h \
        4SpecIDCore/include/engine_algorithm.h \
        4SpecIDCore/include/auditor.h \
        4SpecIDCore/include/datatypes.h \
        4SpecIDCore/include/fileio.h \
        4SpecIDCore/include/network.h \
        4SpecIDCore/include/4Specid.h \
        4SpecIDCore/include/utils.h \
        4SpecIDCore/include/csv.hpp \
        4SpecIDApp/mainwindow.h


FORMS += \
    4SpecIDApp/datasetanalysisdialog.ui \
    4SpecIDApp/mainwindow.ui \
    4SpecIDApp/projectselectiondialog.ui \
    4SpecIDApp/gradingoptionsdialog.ui \
    4SpecIDApp/filterdialog.ui

RESOURCES += icons.qrc

win32:{
        contains(QT_ARCH, i386) {
            message("32-bit")
        } else {
            message("64-bit")
        }
        CONFIG += c++17
        INCLUDEPATH += "4SpecIDCore/include"
        INCLUDEPATH += $$PWD/../../../vcpkg/installed/x64-windows/include/boost

        LIBS += -L$$PWD/../../../vcpkg/installed/x64-windows/lib/ -lboost_thread-vc140-mt
        INCLUDEPATH += $$PWD/../../../vcpkg/installed/x64-windows/include
        DEPENDPATH += $$PWD/../../../vcpkg/installed/x64-windows/include

        LIBS += -L$$PWD/../../../vcpkg/installed/x64-windows/lib/ -lboost_regex-vc140-mt
        INCLUDEPATH += $$PWD/../../../vcpkg/installed/x64-windows/include
        DEPENDPATH += $$PWD/../../../vcpkg/installed/x64-windows/include

        LIBS += -L$$PWD/../../../vcpkg/installed/x64-windows/lib/ -llibcurl
        INCLUDEPATH += $$PWD/../../../vcpkg/installed/x64-windows/include
        DEPENDPATH += $$PWD/../../../vcpkg/installed/x64-windows/include

        RC_ICONS = icons/main_icon.ico
}
macx:{
        CONFIG += c++17 app_bundle
        INCLUDEPATH += "4SpecIDCore/include"
        #Boost
        INCLUDEPATH += "/usr/local/Cellar/boost/1.72.0_3/include/"
        LIBS += -L/usr/local/Cellar/boost/1.72.0_3/lib/ -lboost_regex -lboost_thread-mt
        #Curl
        LIBS += -L/usr/local/opt/curl/lib/ -lcurl.4
        INCLUDEPATH += "/usr/local/opt/curl/include"
        DEPENDPATH += "/usr/local/opt/curl/include"
        #ICON
        ICON = icons/main_icon.icns
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



