#include "graphframe.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>


// public:
MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   createMenus();

   graphframe= new GraphFrame(this);
   ui->graphicsView->setScene(graphframe);

   ui->graphicsView->setMouseTracking(true);
   setWindowTitle("Q2Graph");
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::createMenus() {
   fileMenu= menuBar()->addMenu(tr("&File"));
   fileMenu->addAction(tr("-- &Open --"));
   fileMenu->addAction(tr("-- &Save --"));
   fileMenu->addAction(tr("E&xit"), this, [this](){
      QWidget::close();
   });
}
