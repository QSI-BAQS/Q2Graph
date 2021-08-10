#include "graphframe.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>


// public:
GraphFrame::GraphFrame(QWidget *parent)
   : QGraphicsScene(parent)
{
   clabel= new QLabel;
   clabel->setWindowFlag(Qt::ToolTip);
   //clabel->setAttribute(Qt::WA_TranslucentBackground);
   //clabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
}

GraphFrame::~GraphFrame()
{
   delete clabel;
}

// protected:
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
      clabel->move((event->screenPos().x()) + 15
                   ,(event->screenPos().y()) + 10);

   // draw the dummy (Graph)edge instantiated through mousePressEvent
   if(tracer != nullptr && clabel->text() == "E"){
      // grow the tracer line segment, following the mouse movement
      QLineF growline(tracer->line().p1(), event->scenePos());
      tracer->setLine(growline);
   }
   QGraphicsScene::mouseMoveEvent(event);
}

void GraphFrame::mousePressEvent(QGraphicsSceneMouseEvent * event) {
   // LEFT CLICK events
   if(event->button() == Qt::LeftButton){
      // initiate a (Graph)edge
      if(clabel->text() == "E"){
         // prevent a runtime exception caused by nothing being at the cursor
         // hotspot, 'upon click'
         if(items(event->scenePos()).isEmpty()){
//qDebug() << "nothing at the hotspot, try again";
            // clear 'E' status and drop the cursor label
            cursorState(false);
            clabel->clear();
            return ;
         }
         // collect the p1 vertex at the cursor hotspot, 'upon click'
         QList<QGraphicsItem *> localvs= items(event->scenePos());

         // remove any first element of localvs, which is not a vertex
         if(localvs.first()->type() != GraphVertex::Type)
            localvs.removeFirst();
         else
            // prevent the subsequent mouseMoveEvent, which is meant solely to
            // draw an edge, from bringing the vertex along with it
            localvs.first()->setFlag(QGraphicsItem::ItemIsMovable, false);

         // initialise a 'tracer' line segment; the actual edge will
         // be set in arrears at the subsequent mouseReleaseEvent
         tracer= new QGraphicsLineItem(
                  QLineF(event->scenePos()
                         , event->scenePos()));
         tracer->setPen(QPen(Qt::black,2));

         // add tracer to the GraphFrame (QGraphicsScene) container
         addItem(tracer);

         // reset CURSOR state
         cursorState(false);
         // hide the label: "E" is required as a boolean proxy in subsequent
         // QGraphicsSceneMouseEvent's
         clabel->hide();
      }
      // do an O something
      else if(clabel->text() == "O"){
         /*functionality placeholder*/

         // reset CURSOR state
         cursorState(false);
         clabel->clear();
      }
      // initiate a (Graph)vertex
      else if(clabel->text() == "V"){
         // instantiate the vertex
         GraphVertex * v;
         v= new GraphVertex();

         // set vertex label (REVISIT: with delete operation)
         /* all vertices have the same .type(), which means .indexOf() is
          * ineffective
         */
         v->setVertexID(collectVertices().count());

         // add a QGraphicsItem (GraphVertex) to the GraphFrame
         // (QGraphicsScene) container
//qDebug() << "vertex added:" << v << "type():" << v->type();
         addItem(v);

         // move rendered vertex from beneath the cursor
         QPointF adjScenePos= QPointF(
                  (event->scenePos().x()) - 10.0
                  ,(event->scenePos().y()) - 20.0);
         // TO DO: clear v->setrect() border; why are initial vertex inserts
         // farther from the cursor than latter inserts?
         // assign scene position (logical coordinates) to the vertex
         v->setPos(adjScenePos);

         // reset CURSOR state
         cursorState(false);
         clabel->clear();
      }
      // do an X something
      else if(clabel->text() == "X"){
         /*functionality placeholder*/

         // reset CURSOR state
         cursorState(false);
         clabel->clear();
      }
      // do a Y something
      else if(clabel->text() == "Y"){
         /*functionality placeholder*/

         // reset CURSOR state
         cursorState(false);
         clabel->clear();
      }
      // delete (Graph)vertex X and all connected (Graph)edges
      else if(clabel->text() == "Z"){
         /*functionality placeholder*/

         // reset CURSOR state
         cursorState(false);
         clabel->clear();
      }
   }

   if(event->button() == Qt::RightButton)
      // prevents false positives?
      event->ignore();

   QGraphicsScene::mousePressEvent(event);
}

