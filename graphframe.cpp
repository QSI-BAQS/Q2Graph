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
   if(*p_cursorFT)
      // move the label to x,y coordinates, relative to cursor
      label->move((event->screenPos().x())+15,(event->screenPos().y())+10);
}

void GraphFrame::mousePressEvent(QGraphicsSceneMouseEvent * event) {
   // LEFT CLICK events
   if(event->button() == Qt::LeftButton){
      if(label->text() == "E"){
         /*functionality placeholder*/

         // reset CURSOR state
         cursorState(false);
         label->hide();
      }
      else if(label->text() == "O"){
         /*functionality placeholder*/

         // reset CURSOR state
         cursorState(false);
         label->hide();
      }
      else if(label->text() == "V"){
         // record the 'click' POS
         QPointF lbpPOS {event->scenePos()};
         // QRectF reference point, bottom-right
         QPointF br {lbpPOS.x()-20.0,lbpPOS.y()-20.0};
         // QRectF reference point, top-left
         QPointF tl {lbpPOS};

         // instantiate the vertex
         GraphVertex * v;
         v= new GraphVertex(QRectF(tl,br));

         // fill colour: powder blue
         v->setBrush(QColor::fromRgb(176,224,230));
         // boundary colour: black, 2 pt
         v->setPen(QPen(Qt::black, 2));

         // GraphFrame acts as a container of Vertex objects
         v->setPos(lbpPOS);
         addItem(v);
         //emit vertex_Insert(v);

         // reset CURSOR state
         cursorState(false);
         label->clear();
      }
      else if(label->text() == "X"){
         /*functionality placeholder*/

         // reset CURSOR state
         cursorState(false);
         label->hide();
      }
      else if(label->text() == "Y"){
         /*functionality placeholder*/

         // reset CURSOR state
         cursorState(false);
         label->hide();
      }
      else if(label->text() == "Z"){
         /*functionality placeholder*/

         // reset CURSOR state
         cursorState(false);
         label->hide();
      }
   }
   /*
    * TO DO: menu on RIGHT-CLICK
   */
}

//void GraphFrame::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {}

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
