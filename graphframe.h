#ifndef GRAPHFRAME_H
#define GRAPHFRAME_H
#pragma once

#include "graphedge.h"
#include "graphvertex.h"

#include <QDebug>
#include <QGraphicsScene>
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
   explicit GraphFrame(QWidget * parent = nullptr);
   ~GraphFrame();

protected:
   void keyPressEvent(QKeyEvent * event) override;
   void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
   void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
   void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;


private slots:
   void deleteEdge();
   void deleteVertex();

private:
   bool ft[2] {false,true};
   QLabel * clabel {};
   QMenu * edgemenu {};
   QMenu * vertexmenu {};
   QGraphicsLineItem * tracer {};
   const bool * p_cursorFT {&ft[0]};

   QAction * e_deleteaction;
   QAction * v_deleteaction;

   void createActions();

   void cursorState(bool setTF);
   void setCursorLabel(QString tag);

   void createMenus();

   QList<GraphVertex *> collectVertices();
};

#endif // GRAPHFRAME_H
