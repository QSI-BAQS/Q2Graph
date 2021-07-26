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
   setWindowTitle("Graph 3.0_gvf");

}

MainWindow::~MainWindow()
{
   delete ui;
}

