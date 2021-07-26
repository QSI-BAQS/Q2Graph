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

   // draw a dummy (graph)edge
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
         // collection
         addItem(tracer);

         // reset CURSOR state
         cursorState(false);
         // hide the label, "E" is required as a boolean proxy in subsequent
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
         // (QGraphicsScene) container/collection
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
/*
void GraphFrame::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
   // set the GraphEdge in arrears
   //label->clear();
}*/

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
