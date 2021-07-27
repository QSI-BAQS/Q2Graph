#include "graphedge.h"
#include "graphvertex.h"

GraphEdge::GraphEdge(GraphVertex * p1V, GraphVertex * p2V
                     , QGraphicsItem * parent)
   : QGraphicsLineItem (parent), p1vertex(p1V), p2vertex(p2V)
{
   setFlag(QGraphicsItem::ItemIsSelectable);
   setPen(edgecolour);
}

GraphEdge::~GraphEdge() {};

void GraphEdge::setEdgePosition() {
   QLineF edge(mapFromItem(p1vertex,10,10)
               ,mapFromItem(p2vertex,10,10));
   setLine(edge);
}
