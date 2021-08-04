#ifndef GRAPHVERTEX_H
#define GRAPHVERTEX_H
#pragma once

#include <QDebug>

#include <QBrush>
#include <QFont>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSimpleTextItem>
#include <QPen>
/*
QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE
*/
class GraphVertex : public QGraphicsEllipseItem
{
// group members by noun
public:
   GraphVertex(QGraphicsItem * parent= nullptr);
   // resolve compile error 'undefined reference to XXX vtable', see: https://
   // stackoverflow.com/questions/14010922/qt-undefined-reference-to-vtable
   ~GraphVertex();

   // for establishing instance ID
   enum {Type= UserType + 15};

   void setVertexID(int vscount);

   int type() const override { return Type; }

protected:
   //void contextMenuEvent(QGraphicsSceneContextMenuEvent * event) override;
   void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

private:
   QRectF vertexboundaryrect {QPointF(0.0,0.0), QSizeF(20.0,20.0)};
   QPen vertexcircumferencepen {Qt::black, 2};
   QBrush vertexfill {QColor::fromRgb(245,245,245)};   // white smoke

   QGraphicsSimpleTextItem * vertexid {};
   QBrush vertexidbrush {Qt::darkBlue};
   QFont vertexidfont {"SansSerif", 8, QFont::Normal};
   //QMenu * vertexmenu {};
};

#endif // GRAPHVERTEX_H
