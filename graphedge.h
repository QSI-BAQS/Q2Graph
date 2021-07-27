#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H
#pragma once

#include <QDebug>
#include <QGraphicsLineItem>
#include <QPen>

class GraphVertex;  // for GraphEdge constructor

class GraphEdge : public QGraphicsLineItem
{
// group members by noun
public:
   GraphEdge(GraphVertex * p1V, GraphVertex * p2V
             , QGraphicsItem * parent= nullptr);
   ~GraphEdge();

   // as with GraphVertex: to establish instance ID
   enum { Type= UserType + 4 };


   GraphVertex * p1v() const { return p1vertex; }
   GraphVertex * p2v() const { return p2vertex; }

   void resetColour(const QColor & colour) {
      edgecolour= QPen(colour,2);
   }

   void setEdgePosition();

   int type() const override { return Type; }

private:
   QPen edgecolour {Qt::black,2};
   GraphVertex * p1vertex {};
   GraphVertex * p2vertex {};
};

#endif // GRAPHEDGE_H
