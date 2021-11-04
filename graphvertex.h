#ifndef GRAPHVERTEX_H
#define GRAPHVERTEX_H
#pragma once

#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QJsonObject>
#include <QMenu>
#include <QPainter>
#include <QVector>

class GraphEdge;

QT_BEGIN_NAMESPACE
class QGraphicsSceneContextMenuEvent;
class QMenu;
QT_END_NAMESPACE

class GraphVertex : public QGraphicsEllipseItem
{
public:
   GraphVertex(QMenu *, unsigned int vid= 0, QGraphicsItem * parent= nullptr);
   // resolve compile error 'undefined reference to XXX vtable', see: https://
   // stackoverflow.com/questions/14010922/qt-undefined-reference-to-vtable
   ~GraphVertex();

   // enable use of qgraphicsitem_cast() 
   enum {Type= UserType + 3};
   int type() const override { return Type; }

   QVector<QPair<QPointF,QPointF>> edgemirror {};
   void readEdges(const QJsonObject &);
   void write(QJsonObject &) const;

   // required by QGraphicsView?
   QRectF boundingRect() const override {return vertexboundaryrect; };

   void addEdge(GraphEdge * edge) {edges.push_back(edge); };
   const QVector<GraphEdge *> * lcEdges() const {return & edges; };
   void removeEdge(GraphEdge * edge) {edges.removeAll(edge); };
   void removeEdges();

   void addNeighbour(GraphVertex * vertex) {neighbourvs.push_back(vertex); };
   const QVector<GraphVertex *> * lcNeighbours() const {return & neighbourvs; };
   void clearNeighbours() {neighbourvs.clear(); };

   void resetColour(QColor, qreal= 2, QColor= QColor(245,245,245));
   void setVertexID(unsigned int);

protected:
   void contextMenuEvent(QGraphicsSceneContextMenuEvent *) override;
   QVariant itemChange(GraphicsItemChange, const QVariant &) override;
   void paint(QPainter *, const QStyleOptionGraphicsItem *
              , QWidget * widget= nullptr) override;

private:
   QMenu * contextmenu_v {};

   QVector<GraphEdge *> edges;

   // container of neighbour vertices of vertex X
   QVector<GraphVertex *> neighbourvs;

   QRectF vertexboundaryrect {QPointF(0.0,0.0), QSizeF(20.0,20.0)};
   QPen vertexcircumferencepen {Qt::black, 2};
   QBrush vertexfill {QColor::fromRgb(245,245,245)};   // 'white smoke'

   unsigned int vertexid {};
   QFont vertexidfont {"SansSerif", 8, QFont::Normal};
   QPen vertexidpen {Qt::darkBlue, 1};
};

#endif // GRAPHVERTEX_H
