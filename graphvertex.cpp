#include "graphvertex.h"
#include "graphedge.h"


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
/*
void GraphVertex::removeEdge(GraphEdge * edge) {
   edges.removeAll(edge);
};

void GraphVertex::removeEdges() {}
*/
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

