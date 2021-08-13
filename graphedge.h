#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H
#pragma once

#include <QAction>
#include <QDebug>
#include <QGraphicsLineItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPen>

class GraphVertex;  // for GraphEdge constructor

QT_BEGIN_NAMESPACE
class QGraphicsSceneContextMenuEvent;
class QMenu;
QT_END_NAMESPACE


class GraphEdge : public QGraphicsLineItem
{
public:
   GraphEdge(GraphVertex * p1v, GraphVertex * p2v
             , QGraphicsItem * parent= nullptr);
   ~GraphEdge();

   // as with GraphVertex: to establish instance ID
   enum { Type= UserType + 4 };

   void resetColour(const QColor & colour);
   void resetEdgePosition();

   GraphVertex * p1v() const { return p1vertex; }
   GraphVertex * p2v() const { return p2vertex; }

   int type() const override { return Type; }

protected:
   void contextMenuEvent(QGraphicsSceneContextMenuEvent * event) override;

private:
   QPen edgecolour {Qt::black, 2};

   QMenu * edgemenu {};

   GraphVertex * p1vertex {};
   GraphVertex * p2vertex {};
};

#endif // GRAPHEDGE_H
