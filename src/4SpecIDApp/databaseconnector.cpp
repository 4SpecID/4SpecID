#include "databaseconnector.h"
#include <QUuid>
#include <QDebug>

DatabaseConnector::DatabaseConnector(){
    m_db = QSqlDatabase::addDatabase("QSQLITE",QUuid::createUuid().toString());
    auto db_dir = QDir::currentPath()+QDir::separator()+"4specid.sqlite";
    m_db.setDatabaseName(db_dir);
    if (!m_db.open()) {
        QMessageBox::critical(nullptr,
                              QObject::tr("Cannot open database"),
                              QObject::tr("Unable to establish a database connection.\nClick Ok to exit."),
                              QMessageBox::Ok);
        throw std::runtime_error("Could not connect to database");
    }
}

QStringList DatabaseConnector::getProjects(){
    QSqlQuery query(m_db);
    bool success;
    success = query.exec("select name from projects");
    if(!success) {
        QMessageBox::critical(nullptr,
                              QObject::tr("Could not load projects"),
                              QObject::tr("Could not load projects\n.Click Ok to exit."),
                              QMessageBox::Ok);
        throw std::runtime_error(query.lastError().text().toStdString());
    }
    QStringList result;
    while(query.next()) {
        QString project = query.value(0).toString();
        result << project;
    }
    return result;
}

bool DatabaseConnector::deleteProject(const QString& project/*default params*/){
    QSqlQuery query(m_db);
    query.prepare(
            "DELETE FROM projects "
            "WHERE name = :project_name;"
    );
    query.bindValue(":project_name", project);
    bool success = query.exec();
    if(!success) qDebug() << query.lastError().text();
    return success;
}

bool DatabaseConnector::createProject(const QString& project){
    QString createProjectStatement = "insert into projects (name,max_dist,sources,records) values (\"%1\",%2,%3,%4)";
    createProjectStatement = createProjectStatement.arg(project).arg(1).arg(1).arg(1);
    QSqlQuery query(m_db);
    bool success = query.exec(createProjectStatement);
    if(!success) qDebug() << query.lastError().text();
    return success;
}

void DatabaseConnector::setup(){
    QSqlQuery query(m_db);
    bool success;
    success = query.exec("CREATE TABLE IF NOT EXISTS projects "
                          "(id integer primary key, "
                          "name varchar(255) not null unique,"
                          "max_dist double, "
                          "sources integer, "
                          "records integer)"
                        );
    if(!success)
        throw std::runtime_error(query.lastError().text().toStdString());

    success = query.exec("CREATE TABLE IF NOT EXISTS neighbours"
                                      "(clusterA varchar(255) not null primary key,"
                                      "clusterB varchar(255) not null,"
                                      "distance real not null,"
                                      "time int not null)");
    if(!success)
        throw std::runtime_error(query.lastError().text().toStdString());
}

DatabaseConnector::~DatabaseConnector(){
    m_db.close();
}
