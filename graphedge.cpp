#include "graphedge.h"
#include "graphvertex.h"

GraphEdge::GraphEdge(GraphVertex * p1v, GraphVertex * p2v
                     , QGraphicsItem * parent)
   : QGraphicsLineItem (parent), p1vertex(p1v), p2vertex(p2v)
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
