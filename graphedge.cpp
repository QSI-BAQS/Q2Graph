#include "graphedge.h"
#include "graphvertex.h"


// public:
GraphEdge::GraphEdge(GraphVertex * p1v, GraphVertex * p2v
                     , QGraphicsItem * parent)
   : QGraphicsLineItem (parent), p1vertex(p1v), p2vertex(p2v)
{
   // set edge colour and width
   setPen(edgecolour);

   // define and instantiate the edge
   QLineF edge(mapFromItem(p1vertex, 10, 10)
               , mapFromItem(p2vertex, 10, 10));
   setLine(edge);

   // set the edge behind its p1 and p2 vertices
   setZValue(-999);

   // properties of the edge
   setFlag(QGraphicsItem::ItemIsSelectable);
}

GraphEdge::~GraphEdge() {};


void GraphEdge::resetColour(const QColor & colour) {
   edgecolour= QPen(colour, 2);
   update();
}

void GraphEdge::resetEdgePosition() {
   if(p1vertex != NULL && p2vertex != NULL){
      QLineF edge(mapFromItem(p1vertex, 10, 10)
                  , mapFromItem(p2vertex, 10, 10));
      setLine(edge);
   }
}
