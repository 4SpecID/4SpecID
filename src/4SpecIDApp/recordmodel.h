#ifndef RECORDMODEL_H
#define RECORDMODEL_H
#include <QAbstractTableModel>
#include "qrecord.h"

class RecordModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    RecordModel(std::vector<QRecord> *records, QObject *parent = nullptr);
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int row, const QModelIndex &parent = QModelIndex()) override;
    bool remove;
public slots:
    void onRecordsChanged();
    void sortBySection(int col = 0);
signals:
    void actionPerformed();
    void updateGraph();
    void updateResults();
    void updateCombobox();

private:
    //QVector<record> records;
    size_t cur_count;
    std::vector<QRecord> *records;
    QVector<bool> sort_order;
    int last_col;
};

#endif // RECORDMODEL_H
