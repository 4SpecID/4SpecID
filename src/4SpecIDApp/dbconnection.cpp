#include "dbconnection.h"
#include <QUuid>

DbConnection::DbConnection(){}
DbConnection::DbConnection(QString base_path){
    db = QSqlDatabase::addDatabase("QSQLITE",QUuid::createUuid().toString());
    auto db_dir = base_path+QDir::separator()+"ispeciddb";
    db.setDatabaseName(db_dir);
}
bool DbConnection::createConnection(){
    if (!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
                              QObject::tr("Unable to establish a database connection.\n"
                                          "This example needs SQLite support. Please read "
                                          "the Qt SQL driver documentation for information how "
                                          "to build it.\n\n"
                                          "Click Ok to exit."), QMessageBox::Ok);
        return false;
    }
    return true;
}

void DbConnection::setup(){
    QSqlQuery query(db);
    _success = query.exec("CREATE TABLE IF NOT EXISTS projects (id integer primary key autoincrement, "
                          "name varchar(255) not null, max_dist double, sources integer, records integer)");
    _success = _success && query.exec("CREATE TABLE IF NOT EXISTS neighbours"
                                      "(clusterA varchar(255) not null primary key,"
                                      "clusterB varchar(255) not null,"
                                      "distance real not null,"
                                      "time int not null)");
    if(!_success){
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
    }
}

QVector<QStringList> DbConnection::execQuery(QString statement){
    QStringList results;
    QSqlQuery query(db);
    QVector<QStringList> lst;
    if(query.exec(statement)){
        while (query.next())
        {
            QSqlRecord record = query.record();
            QStringList tmp;
            int count = record.count();
            for(int i=0; i < count; i++)
            {
                tmp << record.value(i).toString();
            }
            lst.append(tmp);
        }
        _success=true;
        lastQuery = query.lastQuery();
    }else{
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
        lastError = query.lastError().text();
        lastQuery = query.lastQuery();
        _success = false;
    }
    return lst;
}

DbConnection::~DbConnection(){
    db.close();
}
