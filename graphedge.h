#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H
#pragma once

#include <QGraphicsLineItem>

class GraphVertex;  // for GraphEdge constructor

class GraphEdge : public QGraphicsLineItem
{
// group members by noun
public:
   GraphEdge();
   //~GraphEdge();
};

#endif // GRAPHEDGE_H
