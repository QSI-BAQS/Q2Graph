#include "graphvertex.h"
#include "graphedge.h"


GraphVertex::GraphVertex(QGraphicsItem * parent)
   : QGraphicsEllipseItem(parent)
{
   // standard vertex: size, fill, circumference pen
   setRect(vertexboundaryrect);
   setBrush(vertexfill);
   setPen(vertexcircumferencepen);

   // enable labelling of GraphVertex representation
   //vertexid= new QGraphicsSimpleTextItem(this);

   // properties of the vertex
   setFlag(QGraphicsItem::ItemIsMovable);
   setFlag(QGraphicsItem::ItemIsSelectable);
   setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

}

GraphVertex::~GraphVertex() {}

void GraphVertex::addEdge(GraphEdge * edge) {
   qDebug() << "edge added:" << edge;
   edges.push_back(edge);
};
/*
void GraphVertex::removeEdge(GraphEdge * edge) {
   edges.removeAll(edge);
};*/

void GraphVertex::setVertexID(int vscount) {
   //qDebug() << "vscount + 1 =" << vscount + 1;
   vertexid= vscount + 1;
}

QVariant GraphVertex::itemChange(GraphicsItemChange change, const QVariant & value) {
   if(change == QGraphicsItem::ItemPositionChange) {
      for(GraphEdge * edge : qAsConst(edges))
         edge->setEdgePosition();
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

