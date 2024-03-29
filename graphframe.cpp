#include "graphframe.hpp"
#include "mainwindow.hpp"

#include <QFile>
#include <QGraphicsSceneMouseEvent>
#include <QJsonArray>
#include <QJsonDocument>
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


// GraphFrame-level read instructions for file loading (format: JSON)
void GraphFrame::openGraph(QString rfile) {
   QFile loadfile(rfile);
   if(!loadfile.open(QIODevice::ReadOnly))
      return ;

   QByteArray jsondata= loadfile.readAll();

   QJsonArray vertexarray= QJsonDocument::fromJson(jsondata).array();

   // 1. replicate vertices
   vertices.clear();
   for (int i= 0; i < vertexarray.size(); ++i) {
      QJsonObject vertex= vertexarray[i].toObject();

      GraphVertex * loadvertex= read(vertex);

      // collect the (GraphVertex *) vertex
      vertices.push_back(loadvertex);
      // add the vertex to the GraphFrame
      addItem(loadvertex);
   }

   // 2. replicate edges
   QMap<double, GraphVertex*> vertexmap {};
   for (GraphVertex * v : qAsConst(vertices)) {
      double kmangle= v->pos().x() + v->pos().y();
      vertexmap[kmangle]= v;
   }
   QVector<QPair<QPointF,QPointF>> edgecheck {};

   for (GraphVertex * v : qAsConst(vertices)) {
      GraphVertex * p1v {};
      GraphVertex * p2v {};

      for (QPair<QPointF,QPointF> pospair : qAsConst(v->edgemirror)) {

         // recreate GraphEdge once
         QPair<QPointF,QPointF> flip= QPair(pospair.second, pospair.first);
         if(edgecheck.contains(pospair) || edgecheck.contains(flip))
            continue;

         if(pospair.first == v->pos()){
            p1v= v;
            double sndmangle= pospair.second.x() + pospair.second.y();
            p2v= vertexmap[sndmangle];
         }
         else {
            double fstmangle= pospair.first.x() + pospair.first.y();
            p1v= vertexmap[fstmangle];
            p2v= v;
         }

         GraphEdge * e= new GraphEdge(p1v, p2v, edgemenu);

         // add the edge to (QVector) 'edges' of vertices p1 and p2
         p1v->addEdge(e);
         p2v->addEdge(e);

         edgecheck.push_back(pospair);

         // add GraphEdge to the GraphFrame container
         addItem(e);
         p1v= 0;
         p2v= 0;
      }
   }
}

// GraphVertex-level operation for GraphFrame::openGraph
GraphVertex * GraphFrame::read(const QJsonObject & json) {
   unsigned int vid {};
   QPointF vpos {};

   if(json.contains("vid") && json["vid"].isDouble())
      vid= json["vid"].toInt();

   if(json.contains("vpos") && json["vpos"].isArray()){
      QJsonArray vertexposxy= json["vpos"].toArray();

      qreal vposx= vertexposxy[0].toDouble();
      qreal vposy= vertexposxy[1].toDouble();
      vpos= QPointF(vposx, vposy);
   }

   // instantiate new GraphVertex
   GraphVertex * v;
   v= new GraphVertex(vertexmenu, vid);
   // set vertex position
   v->setPos(vpos);
   // build (GraphVertex) edgemirror
   if(json.contains("vedges") && json["vedges"].isArray())
      v->readEdges(json);

   return v;
}

// GraphFrame-level write instructions for file saving (format: JSON)
void GraphFrame::saveGraph(QString wfile) const {
   QFile savefile(wfile);
   if(!savefile.open(QIODevice::WriteOnly))
      return ;

   QJsonArray loadvertices;
   for (GraphVertex * v : vertices) {
      QJsonObject graphvertexobject;
      v->write(graphvertexobject);
      loadvertices.append(graphvertexobject);
   }
   savefile.write(QJsonDocument(loadvertices).toJson());
}


