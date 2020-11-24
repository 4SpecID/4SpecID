//#include <math.h>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QContextMenuEvent>
#include <QMenu>
#include <qdebug.h>
#include "graphscene.h"
#include "utils.h"
#include <unordered_set>
#include <QApplication>



GraphScene::GraphScene(QWidget *parent, std::vector<QRecord> * records)
    : records(records)
{
    setParent(parent);
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(QRectF(0, 0, 0, 0));
    cur_node_key = "";
}


void GraphScene::generateItems(){
    for(auto& qrec: *records){
        auto species_name = qrec.record.getSpeciesName();
        auto cluster_name = qrec.record.getCluster();
        auto grade = qrec.record.getGrade();
        auto node_it = nodes.find(QString::fromStdString(species_name));
        auto cluster_it = nodes.find(QString::fromStdString(cluster_name));
        Node *node;
        Node *cluster;
        if(node_it == nodes.end()){
            node = new Node(QString::fromStdString(species_name), QString::fromStdString(grade));
            node->hide();
            this->addItem(node);
            nodes.insert(node->getName(), node);
        }else{
            node = qgraphicsitem_cast<Node *>(*node_it);
        }
        if(cluster_it == nodes.end()){
            cluster = new Node(QString::fromStdString(cluster_name), "");
            cluster->hide();
            this->addItem(cluster);
            nodes.insert(cluster->getName(), cluster);
        }else{
            cluster = qgraphicsitem_cast<Node *>(*cluster_it);
        }
        auto edges = node->edges();
        Edge* edge = nullptr;
        for(auto& e : edges){
            if(e != nullptr && node == e->sourceNode() && cluster == e->destNode()){
                edge = e;
            }
        }
        if(edge == nullptr){
            edge = new Edge(node,cluster, qrec.record.count());
            connect(edge, SIGNAL(removeEdge(Edge *)),
                    this, SLOT(onRemoveEdge(Edge *)));
            edges << edge;
            edge->hide();
            this->addItem(edge);
        }	else{
            edge->addCount(qrec.record.count());
        }
    }
    update();
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent* pMouseEvent) {
    QGraphicsItem* pItemUnderMouse = itemAt( pMouseEvent->scenePos().x(), pMouseEvent->scenePos().y(), QTransform());
    if (pItemUnderMouse != nullptr && pItemUnderMouse->isEnabled() &&
            pMouseEvent->modifiers() &  Qt::ControlModifier &&
            pItemUnderMouse->flags() & QGraphicsItem::ItemIsSelectable){
        auto m = qgraphicsitem_cast<Node *>(pItemUnderMouse);
        m->setSelectNode(!pItemUnderMouse->isSelected());
        return;
    }
    QGraphicsScene::mousePressEvent(pMouseEvent);
}

void GraphScene::onGraphChanged(){
    clearScene();
    clean();
    generateItems();
    setSceneRect(itemsBoundingRect());                          // Re-shrink the scene to it's bounding contents
}


void GraphScene::onGraphColorChanged(){
    for(auto& qrec: *records){
        auto key = QString::fromStdString(qrec.record.getSpeciesName());
        auto node = qgraphicsitem_cast<Node *>(nodes[key]);
        node->setColor(QString::fromStdString(qrec.record.getGrade()));
    }
    update();
}



void GraphScene::onRemoveEdge(Edge *edge){
    emit actionPerformed();
    auto src = edge->sourceNode();
    auto dest = edge->destNode();
    auto species = src->getName();
    auto species_str = src->getName().toStdString();
    auto bin = dest->getName();
    auto bin_str = dest->getName().toStdString();
    setComponentVisibleDFS(edge->destNode(), false);
    setComponentVisibleDFS(edge->sourceNode(), false);
    if(src->edges().count() == 0){
        delete nodes[species];
    }
    if(dest->edges().count() == 0){
        delete nodes[bin];
    }
    delete edge;


    cur_node_key = "";
    emit deleteCells(species_str, bin_str);
}

QList<Node*> getDestNode(QSet<Edge*> edges){
    QList<Node*> nodes;
    for (auto e : edges) {
        nodes << e->destNode();
    }
    return nodes;
}
QList<Node*> getSrcNode(QSet<Edge*> edges){
    QList<Node*> nodes;
    for (auto e : edges) {
        nodes << e->sourceNode();
    }
    return nodes;
}

QList<Node*> getNextNodes(Node* node){
    auto edges = node->edges();
    if( !edges.empty() ){
        auto e = edges.begin();
        if( (*e)->destNode() == node){
            return getSrcNode(edges);
        }
        else{
            return getDestNode(edges);
        }
    }
    return QList<Node*>();
}

void GraphScene::setComponentVisibleDFS( Node *root, bool visible){
    QList<Node*> next;
    QList<Node*> current;
    current << root;
    qreal stride_w = 150;
    qreal stride_h = 40;
    qreal sub_stride_h = 30;
    qreal ax = 0;
    qreal ay = -100;
    while(current.size() > 0){
        for (auto node : current){
            if(node->isVisible() != visible){
                node->setVisible(visible);
                auto nodes = getNextNodes(node);
                next += nodes;
                node->setPos(ax,ay);
                ax += stride_w;
                ay += sub_stride_h;
                auto edges = node->edges();
                for(auto e : edges){
                    e->setVisible(visible);
                }
            }
        }
        current = next;

        next = QList<Node*>();
        ax = 0;
        ay += stride_h;
    }
    root->touch();
}



void GraphScene::setComponentVisible(QString key){
    if(!key.isEmpty()) {
        if(!cur_node_key.isEmpty()){
            auto hide_root = qgraphicsitem_cast<Node *>(nodes[cur_node_key]);
            if(hide_root->getName() != key){
                setComponentVisibleDFS(hide_root, false);
            }
        }
        cur_node_key = key;
        auto show_root = qgraphicsitem_cast<Node *>(nodes[cur_node_key]);
        setComponentVisibleDFS(show_root, true);
    }
}

void GraphScene::clearScene(){
    const QList<QGraphicsItem *> items = this->items();
    for (QGraphicsItem *item : items) {
        if(item != nullptr){
            this->removeItem(item);
            delete item;
        }
    }
}

void GraphScene::clean(){
    this->nodes.clear();
    this->edges.clear();
    cur_node_key = "";
}


GraphScene::~GraphScene()
{
    clearScene();
}

void GraphScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    Q_UNUSED(painter);
}

