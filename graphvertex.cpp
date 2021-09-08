#include "graphvertex.h"
#include "graphedge.h"
// enables *QGraphicsView::scene() const, 'a pointer to the scene currently
// visualised in the view'
#include <QGraphicsScene>


// public:
GraphVertex::GraphVertex(QMenu * contextmenu, QGraphicsItem * parent)
   : QGraphicsEllipseItem(parent), contextmenu_v(contextmenu)
{
   // standard vertex: size, fill, circumference pen
   setRect(vertexboundaryrect);
   setBrush(vertexfill);
   setPen(vertexcircumferencepen);

   // properties of the vertex
   setFlag(QGraphicsItem::ItemIsMovable);
   setFlag(QGraphicsItem::ItemIsSelectable);
   setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

}

GraphVertex::~GraphVertex() {}


void GraphVertex::addEdge(GraphEdge * edge) {
   edges.push_back(edge);
};

void GraphVertex::removeEdge(GraphEdge * edge) {
   edges.removeAll(edge);
};

void GraphVertex::removeEdges() {
   // deleting (dynamic) objects then deallocating memory do not mix: recall,
   // removeEdge() will execute against the (master) 'edges' vector
   const auto copy_edges= edges;

   for(GraphEdge * e : copy_edges) {
      // remove the edge from 'edges' container of both vertices...
      e->p1v()->removeEdge(e);
      e->p2v()->removeEdge(e);
      // back it out of the GraphFrame (QGraphicsScene) container...
      scene()->removeItem(e);
      // deallocate the memory
      delete e;
   }
}

void GraphVertex::resetColour(const QColor & colour) {
   vertexcircumferencepen= QPen(colour,2);
   //edges.at(X)->resetColour(colour); ???
   update();
}

void GraphVertex::setVertexID(unsigned int vscount) {
   vertexid= vscount;
}

// protected:
void GraphVertex::contextMenuEvent(QGraphicsSceneContextMenuEvent * event) {
   // the 'heavy lifting' for this menu occurs in GraphFrame
   scene()->clearSelection();
   setSelected(true);
   contextmenu_v->exec(event->screenPos());
}

QVariant GraphVertex::itemChange(GraphicsItemChange change, const QVariant & value) {
   if(change == QGraphicsItem::ItemPositionChange) {
      for(GraphEdge * edge : qAsConst(edges))
         edge->resetEdgePosition();
   }

   return value;
};

void GraphVertex::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
   Q_UNUSED(option);
   Q_UNUSED(widget);

   // call paint to render (constructor) standard vertex
   QGraphicsEllipseItem::paint(painter, option, widget);

   // font, pen and alignment of vertex id
   painter->setFont(vertexidfont);
   painter->setPen(vertexidpen);
   painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(vertexid));
}

