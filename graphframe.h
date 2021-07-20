#ifndef GRAPHFRAME_H
#define GRAPHFRAME_H
#pragma once

#include "graphvertex.h"

#include <QGraphicsScene>
#include <QLabel>
#include <QPointF>

QT_BEGIN_NAMESPACE
class QColor;
class QGraphicsEllipseItem;
class QGraphicsLineItem;
//class QMenu;
QT_END_NAMESPACE

class GraphFrame : public QGraphicsScene
{
   Q_OBJECT

// group members by noun
public:
   explicit GraphFrame(QWidget * parent = nullptr);
   ~GraphFrame();
/*
signals:
   void changeColour_edge();
   void changeColour_vertex();

   //QGraphicsItem, base class to QAbstractGraphicsShapeItem, QGraphicsLineItem
   void graphItem_Select(QGraphicsItem * item);
   void graphItem_setToFront();
   void graphItem_setToRear();

   void vertex_Delete();
   void vertex_Insert(GraphVertex * vertex);*/

protected:
   void keyPressEvent(QKeyEvent * event) override;
   void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
   void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
   //void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

private:
   bool ft[2] {false,true};
   QLabel * label;
   //QPointF * p_lastpos;
   const bool * p_cursorFT {&ft[0]};
   QRectF vertexBoundary;

   void cursorState(bool setTF);
   void setCursorLabel(QString tag);

};

#endif // GRAPHFRAME_H
