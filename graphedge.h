#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H
#pragma once

#include <QDebug>
#include <QGraphicsLineItem>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QPen>

class GraphVertex;  // for GraphEdge constructor

QT_BEGIN_NAMESPACE
class QGraphicsSceneContextMenuEvent;
class QMenu;
QT_END_NAMESPACE


class GraphEdge : public QGraphicsLineItem
{
public:
   GraphEdge(GraphVertex *, GraphVertex *, QMenu *
             , QGraphicsItem * parent= nullptr);
   ~GraphEdge();

   // enable use of qgraphicsitem_cast 
   enum { Type= UserType + 5 };
   int type() const override { return Type; }

   void resetColour(const QColor &);
   void resetEdgePosition();

   GraphVertex * p1v() const { return p1vertex; }
   GraphVertex * p2v() const { return p2vertex; }

protected:
   void contextMenuEvent(QGraphicsSceneContextMenuEvent *) override;

private:
   QMenu * contextmenu_e {};

   QPen edgecolour {Qt::black, 2};

   GraphVertex * p1vertex {};
   GraphVertex * p2vertex {};
};

#endif // GRAPHEDGE_H
