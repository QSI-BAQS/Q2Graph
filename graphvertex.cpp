#include "graphvertex.h"


GraphVertex::GraphVertex(QGraphicsItem * parent)
   : QGraphicsEllipseItem(parent)
{
   // standard vertex: size, fill, circumference pen
   setRect(vertexboundaryrect);
   setBrush(vertexfill);
   setPen(vertexcircumferencepen);

   // enable labelling of GraphVertex representation
   vertexid= new QGraphicsSimpleTextItem(this);

   // properties of the vertex
   setFlag(QGraphicsItem::ItemIsMovable);
   setFlag(QGraphicsItem::ItemIsSelectable);
   setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

}

GraphVertex::~GraphVertex() {
   delete vertexid;
}

void GraphVertex::mousePressEvent(QGraphicsSceneMouseEvent * event) {
   if(event->button() == Qt::LeftButton){
      if(event->modifiers() == Qt::ControlModifier){
         //qDebug() << "Custom item clicked with Ctrl-key";
         setSelected(true);
      }
   }
   QGraphicsItem::mousePressEvent(event);
}

void GraphVertex::setVertexID(int vscount) {
   //qDebug() << "vscount + 1 =" << vscount + 1;
   // position and centre the id text
   int vid {vscount + 1};
   if(vid < 10)
      vertexid->setPos(QPointF(vertexboundaryrect.x() + 6.5
                               , vertexboundaryrect.y()+ 3.0));
   else if(vid > 9 && vid < 100)
      vertexid->setPos(QPointF(vertexboundaryrect.x() + 3.5
                               , vertexboundaryrect.y() + 3.0));
   else
      vertexid->setPos(QPointF(vertexboundaryrect.x() + 1.0
                               , vertexboundaryrect.y() + 3.0));

   vertexid->setBrush(vertexidbrush);
   vertexid->setFont(vertexidfont);
   vertexid->setText(QString::number(vscount + 1));
}
