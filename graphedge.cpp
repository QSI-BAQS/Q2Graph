#include "graphedge.h"

GraphEdge::GraphEdge(GraphVertex * p1V, GraphVertex * p2V
                     , QGraphicsItem * parent)
   : QGraphicsLineItem (parent), p1vertex(p1V), p2vertex(p2V)
{
   setFlag(QGraphicsItem::ItemIsSelectable);
   setPen(edgecolour);
}

GraphEdge::~GraphEdge() {};
