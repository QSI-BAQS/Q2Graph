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
      /*LC v1 then LC one of v1's neighbours then (again) LC v1 then
       * gf_deleteVertex(v1)
      */

      // reset CURSOR state
      cursorState(false);
      clabel->clear();
   }

   // operation: Y local Pauli measurement
   else if(clabel->text() == "Y"){
/*
      // collect the vertex at the cursor hotspot, 'upon click'
      QList<QGraphicsItem *> lc_vertex= items(event->scenePos()
                                            , Qt::ContainsItemShape);

      // prevent a runtime exception caused by nothing being at the cursor
      // hotspot, 'upon click'.  Either operate on a GraphVertex object or,
      // abort
      if(lc_vertex.isEmpty() || lc_vertex.first()->type() != GraphVertex::Type)
         return ;

      // cast a 'QGraphicsItem *' as a 'GraphVertex *' so as to access GraphVertex
      // members
      GraphVertex * lcv= qgraphicsitem_cast<GraphVertex *>(lc_vertex.first());

      // vertex X must have >= 1 edge for LC: abort
      if(lcv->lcEdges()->isEmpty()){
         lcv->setSelected(false);
         return ;
      }

      // 'DRY': pass to general LC function
      gf_localComplementation(lcv);

      // 'DRY': pass to general vertex delete function
      gf_deleteVertex(lcv);
*/
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
      if(localvs.isEmpty() || localvs.first()->type() != GraphVertex::Type)
         return ;

      // cast a 'QGraphicsItem *' as a 'GraphVertex *' in order to access
      // GraphVertex members
      GraphVertex * v4fs= qgraphicsitem_cast<GraphVertex *>(localvs.first());

      // 'DRY': pass to general vertex delete function
      gf_deleteVertex(v4fs);

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



void GraphFrame::createMenus() {
   edgemenu= new QMenu("edge menu");
   edgemenu->addAction("Delete", this, [this](){
      // collect only the edge at the cursor hotspot, 'upon click'
      QList<QGraphicsItem *> del_edge= selectedItems();
      // either first object of del_edge is type GraphEdge or, abort
      if(del_edge.isEmpty() || del_edge.first()->type() != GraphEdge::Type)
         return ;

      // cast a QGraphicsItem * as a GraphEdge * in order to access GraphEdge
      // methods
      GraphEdge * e4fs= qgraphicsitem_cast<GraphEdge *>(del_edge.first());
      // 'DRY': pass to general edge delete function
      gf_deleteEdge(e4fs);
   });
   edgemenu->addAction("-- place 2 --");

   vertexmenu= new QMenu("vertex menu");
   vertexmenu->addAction("Delete", this, [this](){
      // collect only the vertex at the cursor hotspot, 'upon click'
      QList<QGraphicsItem *> del_vertex= selectedItems();
      // either operate on a GraphVertex object or, abort
      if(del_vertex.isEmpty() || del_vertex.first()->type() != GraphVertex::Type)
         return ;
      // cast a 'QGraphicsItem *' as a 'GraphVertex *' in order to access
      // GraphVertex members
      GraphVertex * v4fs= qgraphicsitem_cast<GraphVertex *>(del_vertex.first());
      // 'DRY': pass to general vertex delete function
      gf_deleteVertex(v4fs);
   });
   // retain this as a 'what if' function? (to do) Undo function should leave a
   // right-click LC as a viable option
   vertexmenu->addAction("Local_Complementation", this, [this](){
      // collect only the vertex at the cursor hotspot, 'upon click'
      QList<QGraphicsItem *> lc_vertex= selectedItems();
      // either operate on a GraphVertex object or, abort
      if(lc_vertex.isEmpty() || lc_vertex.first()->type() != GraphVertex::Type)
         return ;
      // cast a 'QGraphicsItem *' as a 'GraphVertex *' so as to access GraphVertex
      // members
      GraphVertex * lcv= qgraphicsitem_cast<GraphVertex *>(lc_vertex.first());
      // vertex X must have >= 1 edge for LC: abort
      if(lcv->lcEdges()->isEmpty()){
         lcv->setSelected(false);
         return ;
      }
      // 'DRY': pass to general LC function
      gf_localComplementation(lcv); });
   vertexmenu->addAction("-- place 3 --");
}

void GraphFrame::gf_deleteEdge(GraphEdge * e4fs) {
   // delete a (Graph)edge
   // pre-condition: target object is type, GraphEdge
   // post-condition: target (Graph)edge is removed, any formerly connected
   //   (Graph)vertices are unaffected

   // remove the edge from 'edges' container of both vertices...
   e4fs->p1v()->removeEdge(e4fs);
   e4fs->p2v()->removeEdge(e4fs);

   // back the edge out of GraphFrame...
   removeItem(e4fs);
   // deallocate the memory
   delete e4fs;
}

void GraphFrame::gf_deleteVertex(GraphVertex *v4fs) {
   // delete a (Graph)vertex and its connected (Graph)edges
   // pre-condition: target object is type, GraphVertex
   // post-condition: target (Graph)vertex and all associated (Graph)edges are
   //   removed; the ID of each remaining vertex is reset

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
      unsigned int id {1};   // TO DO: mutable counter; vertices.size()?

      // ... reset the id numbering of each vertex
      for (GraphVertex * v : qAsConst(vertices)) {
         v->setVertexID(id);
         v->update();
         id++;
      }
   }
}

