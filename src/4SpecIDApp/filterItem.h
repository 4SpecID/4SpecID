#ifndef FILTERITEM_H
#define FILTERITEM_H

#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QCompleter>
#include <functional>
#include <qdebug.h>
class FilterItem : public QWidget
{
    Q_OBJECT
    QHBoxLayout *hlayout;
    QComboBox * groups;
    QComboBox * items;
    QList<QStringList> all_items;


public:
   explicit FilterItem(QList<QString> groups_items, QList<QStringList> all_items, QWidget *parent = Q_NULLPTR) :QWidget(parent){
        hlayout = new QHBoxLayout(this);
        hlayout->setObjectName(QString::fromUtf8("hlayout"));
        this->all_items = all_items;
        groups = new QComboBox(this);
        groups->addItems(groups_items);
        items =  new QComboBox(this);
        items->setEditable(true);
        QCompleter *comp = new QCompleter(this->all_items.at(groups->currentIndex()),items);
        comp->setCaseSensitivity(Qt::CaseInsensitive);
        items->setCompleter(comp);
        items->addItems(this->all_items.at(groups->currentIndex()));
        hlayout->addWidget(groups);
        hlayout->addWidget(items);
        connect(groups, SIGNAL(currentIndexChanged(int)),
                this,SLOT(onGroupChange(int)));
    }

    QString current_group(){
        return groups->currentText();
    }
    QString current_val(){
        return items->currentText();
    }

public slots:
    void onGroupChange(int index){
        auto current_comp = items->completer();
        delete current_comp;
        QCompleter *comp = new QCompleter(this->all_items.at(index),items);
        comp->setCaseSensitivity(Qt::CaseInsensitive);
        items->setCompleter(comp);
        items->clear();
        items->addItems(this->all_items.at(index));
    }

};
#endif // FILTERITEM_H
