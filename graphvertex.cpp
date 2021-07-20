#include "graphvertex.h"

GraphVertex::GraphVertex(QRectF rect, QGraphicsItem * parent)
   : QGraphicsEllipseItem(rect,parent)
{
   // boolean properties of the vertex
   setFlag(QGraphicsItem::ItemIsMovable, true);
   setFlag(QGraphicsItem::ItemIsSelectable, true);

}

GraphVertex::~GraphVertex() {}
