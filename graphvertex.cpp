#include "graphvertex.h"

GraphVertex::GraphVertex(QGraphicsItem * parent)
   : QGraphicsEllipseItem(parent)
{
   // standard vertex size
   setRect(QRectF(
              QPointF(0.0,0.0)
              , QSizeF(20.0,20.0)));

   // boolean properties of the vertex
   setFlag(QGraphicsItem::ItemIsMovable);
   setFlag(QGraphicsItem::ItemIsSelectable);

}

GraphVertex::~GraphVertex() {}


void GraphVertex::mousePressEvent(QGraphicsSceneMouseEvent * event) {
   if(event->button() == Qt::LeftButton){
      if(event->modifiers() == Qt::ControlModifier){
         qDebug() << "Custom item clicked with Ctrl-key";
         setSelected(true);
      }
   }
   QGraphicsItem::mousePressEvent(event);
}

