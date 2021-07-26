#ifndef GRAPHVERTEX_H
#define GRAPHVERTEX_H
#pragma once

#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QVector>

class GraphEdge;

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

class GraphVertex : public QGraphicsEllipseItem
{
// group members by noun
public:
   GraphVertex(QGraphicsItem * parent= nullptr);
   // resolve compile error 'undefined reference to XXX vtable', see: https://
   // stackoverflow.com/questions/14010922/qt-undefined-reference-to-vtable
   ~GraphVertex();

   //enum {Type = UserType + 15};

   void addEdge(GraphEdge * edge);
   void deleteEdge(GraphEdge * edge);
   void removeEdges();

   //int type() const override {return Type;}

protected:
   //void contextMenuEvent(QGraphicsSceneContextMenuEvent * event) override;
   //void keyPressEvent(QKeyEvent * event) override;
   void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

private:
   QVector<GraphEdge *> edges;
   QMenu * vertexmenu {};
};

#endif // GRAPHVERTEX_H