// protected:
void GraphFrame::keyPressEvent(QKeyEvent * event) {
   QVector<int> cursorflags {Qt::Key_E
            , Qt::Key_O
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
   case Qt::Key_O:
      setCursorLabel("O");
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

   // abort: user has not completed entire LPM X operation in contiguous steps,
   // (see 'part1 -> operation: X local Pauli measurement', below)
   if(clabel->text() != "X" && lpmX_mpe2_FT == true){
      // drop the coloured vertex prompts
      for (GraphVertex * v : qAsConst(vertices)) {
         v->resetColour(Qt::black);
      }

      lpmX_mpe2_FT= false;
      x_lcv1->clearNeighbours();
      x_lcv1= 0;
   }

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

   // operation: Local Complementation
   else if(clabel->text() == "O"){
      // collect the vertex at the cursor hotspot, 'upon click'
      QList<QGraphicsItem *> lc_vertex= items(event->scenePos()
                                            , Qt::ContainsItemShape);

      // prevent a runtime exception caused by nothing being at the cursor
      // hotspot, 'upon click'.  Either operate on a GraphVertex object or,
      // abort
      if(lc_vertex.isEmpty() || lc_vertex.first()->type() != GraphVertex::Type){
         cursorState(false);
         clabel->clear();
         return ;
      }

      // cast a 'QGraphicsItem *' as a 'GraphVertex *' so as to access GraphVertex
      // members
      GraphVertex * lcv= qgraphicsitem_cast<GraphVertex *>(lc_vertex.first());

      // vertex X must have >= 1 edge for LC: abort
      if(lcv->lcEdges()->isEmpty()){
         cursorState(false);
         clabel->clear();
         return ;
      }

      // 'DRY': pass to general LC function
      gf_localComplementation(lcv);
      lcv->clearNeighbours();

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

   // part1 -> operation: X local Pauli measurement
   else if(clabel->text() == "X" && lpmX_mpe2_FT == false){
      // collect vertex 1 at the cursor hotspot, 'upon click'
      QList<QGraphicsItem *> lc_vertex= items(event->scenePos()
                                            , Qt::ContainsItemShape);

      // prevent a runtime exception caused by nothing being at the cursor
      // hotspot, 'upon click'.  Either operate on a GraphVertex object or,
      // abort
      if(lc_vertex.isEmpty() || lc_vertex.first()->type() != GraphVertex::Type){
         cursorState(false);
         clabel->clear();
         return ;
      }

      // cast a 'QGraphicsItem *' as a 'GraphVertex *' (i.e. vertex 1) to
      // access GraphVertex members
      x_lcv1= qgraphicsitem_cast<GraphVertex *>(lc_vertex.first());

      // vertex 1 must have >= 1 edge for LC: abort
      if(x_lcv1->lcEdges()->isEmpty()){
         cursorState(false);
         clabel->clear();
         return ;
      }

      // populate QVector<GraphVertex *> of vertex 1 neighbours
      for (GraphEdge * e : *x_lcv1->lcEdges()) {
         // copy the GraphVertex * in order then to access the address of the
         // underlying object
         GraphVertex * foraddrofp1v= e->p1v();
         // test whether pointed-to-objects are equivalent by comparing their
         // addresses
         if(foraddrofp1v != x_lcv1)
            x_lcv1->addNeighbour(e->p1v());
         else
            x_lcv1->addNeighbour(e->p2v());
      }

      // copy of vertex 1 neighbours for X local Pauli measurement operations
      QVector<GraphVertex *> xneighbours= *x_lcv1->lcNeighbours();

      // colour and reformat each of vertex 1's neighbouring vertices as a user
      // prompt to select the next LC vertex
      for (GraphVertex * neighbour : qAsConst(xneighbours)) {
         neighbour->resetColour(QColor(0,255,0), 4, QColor(173,255,47));
      }
      // flag to enable selecting the (neighbour) vertex
      lpmX_mpe2_FT= true;
   }

   // part2 -> operation: X local Pauli measurement
   else if(clabel->text() == "X" && lpmX_mpe2_FT == true){
      // collect vertex 2 at the cursor hotspot, 'upon click'
      QList<QGraphicsItem *> lc_vertex= items(event->scenePos()
                                            , Qt::ContainsItemShape);

      // prevent a runtime exception caused by nothing being at the cursor
      // hotspot, 'upon click'.  Either operate on a GraphVertex object or,
      // abort
      if(lc_vertex.isEmpty() || lc_vertex.first()->type() != GraphVertex::Type){
         // restore all vertices to default colour scheme
         for (GraphVertex * v : qAsConst(vertices)) {
            v->resetColour(Qt::black);
         }
         lpmX_mpe2_FT= false;
         x_lcv1->clearNeighbours();
         x_lcv1= 0;

         cursorState(false);
         clabel->clear();
         return ;
      }

      // cast a 'QGraphicsItem *' as a 'GraphVertex *' (i.e. vertex 2) to
      // access GraphVertex members
      x_lcv2= qgraphicsitem_cast<GraphVertex *>(lc_vertex.first());

      // 'DRY': pass vertex 2 to general LC function
      gf_localComplementation(x_lcv2);

      // restore all vertices to default colour scheme
      for (GraphVertex * v : qAsConst(vertices)) {
         v->resetColour(Qt::black);
      }

      // QVector<GraphVertex *> of vertex 1 neighbours is now out of step with
      //     the rendered graph: clear it
      x_lcv1->clearNeighbours();

      // implement LPM Y on vertex 1
      // 'DRY': pass LPM X vertex 1 to general LC function
      gf_localComplementation(x_lcv1);
      // 'DRY': pass LPM X vertex 1 to general vertex delete function
      gf_deleteVertex(x_lcv1);

      // second LC on vertex 2:
      //    QVector<GraphVertex *> of vertex 2 neighbours is out of step with
      //    the rendered graph: clear it
      x_lcv2->clearNeighbours();
      //    implement LC on vertex 2
      gf_localComplementation(x_lcv2);

      // clean up
      lpmX_mpe2_FT= false;
      x_lcv1= 0;
      x_lcv2= 0;

      // reset CURSOR state
      cursorState(false);
      clabel->clear();
   }

   // operation: Y local Pauli measurement
   else if(clabel->text() == "Y"){
      // collect the vertex at the cursor hotspot, 'upon click'
      QList<QGraphicsItem *> lc_vertex= items(event->scenePos()
                                            , Qt::ContainsItemShape);

      // prevent a runtime exception caused by nothing being at the cursor
      // hotspot, 'upon click'.  Either operate on a GraphVertex object or,
      // abort
      if(lc_vertex.isEmpty() || lc_vertex.first()->type() != GraphVertex::Type){
         cursorState(false);
         clabel->clear();
         return ;
      }

      // cast a 'QGraphicsItem *' as a 'GraphVertex *' so as to access GraphVertex
      // members
      GraphVertex * lcv= qgraphicsitem_cast<GraphVertex *>(lc_vertex.first());

      // vertex X must have >= 1 edge for LC: abort
      if(lcv->lcEdges()->isEmpty()){
          gf_deleteVertex(lcv);
          cursorState(false);
          clabel->clear();
          return ;
      }

      // 'DRY': pass to general LC function
      gf_localComplementation(lcv);

      // 'DRY': pass to general vertex delete function
      gf_deleteVertex(lcv);

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
         cursorState(false);
         clabel->clear();
         return ;
      }

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
         // drop focus from the originating (Graph)vertex
         p1v->setSelected(false);
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
   edgemenu->addAction(tr("&Delete"), this, [this](){
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
//   edgemenu->addAction("-- place 2 --");

   vertexmenu= new QMenu("vertex menu");
   vertexmenu->addAction(tr("&Delete"), this, [this](){
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
//   vertexmenu->addAction("-- place 2 --");
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

void GraphFrame::gf_deleteVertex(GraphVertex * v4fs) {
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
       return ;
   }
   // LC operation: vertex X has > 1 edge
   else {
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
            lcv->addNeighbour(e->p1v());
         else
            lcv->addNeighbour(e->p2v());
      }

      // make a copy of lcv.neighbourvs
      const QVector<GraphVertex *> copy_neighbourvs= *lcv->lcNeighbours();
      // Vector: unique pairs of each (neighbour) vertex
      QVector<QPair<GraphVertex *, GraphVertex *>> all_unique_vertex_pairs {};

      for (GraphVertex * v : *lcv->lcNeighbours()) {
         QPointF vpos= v->pos();
         for (GraphVertex * v_copy : copy_neighbourvs) {
            QPointF vpos_copy= v_copy->pos();

            QPair<GraphVertex *, GraphVertex *> unique_vertex_pair {};
            unique_vertex_pair.first= v;
            unique_vertex_pair.second= v_copy;

            QPair<GraphVertex *, GraphVertex *> flipexc {};
            flipexc.first= v_copy;
            flipexc.second= v;

            // exclude 'auto-pairs' and pair reflections from
            // all_unique_vertex_pair
            if((vpos != vpos_copy)
                  && !all_unique_vertex_pairs.contains(flipexc))
               all_unique_vertex_pairs.push_back(unique_vertex_pair);
         }
      }
      // Vector: any edges of neighbour vertices for DELETE operation
      QVector<GraphEdge *> d_edges {};
      // Vector: any neighbouring vertices for (edge) ADD operation
      QVector<QPair<GraphVertex *, GraphVertex *>> a_vertices {};

      // for each neighbour vertex...
      for(QPair<GraphVertex *, GraphVertex *> vertex_pair
          : qAsConst(all_unique_vertex_pairs)) {
         bool add_TF {true};
         GraphVertex * p_nvx= vertex_pair.first;

         // ... check for an edge connecting it to its paired neighbour vertex
         if(p_nvx->lcEdges()->count() == 1){
            // p_nvx only has a connecting edge to vertex X; recall: vertex_pair
            // is unique, which proscribes need for a second uniqueness check
            a_vertices.push_back(vertex_pair);
            add_TF= false;
         }
         // p_nvx has >1 edge
         else {
            for (GraphEdge * e : *p_nvx->lcEdges()) {
               GraphVertex * p1v= e->p1v();
               GraphVertex * p2v= e->p2v();

               // collect the connecting edge for DELETE operation
               if(((p_nvx == p1v && vertex_pair.second == p2v)
                        || (p_nvx == p2v && vertex_pair.second == p1v))
                     && !d_edges.contains(e)){
                  d_edges.push_back(e);
                  add_TF= false;
               }
            }
		    }
         // collect the paired vertices for ADD operation
         if(add_TF == true && !a_vertices.contains(vertex_pair))
            a_vertices.push_back(vertex_pair);
		 }
//qDebug() << "lcv->pos():" << lcv->pos() << "\nd_edges:" << d_edges.mid(0) << "\na_vertices:" << a_vertices.mid(0) << "\na_vertices.count()" << a_vertices.count();
      // IFF p_nvx has no connecting edge to another neighbour of vertex X, ADD
      // a common edge
      if(!a_vertices.isEmpty()){
         for (QPair<GraphVertex *, GraphVertex *> add_pair : a_vertices) {
            GraphEdge * e= new GraphEdge(add_pair.first, add_pair.second
                                         , edgemenu);

            // add the edge to (QVector) 'edges' of vertices p1 and p2
            add_pair.first->addEdge(e);
            add_pair.second->addEdge(e);

            // add a QGraphicsLineItem (GraphEdge) to the GraphFrame
            // (QGraphicsScene) container
            addItem(e);
         }
      }
      // IFF p_nvx has a connecting edge to another neighbour of vertex X,
      // DELETE the common edge
      if(!d_edges.isEmpty()){
         for (GraphEdge * e : d_edges ) {
            gf_deleteEdge(e);
         }
      }
   }
}

