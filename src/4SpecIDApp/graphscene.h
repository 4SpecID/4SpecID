#ifndef GRAPHVIEWER_H
#define GRAPHVIEWER_H
#include <QGraphicsView>
#include <QGraphicsItem>
#include "qrecord.h"
#include "edge.h"
#include "node.h"

class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphScene(QWidget *parent, std::vector<QRecord>* records);
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* pMouseEvent) override;
    ~GraphScene();

public slots:
    void onRemoveEdge(Edge *);
    void setComponentVisible(QString key = QString::fromStdString(""));
    void onGraphChanged();
    void onGraphColorChanged();

signals:
    void deleteCells(std::string, std::string);
    void updateRecords();
    void updateResults();
    void updateCombobox();
    void actionPerformed();

private:
    void clean();
    void clearScene();
    void setComponentVisibleDFS( Node *root, bool visible = true);
    void drawComponent(Node *root, bool visible =  true);
    void generateItems();
    std::vector<QRecord> * records;
    QMap<QString, QGraphicsItem*> nodes;
    QVector<QGraphicsItem*> edges;
    QString cur_node_key;
};

#endif // GRAPHVIEWER_H
