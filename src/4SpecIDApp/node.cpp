/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "node.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QTextItem>
#include <QFontDatabase>
#include <qdebug.h>


Node::Node(QString name, QString grade)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setFlag(ItemIsSelectable);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    this->name = name;
    this->grade = grade;
    setColor(grade);
    f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    f.setStyleHint(QFont::Monospace);
}

void Node::addEdge(Edge *edge)
{
    edge_set << edge;
    edge->adjust();
}

void Node::removeEdge(Edge *edge)
{
    edge_set.remove(edge);
}


QSet<Edge *> Node::edges()
{
    return edge_set;
}


QString Node::getName() const{
    return name;
}

void Node::setColor(QString grade){
    if(grade== "A"){
        mc = QColor(Qt::green);
        dmc = QColor(Qt::darkGreen);
    }
    else if( grade == "B"){
        mc = QColor(Qt::cyan);
        dmc = QColor(Qt::darkCyan);
    }
    else if( grade == "C"){
        mc = QColor(Qt::yellow);
        dmc = QColor(Qt::darkYellow);
    }
    else if( grade == "D"){
        mc = QColor(Qt::gray);
        dmc = QColor(Qt::darkGray);
    }
    else if( grade == "E"){
        mc = QColor(Qt::red);
        dmc = QColor(Qt::darkRed);
    }else if( grade == "U"){
        mc = QColor(Qt::blue);
        dmc = QColor(Qt::darkBlue);
    }else{
        mc = QColor(Qt::gray);
        dmc = QColor(Qt::black);
    }
    this->grade = grade;
    update();
}


QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    QRectF nodeRect( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
    QString text;
    if(grade.isEmpty()){
        text = this->name;
    }else{
        text = this->name + "(" + this->grade + ")";
    }
    QFontMetricsF fontMetrics(f);
    QRectF textRect = fontMetrics.boundingRect(text);
    textRect.setWidth(textRect.width()*1.5);
    return nodeRect.united(textRect);
}



QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}


void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setFont(f);
    painter->setPen(Qt::black);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);


    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken || isSelected()) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, mc.lighter(120));
        gradient.setColorAt(0, dmc.lighter(120));
    } else {
        gradient.setColorAt(0, mc);
        gradient.setColorAt(1, dmc);
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);

    QFontMetricsF fontMetrics(f);
    QString text;
    if(grade.isEmpty()){
        text = this->name;
    }else{
        text = this->name + "(" + this->grade + ")";
    }

    painter->setBrush(Qt::white);
    QRectF textRect = fontMetrics.boundingRect(text).translated(12,12);
    painter->setPen(Qt::black);
    textRect.setWidth(textRect.width()*1.5);
    painter->drawText(textRect, text);
}

void Node::setSelectNode(bool selected){
    setSelected(selected);
    update();
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{

    switch (change) {
    case ItemPositionHasChanged:
        for (Edge *edge : qAsConst(edge_set))
            edge->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::touch(){
    auto sc = scene();
    QRectF sc_rect = sceneBoundingRect();
    QSet<Node*> current;
    QSet<Node*> seen;
    QSet<Node*> next;
    seen << this;
    current << this;
    while(!current.isEmpty()){
        for(auto& n : current){
            for(auto& e : n->edges()){
                auto src = e->sourceNode();
                auto dest = e->destNode();
                if(!seen.contains(src) && !next.contains(src)){
                    sc_rect |= src->sceneBoundingRect();
                    next << src;
                    seen << src;
                }if(!seen.contains(dest) && !next.contains(dest)){
                    sc_rect |= dest->sceneBoundingRect();
                    next << dest;
                    seen << dest;
                }
            }
        }
        current.swap(next);
        next.clear();
    }
    sc->setSceneRect(sc_rect);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    touch();
    QGraphicsItem::mouseReleaseEvent(event);
}


bool Node::advancePosition()
{
    if (new_pos == pos())
        return false;
    setPos(new_pos);
    return true;
}

