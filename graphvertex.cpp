#include "graphvertex.h"

GraphVertex::GraphVertex(QGraphicsItem * parent)
   : QGraphicsEllipseItem(parent)
{
   // standard vertex size
   setRect(QRectF(
              QPointF(0.0,0.0)
              , QSizeF(20.0,20.0)));

   // boolean properties of the vertex
   setFlag(QGraphicsItem::ItemIsMovable, true);
   setFlag(QGraphicsItem::ItemIsSelectable, true);

}

GraphVertex::~GraphVertex() {}
