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

   createMenus();
}

GraphFrame::~GraphFrame() {}

// protected:
void GraphFrame::keyPressEvent(QKeyEvent * event) {
   QVector<int> cursorflags {Qt::Key_E
            , Qt::Key_0
            , Qt::Key_V
            , Qt::Key_X
            , Qt::Key_Y
            , Qt::Key_Z};

   if(cursorflags.contains(event->key()))
      cursorState(true);
   else
      cursorState(false);

   switch (event->key()) {
   case Qt::Key_E:
      setCursorLabel("E");
      break;
   case Qt::Key_0:
      setCursorLabel("0");
      break;
   case Qt::Key_V:
//qDebug() << "cursorState():" << *p_cursorFT;
      setCursorLabel("V");
      break;
   case Qt::Key_X:
      setCursorLabel("X");
      break;
   case Qt::Key_Y:
      setCursorLabel("Y");
      break;
   case Qt::Key_Z:
      setCursorLabel("Z");
      // to avoid capturing unintended vertex(s) at mousePressEvent
      clearSelection();
      break;
   default:
//qDebug() << "cursorState():" << *p_cursorFT;
      QGraphicsScene::keyPressEvent(event);
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
   if(event->button() != Qt::LeftButton)
      return ;

   // operation: instantiate a (Graph)edge
   if(clabel->text() == "E"){
      // collect the p1 vertex at the cursor hotspot, 'upon click'
      QList<QGraphicsItem *> localvs= items(event->scenePos()
                                            , Qt::ContainsItemShape);
      // prevent a runtime exception caused by nothing being at the cursor
      // hotspot, 'upon click'.  Note, by not failing this gate condition,
      // element 'localvs.first()' must be a (Graph)vertex
      if(localvs.isEmpty() || localvs.first()->type() != GraphVertex::Type){
         // abort:  drop 'E' status and the cursor label
         cursorState(false);
         clabel->clear();
         return ;
      }

      // prevent the ensuing mouseMoveEvent, which is meant solely to extend
      // an edge, from bringing the vertex along with it
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

   // operation: identity matrix
   else if(clabel->text() == "0"){
      /*functionality placeholder*/

      // reset CURSOR state
      cursorState(false);
      clabel->clear();
   }

   // operation: instantiate a (Graph)vertex
   else if(clabel->text() == "V"){
      // instantiate the vertex
      GraphVertex * v;
      v= new GraphVertex(vertexmenu);

      // collect the (GraphVertex *) vertex
      vertices.push_back(v);

      // set the vertex id number; all vertices have the same .type() so
      //   .indexOf() is useless as a proxy id
      v->setVertexID(vertices.count());

      // add a QGraphicsItem (GraphVertex) to the GraphFrame
      // (QGraphicsScene) container
//qDebug() << "vertex added:" << v << "type():" << v->type();   
      addItem(v);

      // move rendered vertex from beneath the cursor
      QPointF adjScenePos= QPointF(
               (event->scenePos().x()) - 10.0
               , (event->scenePos().y()) - 20.0);

      // TO DO: clear v->setrect() border; why are initial vertex inserts
      // farther from the cursor than latter inserts?
      // assign scene position (logical coordinates) to the vertex
      v->setPos(adjScenePos);

      // reset CURSOR state
      cursorState(false);
      clabel->clear();
   }

   // operation: X local Pauli measurement
   else if(clabel->text() == "X"){
      /*functionality placeholder*/

      // reset CURSOR state
      cursorState(false);
      clabel->clear();
   }

   // operation: Y local Pauli measurement
   else if(clabel->text() == "Y"){
      /*functionality placeholder*/

      // reset CURSOR state
      cursorState(false);
      clabel->clear();
   }

   // operation: Z local Pauli measurement
   else if(clabel->text() == "Z"){
      // collect the vertex at the cursor hotspot, 'upon click'
      QList<QGraphicsItem *> localvs= items(event->scenePos()
                                            , Qt::ContainsItemShape);
      // prevent a runtime exception caused by nothing being at the cursor
      // hotspot, 'upon click'.  Note, by not failing this gate condition,
      // element 'localvs.first()' must be a (Graph)vertex
      if(localvs.isEmpty() || localvs.first()->type() != GraphVertex::Type){
         return ;
      }
      // Cf. deleteVertex(), below
      else {
         // cast a 'QGraphicsItem *' as a 'GraphVertex *' in order to access
         // GraphVertex members
         GraphVertex * v4fs= qgraphicsitem_cast<GraphVertex *>(localvs.first());

         // remove any and all (Graph)edges connected to the vertex
         v4fs->removeEdges();

         // remove the vertex from QList 'vertices'
         vertices.removeAll(v4fs);
         // back the vertex out of GraphFrame...
         removeItem(v4fs);
         // deallocate the memory
         delete v4fs;

         // if there are any remaining vertices...
         if(!vertices.isEmpty()){
            unsigned int id {1};   // TO DO: mutable counter
            // ... reset the id numbering of each vertex
            for (GraphVertex * v : qAsConst(vertices)) {
               v->setVertexID(id);
               v->update();
               id++;
            }
         }
      }

      // reset CURSOR state
      cursorState(false);
      clabel->clear();
   }

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
      // GraphEdge, back out the variable tracer from GraphFrame
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
         GraphEdge * e= new GraphEdge(p1v, p2v, edgemenu);

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
   // create E/0/V/X/Y/Z label for cursor
   // pre-condition: tag is type QString
   // post-condition: a visible, persistent label for the cursor

   // label appears at (x,y coordinates), relative to cursor
   clabel->move(QCursor::pos().x() + 15
                , QCursor::pos().y() + 10);

   // set label at E/0/V/X/Y/Z
   clabel->setText(tag);

   // reveal a hidden cursor
   if(clabel->isHidden())
      clabel->show();
};

void GraphFrame::deleteEdge() {
   // delete a (Graph)edge
   // pre-condition: target object is type, GraphEdge
   // post-condition: target (Graph)edge is removed, any formerly connected
   //   (Graph)vertices are unaffected

   // collect only the edge at the cursor hotspot, 'upon click'
   QList<QGraphicsItem *> del_edge= selectedItems();
   // either first object of del_edge is type GraphEdge or, abort
   if(del_edge.isEmpty() || del_edge.first()->type() != GraphEdge::Type){
      return ;
   }
   else {
      // cast a QGraphicsItem * as a GraphEdge * in order to access GraphEdge
      // methods
      GraphEdge * e4fs= qgraphicsitem_cast<GraphEdge *>(del_edge.first());

      // remove the edge from 'edges' container of both vertices...
      e4fs->p1v()->removeEdge(e4fs);
      e4fs->p2v()->removeEdge(e4fs);
      // back the edge out of GraphFrame...
      removeItem(e4fs);
      // deallocate the memory
      delete e4fs;
   }
}

void GraphFrame::localComplementation() {
   // LC applied to vertex X: all vertices of X's neighbourhood not joined by
   // a (Graph)edge to gain an edge and conversely
   // pre-condition: target object is type, GraphVertex
   // post-condition: edges are reconfigured to LC status

   // collect only the vertex at the cursor hotspot, 'upon click'
   QList<QGraphicsItem *> lc_vertex= selectedItems();

   // either operate on a GraphVertex object or, abort
   if(lc_vertex.isEmpty() || lc_vertex.first()->type() != GraphVertex::Type)
      return ;

   // cast a 'QGraphicsItem *' as a 'GraphVertex *' so as to access GraphVertex
   // members
   GraphVertex * lcv= qgraphicsitem_cast<GraphVertex *>(lc_vertex.first());

   // vertex X must have at least one (1) edge for LC: abort
   if(lcv->lcEdges()->isEmpty()){
      lcv->setSelected(false);
      return ;
   }

   // if vertex X has one (1) edge, LC operation
   if(lcv->lcEdges()->count() == 1){
      lcv->removeEdges();
      lcv->setSelected(false);
   }
   // if vertex X has > 1 edge, LC operation
   else {
      // container: neighbour vertex with one (1) edge, to vertex X
      QVector<GraphVertex *> neighbourEQ1edge {};
      // container: neighbour vertex with an edge to vertex X plus an edge to
      // at least one other vertex
      QVector<GraphVertex *> neighbourGT1edge {};

      // capture the neighbour vertices of vertex X (i.e. the vertex 'at the
      // opposite end' of any edge shared with vertex X); note the dereference
      // operation on lcv->lcEdges()
      for (GraphEdge * e : *lcv->lcEdges()) {
         // copy the GraphVertex * in order then to access the address of the
         // underlying object
         GraphVertex * foraddrofp1v= e->p1v();

         // test whether pointed-to-objects are equivalent by comparing their
         // addresses
         if(foraddrofp1v != lcv){
            if(e->p1v()->lcEdges()->count() == 1)
               // opposite vertex has one (1) edge: sort to container
               // neighbourEQ1edge
               neighbourEQ1edge.push_back(e->p1v());
            else
               // opposite edge has >1 edges: sort to container
               // neighbourGT1edge
               neighbourGT1edge.push_back(e->p1v());
         }
         else {
            if(e->p2v()->lcEdges()->count() == 1)
               // opposite vertex has one (1) edge: sort to container
               // neighbourEQ1edge
               neighbourEQ1edge.push_back(e->p2v());
            else
               // opposite edge has >1 edges: sort to container
               // neighbourGT1edge
               neighbourGT1edge.push_back(e->p2v());
         }
      }
//qDebug() << "vertex X pos():" << lcv->pos() << "neighbourEQ1edge:" << neighbourEQ1edge.mid(0);
//qDebug() << "vertex X pos():" << lcv->pos() << "neighbourGT1edge:" << neighbourGT1edge.mid(0);

      /*for (GraphVertex * p_nvx : neighbourEQ1edge) {
         // recurse over neighbourEQ1edge, connecting each element to the other
         // elements
         return ;
      }*/

      // TO DO: recursive solution to exclude edge v2 -> v5
      for (GraphVertex * p_nvx : neighbourGT1edge ) {
         // IFF p_nvx connects to another neighbour of vertex X, delete the
         // common edge
         for (GraphEdge * e : *p_nvx->lcEdges()) {
            // derive the GraphVertex * in order then to access the address of
            // the underlying object
            GraphVertex * fs4p1v= e->p1v();
            GraphVertex * fs4p2v= e->p2v();

            // this will delete edge v2 -> v5
            if(fs4p1v != lcv && fs4p2v != lcv){
               // Cf. deleteEdge above: remove edge + removeItem + delete operation
               // remove the edge from 'edges' container of both vertices...
               fs4p1v->removeEdge(e);
               fs4p2v->removeEdge(e);
               // back the edge out of GraphFrame...
               removeItem(e);
               // deallocate the memory
               delete e;
            }
         }
      }
   }
}

void GraphFrame::createMenus() {
   edgemenu= new QMenu("edge menu");
   edgemenu->addAction("Delete", this, [this](){ deleteEdge(); });
   edgemenu->addAction("-- place 2 --");

   vertexmenu= new QMenu("vertex menu");
   vertexmenu->addAction("Delete", this, [this](){ deleteVertex(); });
   vertexmenu->addAction("Local_Complementation", this
                         , [this](){ localComplementation(); });
   vertexmenu->addAction("-- place 3 --");
}

void GraphFrame::deleteVertex() {
   // delete a (Graph)vertex and its connected (Graph)edges
   // pre-condition: target object is type, GraphVertex
   // post-condition: target (Graph)vertex and all associated (Graph)edges are
   //   removed; the ID of each remaining vertex is reset

   // collect only the vertex at the cursor hotspot, 'upon click'
   QList<QGraphicsItem *> del_vertex= selectedItems();
   // either operate on a GraphVertex object or, abort
   if(del_vertex.isEmpty() || del_vertex.first()->type() != GraphVertex::Type)
      return ;
   else {
      // cast a 'QGraphicsItem *' as a 'GraphVertex *' in order to access
      // GraphVertex members
      GraphVertex * v4fs= qgraphicsitem_cast<GraphVertex *>(del_vertex.first());

      // remove any and all (Graph)edges connected to the vertex
      v4fs->removeEdges();

      // remove the vertex from QList 'vertices'
      vertices.removeAll(v4fs);
      // back the vertex out of GraphFrame...
      removeItem(v4fs);
      // deallocate the memory
      delete v4fs;

      // if there are any remaining vertices...
      if(!vertices.isEmpty()){
         unsigned int id {1};   // TO DO: mutable counter
         // ... reset the id numbering of each vertex
         for (GraphVertex * v : qAsConst(vertices)) {
            v->setVertexID(id);
            v->update();
            id++;
         }
      }
   }
}

