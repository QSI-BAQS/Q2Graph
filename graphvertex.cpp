#include "graphvertex.hpp"
#include "graphedge.hpp"
// enables *QGraphicsView::scene() const, 'a pointer to the scene currently
// visualised in the view'
#include <QGraphicsScene>
#include <QJsonArray>


// public:
GraphVertex::GraphVertex(QMenu * contextmenu, unsigned int vid
                         , QGraphicsItem * parent)
   : QGraphicsEllipseItem(parent), contextmenu_v(contextmenu), vertexid(vid)
{
   // standard vertex: size, fill, circumference pen
   setRect(vertexboundaryrect);
   setBrush(vertexfill);
   setPen(vertexcircumferencepen);

   // properties of the vertex
   setFlag(QGraphicsItem::ItemIsFocusable);
   setFlag(QGraphicsItem::ItemIsMovable);
   setFlag(QGraphicsItem::ItemIsSelectable);
   setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

GraphVertex::~GraphVertex() {}


// read instructions for replicating private: <QVector> edges
void GraphVertex::readEdges(const QJsonObject & json) {
   QJsonArray edgesarray= json["vedges"].toArray();
   qreal vposx {};
   qreal vposy {};
   QPointF vertexXY {};
   QPair<QPointF,QPointF> readedge;

   bool setsecond= false;

   edgemirror.clear();
   for (int edgeindex= 0; edgeindex < edgesarray.size(); ++edgeindex){
      if(edgeindex % 2 == 0)
         vposx= edgesarray[edgeindex].toDouble();
      else
         vposy= edgesarray[edgeindex].toDouble();

      if(vposy){
         vertexXY.setX(vposx);
         vertexXY.setY(vposy);

         if(!setsecond){
            readedge.first= vertexXY;
            setsecond= true;
         }
         else {
            readedge.second= vertexXY;
            setsecond= false;
            edgemirror.push_back(readedge);
         }
         vposx= 0;
         vposy= 0;
      }
   }
}

// GraphVertex-level write instructions for file saving (format: JSON)
void GraphVertex::write(QJsonObject & json) const {
   json["vid"]= (double) vertexid;

   QJsonArray posxy;
   posxy.append(this->pos().x());
   posxy.append(this->pos().y());
   json["vpos"]= posxy;

   //QVector<GraphEdge *> edges
   QJsonArray vedgesxy;
   for(const GraphEdge * e : edges) {
      vedgesxy.append(e->p1v()->x());   // e.g., vedges[0];[4];[8]...
      vedgesxy.append(e->p1v()->y());   // e.g., vedges[1];[5];[9]...
      vedgesxy.append(e->p2v()->x());   // e.g., vedges[2];[6];[10]...
      vedgesxy.append(e->p2v()->y());   // e.g., vedges[3];[7];[11]...
   }
   json["vedges"]= vedgesxy;
}

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

void GraphVertex::resetColour(QColor colour, qreal pen, QColor fill) {
   vertexcircumferencepen= QPen(colour, pen);
   vertexfill= fill;
   //edges.at(X)->resetColour(colour); ???
   setPen(vertexcircumferencepen);
   setBrush(fill);

   update(boundingRect());
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

