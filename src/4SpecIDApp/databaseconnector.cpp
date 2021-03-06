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

bool DatabaseConnector::updateProjectParameters(const ispecid::datatypes::Project& project){
    QSqlQuery query(m_db);
    query.prepare(
    "UPDATE projects "
    "SET sources = :sources,"
        "records = :records,"
        "distance = :distance "
    "WHERE "
        "name = :project"
    );
    query.bindValue(":sources", project.parameters.minSources);
    query.bindValue(":records", project.parameters.minRecords);
    query.bindValue(":distance", project.parameters.maxDistance);
    query.bindValue(":project", QString::fromStdString(project.name));
    bool success = query.exec();
    if(!success) {
        qDebug() << QString("updateProjectParameters:") << query.lastError().text();
    }
    return success;
}

bool DatabaseConnector::getProjects(QStringList& projects){
    QSqlQuery query(m_db);
    bool success;
    success = query.exec("select name from projects");
    if(!success) {
        qDebug() << QString("getProjects:") << query.lastError().text();
        return false;
    }
    while(query.next()) {
        QString project = query.value(0).toString();
        projects << project;
    }
    return true;
}

bool DatabaseConnector::deleteProject(const QString& project/*default params*/){
    QSqlQuery query(m_db);
    query.prepare( "DELETE FROM projects WHERE name = :project_name;" );
    query.bindValue(":project_name", project);
    bool success = query.exec();
    if(!success) qDebug() << QString("deleteProject:") << query.lastError().text();
    return success;
}


bool DatabaseConnector::setup(){
    QSqlQuery query(m_db);
    bool success;
    success = query.exec("CREATE TABLE IF NOT EXISTS projects "
                          "(id integer primary key, "
                          "name varchar(255) not null unique,"
                          "sources integer, "
                          "records integer, "
                          "distance double)"
                        );

    if(!success){
        qDebug() << QString("setup projects tables:") << query.lastError().text();
        return success;
    }
    success = query.exec("CREATE TABLE IF NOT EXISTS neighbours"
                                      "(clusterA varchar(255) not null primary key,"
                                      "clusterB varchar(255) not null,"
                                      "distance real not null,"
                                      "time int not null)");
    if(!success){
        qDebug() << QString("setup neighbour projects table:") << query.lastError().text();
        return success;
    }
    return success;
}

bool DatabaseConnector::loadProject(ispecid::datatypes::Project& project){
    if(project.name == ""){
        return false;
    }
    QSqlQuery query(m_db);
    bool success = query.exec("select * from projects");
    if(!success){
        qDebug() << QString("loadProject:")<< query.lastError().text();
        return success;
    }
    if(!query.first()){
        qDebug() << QString("loadProject:")<< query.lastError().text();
        return success;
    }
    project.name = query.value(1).toString().toStdString();
    project.parameters.minSources = query.value(2).toInt();
    project.parameters.minRecords = query.value(3).toInt();
    project.parameters.maxDistance = query.value(4).toDouble();
    return success;
}



bool DatabaseConnector::createProject(const ispecid::datatypes::Project& project,
                                      std::vector<std::string>& header,
                                      std::vector<std::vector<std::string>>& records,
                                      bool hasGrade, bool hasModification, int batch){
    bool success = false;
    QString typed_columns, columns;
    for(auto& col : header){
        typed_columns += QString::fromStdString(col) + " varchar(50),";
        columns += QString::fromStdString(col) + ",";
    }
    if(!hasGrade){
        typed_columns += "grade varchar(50),";
        columns += "grade,";
    }
    if(!hasModification){
        typed_columns += "modification,";
        columns += "modification,";
    }

    typed_columns.chop(1);
    columns.chop(1);
    if(m_db.transaction())
    {
        QSqlQuery query(m_db);
        success = createProjectEntry(project);
        if(!success){
            m_db.rollback();
            return success;
        }

        success = createRecordsTable(project, columns);
        if(!success){
            m_db.rollback();
            return success;
        }

        success = insertProjectRecords(project, columns, records, hasGrade, hasModification, batch);
        if(!success){
            m_db.rollback();
            return success;
        }

        if(!m_db.commit())
        {
            m_db.rollback();
            return false;
        }
    }
    else
    {
        qDebug() << "Failed to start transaction mode";
    }

    return success;
}


bool DatabaseConnector::createProjectEntry(const ispecid::datatypes::Project& project){
    QString createProjectStatement = "insert into projects (name,sources,records, distance) values (\"%1\",%2,%3,%4)";
    createProjectStatement = createProjectStatement.arg(QString::fromStdString(project.name))
            .arg(project.parameters.minSources)
            .arg(project.parameters.minRecords)
            .arg(project.parameters.maxDistance);
    QSqlQuery query(m_db);
    bool success = query.exec(createProjectStatement);
    if(!success) qDebug() << QString("createProjectEntry:") << query.lastError().text();
    return success;
}

bool DatabaseConnector::createRecordsTable(const ispecid::datatypes::Project & project, const QString &columns){
    QSqlQuery query(m_db);
    bool success;
    QString createTableStatement("CREATE TABLE IF NOT EXISTS %1"
                                  "(id integer primary key, %2)");


    createTableStatement = createTableStatement.arg(QString::fromStdString(project.name), columns);
    success = query.exec(createTableStatement);
    if(!success){
        qDebug() << QString("createRecordsTable:") << query.lastError().text();
        qDebug() << query.lastQuery();
    }
    return success;
}

bool DatabaseConnector::insertProjectRecords(const ispecid::datatypes::Project& project,
                                             const QString& columns,
                                             std::vector<std::vector<std::string>>& records,
                                             bool hasGrade, bool hasModification, int batch){
    int size = 0;
    bool success = true;
    QSqlQuery query(m_db);
    QString values;
    QString insertStatementBase = "INSERT INTO "+QString::fromStdString(project.name)+" ("+columns+") VALUES ";
    for(auto& row : records){
        if(!hasGrade) row.push_back("U");
        if(!hasModification) row.push_back("");
        QString curValue = "(";
        for(auto& col : row){
            curValue += "\""+ QString::fromStdString(col) + "\",";
            qDebug() << QString::fromStdString(col);
        }
        qDebug() << curValue;
        curValue.chop(1);
        curValue += ")";
        values += curValue;
        if(size == batch){
            success = query.exec(insertStatementBase + values);
            if(!success){
                qDebug() << query.lastQuery();
                qDebug() << QString("insertProjectRecords:") << query.lastError().text();
                return success;
            }
            size = 0;
        }else{
            values += ",";
            size++;
        }
    }

    values.chop(1);
    success = query.exec(insertStatementBase + values);
    if(!success){
        qDebug() << query.lastQuery();
        qDebug() << QString("insertProjectRecords:") << query.lastError().text();
    }
    return success;
}


DatabaseConnector::~DatabaseConnector(){
    m_db.close();
}
