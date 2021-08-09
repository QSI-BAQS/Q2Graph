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

   ui->graphicsView->setMouseTracking(true);
   setWindowTitle("q2graph_GVF_v0.3");
   /*
   clabel= new QLabel;
   clabel->setWindowFlag(Qt::ToolTip);
   clabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");*/
}

MainWindow::~MainWindow()
{
   delete ui;
}

