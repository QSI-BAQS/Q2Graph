#ifndef GRAPHFRAME_H
#define GRAPHFRAME_H
#pragma once

#include "graphedge.h"
#include "graphvertex.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QJsonObject>
#include <QLabel>
#include <QPointF>

QT_BEGIN_NAMESPACE
class QColor;
class QGraphicsEllipseItem;
class QGraphicsItem;
class QGraphicsLineItem;
QT_END_NAMESPACE

class GraphFrame : public QGraphicsScene
{
   Q_OBJECT

public:
   explicit GraphFrame(QWidget * parent= nullptr);
   ~GraphFrame();

   void openGraph(QString);
   GraphVertex * read(const QJsonObject &);
   void saveGraph(QString) const;
//   void write(QJsonObject &) const;

protected:
   void keyPressEvent(QKeyEvent *) override;
   void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;
   void mousePressEvent(QGraphicsSceneMouseEvent *) override;
   void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;

private:
   bool ft[2] {false,true};
   QLabel * clabel {};
   QMenu * edgemenu {};
   QMenu * vertexmenu {};
   QGraphicsLineItem * tracer {};
   const bool * p_cursorFT {&ft[0]};

   QList<GraphVertex *> vertices {};

   bool lpmX_mpe2_FT {false};
   GraphVertex * x_lcv1;
   GraphVertex * x_lcv2;

   GraphVertex * writevertex;
   QVector<QPair<QPointF,QPointF>> opengraph_vertices;

   void cursorState(bool);
   void setCursorLabel(QString);

   void createMenus();

   void gf_deleteEdge(GraphEdge *);
   void gf_deleteVertex(GraphVertex *);
   void gf_localComplementation(GraphVertex *);
};

#endif // GRAPHFRAME_H
