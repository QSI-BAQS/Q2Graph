#include "graphframe.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>


GraphFrame::GraphFrame(QWidget *parent)
   : QGraphicsScene(parent)
{
   label= new QLabel;
   label->setWindowFlag(Qt::ToolTip);
}

GraphFrame::~GraphFrame()
{
   delete label;
}

void GraphFrame::cursorState(bool setTF) {
   // 'setter' function
   // pre-condition: setTF == true | false
   // post-condition: datum: cursor, set as true or false
   if(setTF == false)
      p_cursorFT= &ft[0];

   else if(setTF == true)
      p_cursorFT= &ft[1];
}

void GraphFrame::keyPressEvent(QKeyEvent * event) {
   if(event->key() == Qt::Key_E){
      setCursorLabel("E");
      cursorState(true);
   }
   else if(event->key() == Qt::Key_O){
      setCursorLabel("O");
      cursorState(true);
   }
   else if (event->key() == Qt::Key_V){
      setCursorLabel("V");
      cursorState(true);
   }
   else if (event->key() == Qt::Key_X){
      setCursorLabel("X");
      cursorState(true);
   }
   else if (event->key() == Qt::Key_Y){
      setCursorLabel("Y");
      cursorState(true);
   }
   else if (event->key() == Qt::Key_Z){
      setCursorLabel("Z");
      cursorState(true);
   }
}

void GraphFrame::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {
   // position the label at x,y coordinates, relative to cursor
   if(*p_cursorFT)
      label->move((event->screenPos().x())+15,(event->screenPos().y())+10);

   // draw the dummy (graph)edge instantiated through mousePressEvent
   if(tracer != nullptr && label->text() == "E"){
      // grow the tracer line segment, following the mouse movement
      QLineF growline(tracer->line().p1(), event->scenePos());
      tracer->setLine(growline);
   }
}

void GraphFrame::mousePressEvent(QGraphicsSceneMouseEvent * event) {
   // LEFT CLICK events
   if(event->button() == Qt::LeftButton){
      if(label->text() == "E"){
         // initialise a 'tracer' line segment; the actual (Graph)Edge will be
         // set in arrears as a mouseReleaseEvent
         tracer= new QGraphicsLineItem(
                  QLineF(event->scenePos()
                         ,event->scenePos()));
         tracer->setPen(QPen(Qt::black,2));

         // add tracer to 'this' GraphFrame (QGraphicsScene) container/
         addItem(tracer);

         // reset CURSOR state
         cursorState(false);
         // hide the label: "E" is required as a boolean proxy in subsequent
         // QGraphicsSceneMouseEvent's
         label->hide();
      }
      else if(label->text() == "O"){
         /*functionality placeholder*/

         // reset CURSOR state
         cursorState(false);
         label->clear();
      }
      else if(label->text() == "V"){
         // instantiate the vertex
         GraphVertex * v;
         v= new GraphVertex();

         // colour of vertex fill: powder blue
         v->setBrush(QColor::fromRgb(176,224,230));
         // colour and font size of vertex circumference
         v->setPen(QPen(Qt::black, 2));

         // add a QGraphicsItem (GraphVertex) to 'this' GraphFrame
         // (QGraphicsScene) container
         addItem(v);

         // move rendered vertex from beneath the cursor
         QPointF adjScenePos= QPointF(
                  (event->scenePos().x()) - 10.0
                  ,(event->scenePos().y()) - 20.0);
         // TO DO: clear v->setrect() border; why are initial vertex inserts
         // farther from the cursor than latter inserts?
         // assign scene position (logical coordinates) to the vertex
         v->setPos(adjScenePos);
         //emit vertex_Insert(v);

         // reset CURSOR state
         cursorState(false);
         label->clear();
      }
      else if(label->text() == "X"){
         /*functionality placeholder*/

         // reset CURSOR state
         cursorState(false);
         label->clear();
      }
      else if(label->text() == "Y"){
         /*functionality placeholder*/

         // reset CURSOR state
         cursorState(false);
         label->clear();
      }
      else if(label->text() == "Z"){
         /*functionality placeholder*/

         // reset CURSOR state
         cursorState(false);
         label->clear();
      }
      QGraphicsScene::mousePressEvent(event);
   }
   /*
    * TO DO: menu on RIGHT-CLICK
   */
}

void GraphFrame::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
   if(tracer != nullptr && label->text() == "E"){
      // staging area for the vertex identified at p1 coordinates
      QList<QGraphicsItem *> p1Items= items(tracer->line().p1());
      // if the first object of p1Items is a tracer, remove it
      if(p1Items.count() && p1Items.first() == tracer)
         p1Items.removeFirst();

      //  staging area for the vertex identified at p2 coordinates
      QList<QGraphicsItem *> p2Items= items(tracer->line().p2());
      // as with p1Items: if the first object of p2Items is a tracer, remove it
      if(p2Items.count() && p2Items.first() == tracer)
         p2Items.removeFirst();

      // having now selected vertices p1 and p2 to pass as constructors to
      // GraphEdge, back out the variable tracer from 'this' GraphFrame
      // (QGraphicsScene) container...
      removeItem(tracer);
      // then, release the memory
      delete tracer;

      if(p1Items.count() > 0 && p2Items.count() > 0
            && p1Items.first()->type() == GraphVertex::Type   // match IDs
            && p2Items.first()->type() == GraphVertex::Type   // match IDs
            && p1Items.first() != p2Items.first()){
         // create a pointer to the (Graph)vertex designated as p1
         GraphVertex * p1V= qgraphicsitem_cast<GraphVertex *>(p1Items.first());
         // create a pointer to the (Graph)vertex designated as p2
         GraphVertex * p2V= qgraphicsitem_cast<GraphVertex *>(p2Items.first());
         // use p1V and p2V as constructors to instantiate the edge
         GraphEdge * e= new GraphEdge(p1V, p2V);

         // set the edge behind the p1 and p2 vertices
         e->setZValue(-1000.0);

         // add a QGraphicsLineItem (GraphEdge) to 'this' GraphFrame
         // (QGraphicsScene) container
         addItem(e);
         // assign scene position (logical coordinates) to the edge
         e->setEdgePosition();

         // at last, clear the "E" label
         if(label->text() == "E")
            label->clear();
      }
   }
   tracer= nullptr;
   QGraphicsScene::mouseReleaseEvent(event);
}

void GraphFrame::setCursorLabel(QString tag) {
   // create E/O/V/X/Y/Z label for cursor
   // pre-condition: tag is type QString
   // post-condition: a visible, persistent label for the cursor

   // label appears at (x,y coordinates), relative to cursor
   label->move(QCursor::pos().x()+15, QCursor::pos().y()+10);

   // layout
   label->setText(tag);

   if(label->isHidden())
      label->show();
};
