#include "graphvertex.h"
#include "graphedge.h"
// enables *QGraphicsView::scene() const, 'a pointer to the scene currently
// visualised in the view'
#include <QGraphicsScene>


// public:
GraphVertex::GraphVertex(QGraphicsItem * parent)
   : QGraphicsEllipseItem(parent)
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
//qDebug() << "edge added:" << edge << "type():" << edge->type();
   edges.push_back(edge);
};

void GraphVertex::removeEdge(GraphEdge * edge) {
   edges.removeAll(edge);
};

void GraphVertex::z_removeEdges() {
   // deleting (dynamic) objects then deallocating memory don't mix: recall,
   // removeEdge() will execute against the (master) 'edges' vector
   const auto z_edges= edges;

   for(GraphEdge * e : z_edges) {
      // remove the edge from both vertices...
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
//qDebug() << "vscount + 1 =" << vscount + 1;
   vertexid= vscount + 1;
}

// protected:
void GraphVertex::contextMenuEvent(QGraphicsSceneContextMenuEvent * event) {
   vertexmenu= new QMenu;
   vertexmenu->addAction("v_Delete");
   vertexmenu->addAction("--v_dummy--");

   vertexmenu->exec(event->screenPos());
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
   QGraphicsEllipseItem::paint(painter,option,widget);

   // font, pen and alignment of vertex id
   painter->setFont(vertexidfont);
   painter->setPen(vertexidpen);
   painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(vertexid));
}