void GraphFrame::gf_localComplementation(GraphVertex * lcv) {
   // LC applied to (focus) vertex X: all vertices of X's neighbourhood not
   // joined by a (Graph)edge to gain an edge and conversely
   // pre-condition: target object is type, GraphVertex
   // post-condition: edges are reconfigured to LC status

   // LC operation: vertex X has one (1) edge
   if(lcv->lcEdges()->count() == 1){
      lcv->removeEdges();
      //lcv->setSelected(false);
   }
   // LC operation: vertex X has > 1 edge
   else {
      // container: neighbour vertices of vertex X
      QVector<GraphVertex *> neighbourvs {};

      // capture the neighbour vertices of vertex X (i.e. the vertex 'at the
      // opposite end' of any edge shared with vertex X); note the dereference
      // operation on lcv->lcEdges()
      for (GraphEdge * e : *lcv->lcEdges()) {
         // copy the GraphVertex * in order then to access the address of the
         // underlying object
         GraphVertex * foraddrofp1v= e->p1v();
         // test whether pointed-to-objects are equivalent by comparing their
         // addresses
         if(foraddrofp1v != lcv)
            neighbourvs.push_back(e->p1v());
         else
            neighbourvs.push_back(e->p2v());
      }
//qDebug() << "vertex X pos():" << lcv->pos() << "neighbourvs:" << neighbourvs.mid(0);
      // Vector: any neighbour vertices for (edge) ADD operation
      QVector<QPair<GraphVertex *, GraphVertex *>> a_vertices {};
      // Vector: any edges of neighbour vertices for DELETE operation
//      QVector<GraphEdge *> d_edges {};

      // assemble edge collections: ADD and DELETE
      for (GraphVertex * p_nvx : neighbourvs) {

         // edge collection: ADD
         QVector<GraphVertex *> add_neighbourvs {};
         add_neighbourvs= neighbourvs;
         // create copy of neighbourvs less the in-scope vertex, p_nvx
         add_neighbourvs.removeAll(p_nvx);
//qDebug() << "add_neighbourvs:" << add_neighbourvs.count();
         for (GraphVertex * p_other_nvx : qAsConst(add_neighbourvs) ) {
            // iterate over the (Graph)edges connected to the neighbour vertex
            // of in-scope vertex, p_nvx
            for (GraphEdge * e : *p_other_nvx->lcEdges()) {
               GraphVertex * other_v_1= e->p1v();
               GraphVertex * other_v_2= e->p2v();

               // logic: p_other_nvx must be other_v_1 or other_v_2 hence lcv/
               // ADD-vertex must be other_v_2 or other_v_1 thus, eliminate lcv
               // to get the target ADD-vertex

               // evaluate only those mutual neighbour vertices of (LC vertex),
               // lcv not connected by edge to p_nvx
               if((p_nvx != other_v_1 && p_nvx != other_v_2)
                     && (lcv != other_v_1 && lcv != other_v_2)){
                  // check: edge between p_nvx and target ADD-vertex
                  // represented as p1v <-> p2v is not missed in an iteration
                  // coded to fire at p2v <-> p1v, or conversely
                  //    state: target ADD-vertex == other_v_1 ?
                  if(p_other_nvx == other_v_1){
                     unsigned int counter {0};
                     // check: p_nvx is not already connected to target ADD-
                     // vertex
                     for (GraphEdge * e : *p_nvx->lcEdges()) {
                        GraphVertex * isp_nvxp1v= e->p1v();
                        GraphVertex * isp_nvxp2v= e->p2v();

                        if(p_other_nvx != isp_nvxp1v && p_other_nvx != isp_nvxp2v)
                           counter += 1;
                     }

                     // fires IFF there is no existing edge connecting p_nvx to
                     // target ADD-vertex, p_other_nvx
                     if(counter == (unsigned int) p_nvx->lcEdges()->count()){
                        QPair<GraphVertex *, GraphVertex *> add;
                        add.first= p_nvx;
                        add.second= p_other_nvx;

                        // collect add in a_vertices...
                        if(a_vertices.isEmpty())
                           a_vertices.push_back(add);
                        // but do enforce uniqueness of a_vertices elements
                        else {
                           QPair<GraphVertex *, GraphVertex *> addflip;
                           addflip.first= p_other_nvx;
                           addflip.second= p_nvx;

                           if(a_vertices.contains(add) || a_vertices.contains(addflip))
                              return ;
                           else
                              a_vertices.push_back(add);
                           }
                        }
                  }
                  // state: target ADD-vertex == other_v_2 ?
                  else if(p_other_nvx == other_v_2){
                     unsigned int counterp {0};
                     // check: p_nvx is not already connected to target ADD-
                     // vertex, p_other_nvx
                     for (GraphEdge * e : *p_nvx->lcEdges()) {
                        GraphVertex * isp_nvxp1v_p= e->p1v();
                        GraphVertex * isp_nvxp2v_p= e->p2v();

                        if(p_other_nvx != isp_nvxp1v_p
                              && p_other_nvx != isp_nvxp2v_p)
                           counterp += 1;
                     }

                     // fires IFF there is no existing edge connecting p_nvx to
                     // target ADD-vertex, p_other_nvx
                     if(counterp == (unsigned int) p_nvx->lcEdges()->count()){
                        QPair<GraphVertex *, GraphVertex *> add;
                        add.first= p_nvx;
                        add.second= p_other_nvx;

                        // collect add in a_vertices...
                        if(a_vertices.isEmpty())
                           a_vertices.push_back(add);
                        // but enforce uniqueness of a_vertices elements
                        else {
                           QPair<GraphVertex *, GraphVertex *> addflip;
                           addflip.first= p_other_nvx;
                           addflip.second= p_nvx;

                           if(a_vertices.contains(add) || a_vertices.contains(addflip))
                              return ;
                           else
                              a_vertices.push_back(add);
                           }
                        }
                  }
               }
            }
         }
qDebug() << "p_nvx->pos():" << p_nvx->pos() << "a_vertices:" << a_vertices.mid(0);
/*         // edge collection: DELETE
         for (GraphEdge * e : *p_nvx->lcEdges()) {
            // derive the GraphVertex * in order then to access the address of
            // the underlying object
            GraphVertex * delp1v= e->p1v();
            GraphVertex * delp2v= e->p2v();

            // DELETE antecedent 1: (both vertices are neighbours of vertex X)
            if((neighbourvs.contains(delp1v)
                && neighbourvs.contains(delp2v))
                  // ^ DELETE antecedent 2: (neither vertex is vertex X)
                  && (delp1v != lcv && delp2v != lcv)){
               // consequent: collect in d_edges any edge which connects two
               // neighbours of vertex X
               d_edges.push_back(e);
            }
         }
         // IFF p_nvx has no connecting edge to another neighbour of vertex X,
         // add a common edge

         // IFF p_nvx has a connecting edge to another neighbour of vertex X,
         // delete the common edge
         for (GraphEdge * e : d_edges ) {
            gf_deleteEdge(e);
         }*/
      }
   }
}

