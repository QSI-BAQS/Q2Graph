#include "graphedge.h"
#include "graphframe.h"
#include "graphvertex.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   graphframe= new GraphFrame(this);
   ui->graphicsView->setScene(graphframe);
   /*
   connect(graphframe, &GraphFrame::changeColour_edge, this, &MainWindow::changeColour_edge);
   connect(graphframe, &GraphFrame::changeColour_vertex, this, &MainWindow::changeColour_vertex);
   connect(graphframe, &GraphFrame::graphItem_Select, this, &MainWindow::graphItem_Select);
   connect(graphframe, &GraphFrame::graphItem_setToFront, this, &MainWindow::graphItem_setToFront);
   connect(graphframe, &GraphFrame::graphItem_setToRear, this, &MainWindow::graphItem_setToRear);
   connect(graphframe, &GraphFrame::vertex_Delete, this, &MainWindow::vertex_Delete);
   connect(graphframe, &GraphFrame::vertex_Insert, this, &MainWindow::vertex_Insert);
*/
   ui->graphicsView->setMouseTracking(true);
   setWindowTitle("Graph 3.0_gvf");

}

MainWindow::~MainWindow()
{
   delete ui;
}

