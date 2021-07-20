#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphvertex.h"

#include <QMainWindow>

class GraphFrame;

QT_BEGIN_NAMESPACE
class QAction;
namespace Ui { class MainWindow; }
class QGraphicsView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow(QWidget * parent = nullptr);
   ~MainWindow();
/*
private slots:
   // mainwindow executes GraphFrame instructions
   void changeColour_edge();
   void changeColour_vertex();

   void graphItem_Select(QGraphicsItem * item);
   void graphItem_setToFront();
   void graphItem_setToRear();

   void vertex_Delete();
   void vertex_Insert(GraphVertex * vertex);
*/
private:
   Ui::MainWindow * ui;
   GraphFrame * graphframe;

   //QAction * addAction;
   //QAction * deleteAction;
   //QAction * exitAction;
   //QAction * toBack;
   //QAction * toFront;

   //QMenu * fileMenu;
   //QMenu * graphItemMenu;
};
#endif // MAINWINDOW_H
