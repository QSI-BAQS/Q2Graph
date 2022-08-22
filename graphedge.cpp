#include "graphedge.hpp"
#include "graphvertex.hpp"
// enables *QGraphicsView::scene() const, 'a pointer to the scene currently
// visualised in the view'
#include <QGraphicsScene>


// public:
GraphEdge::GraphEdge(GraphVertex * p1v, GraphVertex * p2v, QMenu * contextmenu
                     , QGraphicsItem * parent)
   : QGraphicsLineItem(parent), contextmenu_e(contextmenu), p1vertex(p1v)
   , p2vertex(p2v)

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

// protected:
void GraphEdge::contextMenuEvent(QGraphicsSceneContextMenuEvent * event) {
   // the 'heavy lifting' for this functionality occurs in GraphFrame
   scene()->clearSelection();
   setSelected(true);
   contextmenu_e->exec(event->screenPos());
}