void GraphFrame::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
   if(tracer != nullptr && clabel->text() == "E"){
      // staging area for the vertex identified at p1 coordinates
      QList<QGraphicsItem *> p1items= items(tracer->line().p1());
      // if the first item of p1items is a tracer instance, remove it
      if(p1items.count() && p1items.first() == tracer)
         p1items.removeFirst();      
//qDebug() << "p1items.count():" << p1items.count();

      //  staging area for the vertex identified at p2 coordinates
      QList<QGraphicsItem *> p2items= items(tracer->line().p2());
      // as with p1items: if the first item of p2items is a tracer instance,
      // remove it
      if(p2items.count() && p2items.first() == tracer)
         p2items.removeFirst();      
//qDebug() << "p2items.count():" << p2items.count();

      // having now selected vertices p1 and p2 to pass as constructors to
      // GraphEdge, back out the variable tracer from the GraphFrame
      // (QGraphicsScene) container...
      removeItem(tracer);
      // then, deallocate the memory
      delete tracer;

      if(p1items.count() > 0 && p2items.count() > 0
            /* 'All standard graphicsitem classes are associated with a unique
               value' */
            && p1items.first()->type() == GraphVertex::Type
            && p2items.first()->type() == GraphVertex::Type
            && p1items.first() != p2items.first()){
         // create a pointer to the (Graph)vertex designated as p1
         GraphVertex * p1v= qgraphicsitem_cast<GraphVertex *>(p1items.first());
         // restore the 'movable' property of p1v, which was suspended at
         // mousePressEvent
         p1v->setFlag(QGraphicsItem::ItemIsMovable, true);
         // create a pointer to the (Graph)vertex designated as p2
         GraphVertex * p2v= qgraphicsitem_cast<GraphVertex *>(p2items.first());
         // use p1v and p2v as constructors to instantiate the edge
         GraphEdge * e= new GraphEdge(p1v, p2v);

         // add the edge to (QVector) 'edges' of vertices p1 and p2
         p1v->addEdge(e);
         p2v->addEdge(e);

         // add a QGraphicsLineItem (GraphEdge) to the GraphFrame
         // (QGraphicsScene) container
//qDebug() << "edge added:" << e << "type():" << e->type();
         addItem(e);

         // at last, clear the "E" label
         if(clabel->text() == "E")
            clabel->clear();
      }
   }
   tracer= nullptr;
   QGraphicsScene::mouseReleaseEvent(event);
}

// private:
void GraphFrame::cursorState(bool setTF) {
   // 'setter' function
   // pre-condition: setTF == true | false
   // post-condition: datum: cursor, set as true or false
   if(setTF == false)
      p_cursorFT= &ft[0];

   else if(setTF == true)
      p_cursorFT= &ft[1];
}

void GraphFrame::setCursorLabel(QString tag) {
   // create E/O/V/X/Y/Z label for cursor
   // pre-condition: tag is type QString
   // post-condition: a visible, persistent label for the cursor

   // label appears at (x,y coordinates), relative to cursor
   clabel->move(QCursor::pos().x() + 15
                , QCursor::pos().y() + 10);

   // set label at E/O/V/X/Y/Z
   clabel->setText(tag);

   // reveal a hidden cursor
   if(clabel->isHidden())
      clabel->show();
};

// select QList over QVector for its guarantee of preserving entry order
QList<GraphVertex *> GraphFrame::collectVertices(){
   // source
   QList<QGraphicsItem *> allitems= items();
   // target
   QList<GraphVertex *> allvs {};

   // cast any QGraphicsItem as a GraphVertex, using its virtual (int) Type as
   // an identifier, then, collect it
   for (QGraphicsItem * i : qAsConst(allitems)) {
      if(i->type() == GraphVertex::Type){
         GraphVertex * iisv= qgraphicsitem_cast<GraphVertex *>(i);
         allvs.push_back(iisv);
      }
   }

   return allvs;
}
