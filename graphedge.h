#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H
#pragma once

#include <QDebug>
#include <QGraphicsLineItem>
#include <QPen>

class GraphVertex;  // for GraphEdge constructor

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

private:
   QPen edgecolour {Qt::black, 2};
   GraphVertex * p1vertex {};
   GraphVertex * p2vertex {};
};

#endif // GRAPHEDGE_H
