#include <QItemSelectionRange>
#include <QDataStream>
#include "csv.hpp"
#include "recordmodel.h"
#include <qdebug.h>

RecordModel::RecordModel(std::vector<QRecord> *records, QObject *parent)
    : QAbstractTableModel(parent), cur_count(0), records(records)
{
    remove = false;
    sort_order = QVector<bool>(this->columnCount(), true);
    last_col = -1;
}




void RecordModel::sortBySection(int col){
    if(col== 0){
        if(last_col == col) sort_order[col]=!sort_order[col];
        if(sort_order[col]){
            std::sort(records->begin(), records->end(), [](QRecord a, QRecord b){
                return a.record.getSpeciesName().compare(b.record.getSpeciesName())<0;
            });
        }
        else{
            std::sort(records->begin(), records->end(), [](QRecord a, QRecord b){
                return a.record.getSpeciesName().compare(b.record.getSpeciesName())>0;
            });
        }
    }
    else if(col== 1){
        if(last_col == col) sort_order[col]=!sort_order[col];
        if(sort_order[col]){
            std::sort(records->begin(), records->end(), [](QRecord a, QRecord b){
                return a.record.getCluster().compare(b.record.getCluster())<0;
            });
        }
        else{
            std::sort(records->begin(), records->end(), [](QRecord a, QRecord b){
                return a.record.getCluster().compare(b.record.getCluster())>0;
            });
        }
    }
    else if(col== 2){
        if(last_col == col) sort_order[col]=!sort_order[col];
        if(sort_order[col]){
            std::sort(records->begin(), records->end(), [](QRecord a, QRecord b){
                return a.record.getGrade().compare(b.record.getGrade())<0;
            });
        }
        else{
            std::sort(records->begin(), records->end(), [](QRecord a, QRecord b){
                return a.record.getGrade().compare(b.record.getGrade())>0;
            });
        }
    }
    else if(col== 3){
        if(last_col == col) sort_order[col]=!sort_order[col];
        if(sort_order[col]){
            std::sort(records->begin(), records->end(), [](QRecord a, QRecord b){
                return a.record.getSource().compare(b.record.getSource())<0;
            });
        }
        else{
            std::sort(records->begin(), records->end(), [](QRecord a, QRecord b){
                return a.record.getSource().compare(b.record.getSource())>0;
            });
        }
    }
    else if(col== 4){
        if(last_col == col) sort_order[col]=!sort_order[col];
        if(sort_order[col]){
            std::sort(records->begin(), records->end(), [](QRecord a, QRecord b){
                return a.record.count() > b.record.count();
            });
        }
        else{
            std::sort(records->begin(), records->end(), [](QRecord a, QRecord b){
                return a.record.count() < b.record.count();
            });
        }
    }
    last_col = col;
    auto topLeft = this->index(0,0);
    auto rightBottom = this->index(records->size(),columnCount());
    emit dataChanged(topLeft, rightBottom, {Qt::DisplayRole});
}

int RecordModel::rowCount(const QModelIndex & /*parent*/) const
{
    return cur_count;//engine->getEntries().size();
}

int RecordModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 5;
}


QVariant RecordModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        QString sort = "";
        if(last_col == section){
            sort = sort_order[last_col] ? "(ASC)":"(DESC)";
        }
        switch (section) {
        case 0:
            return QString("Species") + sort;
        case 1:
            return QString("Bin")+sort;
        case 2:
            return QString("Grade")+sort;
        case 3:
            return QString("Source")+sort;
        case 4:
            return QString("Count")+sort;
        default:
            return QVariant();

        }
    }else if(role == Qt::DecorationRole && orientation == Qt::Horizontal)
    {
        switch (section) {
        case 0:
            return QString("Species");
        case 1:
            return QString("Bin");
        case 2:
            return QString("Grade");
        case 3:
            return QString("Source");
        case 4:
            return QString("Count");
        default:
            return QVariant();
        }
    }
    else if(role == Qt::DisplayRole && orientation == Qt::Vertical){
        return section;
    }
    return QVariant();
}

QVariant RecordModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    int size = records->size();


    if (!index.isValid())
        return QVariant();
    if (index.row() >= size)
        return QVariant();
    if (col == 0 && (role == Qt::DisplayRole ||
                     role == Qt::EditRole)) {
        return QString::fromStdString( records->at(row).record.getSpeciesName());
    }else if(col == 1 && (role == Qt::DisplayRole ||
                          role == Qt::EditRole)) {
        return QString::fromStdString( records->at(row).record.getCluster());
    }else if(col == 2 && (role == Qt::DisplayRole ||
                          role == Qt::EditRole)) {
        return QString::fromStdString( records->at(row).record.getGrade());
    }else if(col == 3 && role == Qt::DisplayRole){
        return QString::fromStdString( records->at(row).record.getSource());
    }else if(col == 4 && role == Qt::DisplayRole){
        return records->at(row).record.count();
    }
    return QVariant();
}


bool RecordModel::removeRow(int row, const QModelIndex &parent)
{
    bool success = removeRows(row,1,parent);
    return success;
}

bool RecordModel::removeRows(int position, int rows, const QModelIndex &parent){
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    for (int row=0; row < rows; ++row) {
        if(remove){
            records->erase(records->begin()+position);
            cur_count--;
        }
    }
    endRemoveRows();
    return true;
}

void RecordModel::onRecordsChanged(){
    beginResetModel();
    if(cur_count!=0){

        removeRows(0, cur_count, QModelIndex());
    }
    cur_count = records->size();
    insertRows(0, cur_count, QModelIndex());
    endResetModel();
}

bool RecordModel::setData(const QModelIndex &index, const QVariant &value, int role){
    int row = index.row();
    int col = index.column();
    if(role == Qt::EditRole){
        std::string field_name;
        auto field_value = value.toString().toStdString();
        if(field_value.empty()) return false;
        auto qrec = records->begin()+row;
        auto& entry = qrec->record;
        switch(col){
        case 0:
            if(field_value != entry.getSpeciesName()){
                emit actionPerformed();
                entry.setSpeciesName(field_value);
                qrec->modification += "Species name changed;";
                for (auto it = records->begin(); it!=records->end(); it++) {
                    if(it != qrec && qrec->joinRecord(*it)){
                        records->erase(qrec);
                        onRecordsChanged();
                        break;
                    }
                }
                emit dataChanged(index, index, {Qt::DisplayRole});
                emit updateGraph();
                emit updateCombobox();
            }
            break;
        case 1:
            if(field_value != entry.getCluster()){
                emit actionPerformed();
                entry.setCluster(field_value);
                qrec->modification += "Cluster changed;";
                for (auto it = records->begin(); it!=records->end(); it++) {
                    if(it != qrec && qrec->joinRecord(*it)){
                        records->erase(it);
                        onRecordsChanged();
                        break;
                    }
                }
                emit dataChanged(index, index, {Qt::DisplayRole});
                emit updateGraph();
                emit updateCombobox();
            }
            break;
        case 3:
            if(field_value != entry.getSource()){
                emit actionPerformed();
                entry.setSource(field_value);
                qrec->modification += "Source changed;";
                for (auto it = records->begin(); it!=records->end(); it++) {
                    if(it != qrec && qrec->joinRecord(*it)){
                        records->erase(it);
                        onRecordsChanged();
                        break;
                    }
                }
                emit dataChanged(index, index, {Qt::DisplayRole});
                emit updateGraph();
                emit updateCombobox();
            }
            break;
        default:
            return false;
        }



        return true;
    }
    return false;
}



Qt::ItemFlags RecordModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}


