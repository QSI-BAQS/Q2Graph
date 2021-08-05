#ifndef GRAPHVERTEX_H
#define GRAPHVERTEX_H
#pragma once

#include <QDebug>

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QVector>

class GraphEdge;
/*
QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE
*/
class GraphVertex : public QGraphicsEllipseItem
{
public:
   GraphVertex(QGraphicsItem * parent= nullptr);
   // resolve compile error 'undefined reference to XXX vtable', see: https://
   // stackoverflow.com/questions/14010922/qt-undefined-reference-to-vtable
   ~GraphVertex();

   // for establishing instance ID
   enum {Type= UserType + 15};

   // required by QGraphicsView?
   QRectF boundingRect() const override { return vertexboundaryrect; };
   int type() const override { return Type; }

   void addEdge(GraphEdge * edge);
   //void removeEdge(GraphEdge * edge);
   //void removeEdges();

   void setVertexID(int vscount);

protected:
   //void contextMenuEvent(QGraphicsSceneContextMenuEvent * event) override;
   QVariant itemChange(GraphicsItemChange change
                       , const QVariant & value) override;
   void paint(QPainter * painter, const QStyleOptionGraphicsItem * option
              , QWidget * widget) override;

private:
   QVector<GraphEdge *> edges;

   QRectF vertexboundaryrect {QPointF(0.0,0.0), QSizeF(20.0,20.0)};
   QPen vertexcircumferencepen {Qt::black, 2};
   QBrush vertexfill {QColor::fromRgb(245,245,245)};   // white smoke

   int vertexid {};
   QFont vertexidfont {"SansSerif", 8, QFont::Normal};
   QPen vertexidpen {Qt::darkBlue, 1};
   //QMenu * vertexmenu {};
};

#endif // GRAPHVERTEX_H
