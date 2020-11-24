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

#include "edge.h"
#include <qdebug.h>
#include <iostream>
#include <QPainter>
//#include <QtMath>
#include <QFontDatabase>

Edge::Edge(Node *sourceNode, Node *destNode, int count)
    : source(sourceNode), dest(destNode)
{
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setAcceptHoverEvents(true);
    setZValue(1);
    source->addEdge(this);
    dest->addEdge(this);
    adjust();
    this->count = count;
    f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    f.setStyleHint(QFont::Monospace);
}


void Edge::addCount(int count){
    this->count += count;
}

Node *Edge::sourceNode()
{
    return source;
}

Node *Edge::destNode()
{
    return dest;
}



void Edge::adjust()
{
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
        auto mid =line.center();
        midPoint = QPointF(mid.x(), mid.y()+20);
    } else {
        sourcePoint = destPoint = midPoint = line.p1();
    }
}



QRectF Edge::boundingRect() const
{
    if (!source || !dest)
            return QRectF();

        qreal penWidth = 1;
        qreal extra = (penWidth + arrowSize) / 2.0;
        auto countStr = QString::number(count);
        QFontMetricsF fontMetrics(f);
        QRectF text = fontMetrics.boundingRect(countStr);
        text.translate(midPoint.x(),midPoint.y());
        text.setWidth(text.width()*1.5);
        return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                          destPoint.y() - sourcePoint.y()))
                .united(text)
                .normalized()
                .adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(m_pen);
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;
    painter->drawLine(line);
    auto countStr = QString::number(count);
    QFontMetricsF fontMetrics(f);
    QRectF textRect = fontMetrics.boundingRect(countStr);
    painter->setPen(Qt::black);
    textRect.translate(midPoint.x(),midPoint.y());
    textRect.setWidth(textRect.width()*1.5);
    m_pen.setColor(Qt::black);
    painter->drawText(textRect, Qt::AlignCenter, countStr);
    painter->drawRect(textRect);
}


void Edge::remove(){
    dest->removeEdge(this);
    source->removeEdge(this);
    emit removeEdge(this);
}


void Edge::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *removeAction = menu.addAction("Remove");
    if ( menu.exec( event->screenPos() ) == removeAction )
    {
        this->remove();
    }
}

void Edge::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    m_pen.setColor(Qt::cyan);
    update();
}

void Edge::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    m_pen.setColor(Qt::black);
    update();
}
